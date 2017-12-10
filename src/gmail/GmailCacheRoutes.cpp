#include <QDir>
#include <QFile>
#include "GmailCacheRoutes.h"
#include "GmailRoutes.h"
#include "Endpoint.h"

using namespace googleQt;

#define EXPECT_STRING_VAL(S, W) if(S.isEmpty()){                        \
        qWarning() << "Expected for cache setup:" << W << "Call setupSQLiteCache first."; \
        return false;                                                   \
    }                                                                   \

mail_cache::GmailCacheRoutes::GmailCacheRoutes(Endpoint& endpoint, 
                                            GmailRoutes& gmail_routes)
    :m_endpoint(endpoint),
    m_gmail_routes(gmail_routes)
{

};

std::unique_ptr<UserBatchResult<QString, 
                                messages::MessageResource>> mail_cache::GmailCacheRoutes::getUserBatchMessages(QString userId,
                                                                                                   EDataState f,
                                                                                                   const std::list<QString>& id_list)
{
    return getUserBatchMessages_Async(userId, f, id_list)->waitForResultAndRelease();
};

void mail_cache::GmailCacheRoutes::ensureCache()const
{
    if (!m_GMailCache) {
        m_GMailCache.reset(new mail_cache::GMailCache(m_endpoint));
    }
};


mail_cache::GMailCacheQueryTask* mail_cache::GmailCacheRoutes::newResultFetcher(QString userId, EDataState state)
{
    int accId = 0;
    if (m_GMailCache) {
        auto storage = m_GMailCache->sqlite_storage();
        if (!storage) {
            qWarning() << "ERROR. Expected cache storage";
        }
        else {
            accId = storage->findAccount(userId);
        }
    }
    else {
        qWarning() << "ERROR. Expected cache object";
    }

    mail_cache::GMailCacheQueryTask* rfetcher = new mail_cache::GMailCacheQueryTask(userId,
        accId,
        state,
        m_endpoint,
        *this,
        m_GMailCache);

    return rfetcher;
};


UserBatchRunner<QString,
                mail_cache::MessagesReceiver,
                messages::MessageResource>* mail_cache::GmailCacheRoutes::getUserBatchMessages_Async(QString userId,
                                                                                         EDataState f, 
                                                                                         const std::list<QString>& id_list)
{
    mail_cache::MessagesReceiver* mr = new mail_cache::MessagesReceiver(m_gmail_routes, userId, f);
    
    UserBatchRunner<QString,
                    mail_cache::MessagesReceiver,
                    messages::MessageResource>* r = new UserBatchRunner<QString,
                                                                        mail_cache::MessagesReceiver,
                                                                        messages::MessageResource>(id_list, mr, m_endpoint);
    r->run();
    return r;
};

mail_cache::data_list_uptr mail_cache::GmailCacheRoutes::getCacheMessages(QString userId, EDataState state, const std::list<QString>& id_list)
{
    return getCacheMessages_Async(userId, state, id_list)->waitForResultAndRelease();
};


mail_cache::GMailCacheQueryTask* mail_cache::GmailCacheRoutes::getCacheMessages_Async(QString userId, EDataState state,
    const std::list<QString>& id_list,
    mail_cache::GMailCacheQueryTask* rfetcher /*= nullptr*/)
{
    ensureCache();
    if (!rfetcher)
    {
        rfetcher = newResultFetcher(userId, state);
    }
    m_GMailCache->query_Async(state, id_list, rfetcher);
    return rfetcher;
};

mail_cache::data_list_uptr mail_cache::GmailCacheRoutes::getCacheMessages(int numberOfMessages, uint64_t labelFilter)
{
    ensureCache();
    mail_cache::GMailCacheQueryTask* rfetcher = newResultFetcher(m_endpoint.apiClient()->userId(), EDataState::snippet);
    m_GMailCache->topCacheData(rfetcher, numberOfMessages, labelFilter);
    return rfetcher->waitForResultAndRelease();
};


mail_cache::GMailCacheQueryTask* mail_cache::GmailCacheRoutes::getNextCacheMessages_Async(QString userId,
    int messagesCount /*= 40*/,
    QString pageToken /*= ""*/,
    QStringList* labels /*= nullptr*/)
{
    mail_cache::GMailCacheQueryTask* rfetcher = newResultFetcher(userId, EDataState::snippet);

    gmail::ListArg listArg;
    listArg.setMaxResults(messagesCount);
    listArg.setPageToken(pageToken);
    if (labels)
    {
        listArg.labels() = *labels;
    }

    m_gmail_routes.getMessages()->list_Async(listArg)->then([=](std::unique_ptr<messages::MessageListRes> mlist)
    {
        std::list<QString> id_list;
        for (auto& m : mlist->messages())
        {
            id_list.push_back(m.id());
        }
        rfetcher->setNextPageToken(mlist->nextpagetoken());
        getCacheMessages_Async(userId, EDataState::snippet, id_list, rfetcher);
    },
        [=](std::unique_ptr<GoogleException> ex)
    {
        rfetcher->failed_callback(std::move(ex));
    });

    return rfetcher;
};

mail_cache::data_list_uptr mail_cache::GmailCacheRoutes::getNextCacheMessages(QString userId,
    int messagesCount /*= 40*/,
    QString pageToken /*= ""*/,
    QStringList* labels /*= nullptr*/)
{
    return getNextCacheMessages_Async(userId,
        messagesCount,
        pageToken,
        labels)->waitForResultAndRelease();
};

GoogleVoidTask* mail_cache::GmailCacheRoutes::trashCacheMessage_Async(QString userId, QString msg_id)
{
    ensureCache();
    GoogleVoidTask* rv = m_endpoint.produceVoidTask();
    googleQt::gmail::TrashMessageArg arg(userId, msg_id);
    m_gmail_routes.getMessages()->trash_Async(arg)->then([=]()
    {
        //clean up cache
        auto storage = m_GMailCache->sqlite_storage();
        if (storage) {
            int accId = storage->findAccount(userId);
            if (accId != -1) {
                std::set<QString> set2remove;
                set2remove.insert(msg_id);
                m_GMailCache->persistent_clear(accId, set2remove);
                storage->deleteAttachmentsFromDb(msg_id);
            }
        }
        rv->completed_callback();
    },
        [=](std::unique_ptr<GoogleException> ex) {
        rv->failed_callback(std::move(ex));
    });
    return rv;
};

bool mail_cache::GmailCacheRoutes::setupSQLiteCache(QString dbPath,
    QString downloadPath,
    QString dbName /*= "googleqt"*/,
    QString dbprefix /*= "api"*/)
{
    ensureCache();

    if (m_GMailCache->hasLocalPersistentStorate())
    {
        return true;
    }

    EXPECT_STRING_VAL(m_endpoint.client()->userId(), "UserId");
    EXPECT_STRING_VAL(dbPath, "DB path");
    EXPECT_STRING_VAL(downloadPath, "Download path");
    EXPECT_STRING_VAL(dbName, "DB name");
    EXPECT_STRING_VAL(dbprefix, "DB prefix");


    std::unique_ptr<mail_cache::GMailSQLiteStorage> st(new mail_cache::GMailSQLiteStorage(m_GMailCache));
    if (!st->init_db(dbPath, downloadPath, dbName, dbprefix))
    {
        m_GMailCache->invalidate();
        qWarning() << "Failed to initialize SQLite storage" << dbPath << dbName << dbprefix;
        return false;
    }

    m_GMailCache->setupLocalStorage(st.release());

    if (googleQt::isConnectedToNetwork()) {
        refreshLabels();
    }

    return true;
};

bool mail_cache::GmailCacheRoutes::resetSQLiteCache()
{
    if (!m_GMailCache) {
        qWarning("GMail cache is not defined. Please call setupSQLiteCache first.");
        return false;
    }

    if (!m_GMailCache->hasLocalPersistentStorate()) {
        qWarning("DB storage of GMail cache is not defined. Please call setupSQLiteCache first.");
        return false;
    }

    EXPECT_STRING_VAL(m_endpoint.client()->userId(), "UserId");
    mail_cache::GMailSQLiteStorage* storage = m_GMailCache->sqlite_storage();
    if (!storage) {
        qWarning("DB storage of GMail cache is not defined. Please call setupSQLiteCache first.");
        return false;
    }
    QString dbPath = storage->m_dbPath;
    QString downloadPath = storage->m_downloadDir;
    QString dbName = storage->m_dbName;
    QString dbprefix = storage->m_metaPrefix;

    EXPECT_STRING_VAL(dbPath, "DB path");
    EXPECT_STRING_VAL(downloadPath, "Download path");
    EXPECT_STRING_VAL(dbName, "DB name");
    EXPECT_STRING_VAL(dbprefix, "DB prefix");

    if (m_GMailCache) {
        m_GMailCache->invalidate();
    }
    m_GMailCache.reset(new mail_cache::GMailCache(m_endpoint));

    return setupSQLiteCache(dbPath, downloadPath, dbName, dbprefix);
};

bool mail_cache::GmailCacheRoutes::hasCache()const
{
    if (m_GMailCache)
        return true;
    return false;
};


GoogleVoidTask* mail_cache::GmailCacheRoutes::refreshLabels_Async()
{
    ensureCache();

    GoogleVoidTask* rv = m_endpoint.produceVoidTask();

    googleQt::GoogleTask<labels::LabelsResultList>* t = m_gmail_routes.getLabels()->list_Async();
    t->then([=](std::unique_ptr<labels::LabelsResultList> lst)
    {
        auto storage = m_GMailCache->sqlite_storage();
        if (storage) {
            for (auto& lbl : lst->labels())
            {
                QString label_id = lbl.id();
                auto db_lbl = storage->findLabel(label_id);
                if (db_lbl)
                {
                    storage->updateDbLabel(lbl);
                }
                else
                {
                    storage->insertDbLabel(lbl);
                }
            }
        }
        rv->completed_callback();
    },
        [=](std::unique_ptr<GoogleException> ex) {
        rv->failed_callback(std::move(ex));
    });
    return rv;
};

GoogleVoidTask* mail_cache::GmailCacheRoutes::downloadAttachment_Async(googleQt::mail_cache::msg_ptr m,
    googleQt::mail_cache::att_ptr a,
    QString destinationFolder)
{
    ensureCache();
    GoogleVoidTask* rv = m_endpoint.produceVoidTask();

    if (a->status() == mail_cache::AttachmentData::statusDownloadInProcess) {
        qWarning() << "attachment download already in progress " << m->id() << a->attachmentId();
    }
    auto storage = m_GMailCache->sqlite_storage();
    if (!storage) {
        qWarning() << "ERROR. Expected storage object.";
        rv->completed_callback();
        return rv;
    }

    QString userId = storage->findUser(m->accountId());
    if (userId.isEmpty()) {
        qWarning() << "ERROR. Failed to locate userId" << m->id() << m->accountId();
        rv->completed_callback();
        return rv;
    }

    a->m_status = mail_cache::AttachmentData::statusDownloadInProcess;
    gmail::AttachmentIdArg arg(userId, m->id(), a->attachmentId());
    auto t = m_gmail_routes.getAttachments()->get_Async(arg);
    t->then([=](std::unique_ptr<attachments::MessageAttachment> att)
    {
        QDir dest_dir;
        if (!dest_dir.exists(destinationFolder)) {
            if (!dest_dir.mkpath(destinationFolder)) {
                qWarning() << "Failed to create directory" << destinationFolder;
                return;
            };
        }
        
        QString destFile = destinationFolder + "/" + a->filename();
        if (QFile::exists(destFile)) {
            //create some reasonable unique file name
            QFileInfo fi(destFile);
            QString name = fi.baseName().left(64);
            QString ext = fi.suffix();
            int idx = 1;
            while (idx < 1000) {
                destFile = destinationFolder + "/" + name + QString("_%1").arg(idx) + "." + ext;
                if (!QFile::exists(destFile))
                    break;
            }
        }

        QFile file_in(destFile);
        if (file_in.open(QFile::WriteOnly)) {
            file_in.write(QByteArray::fromBase64(att->data(), QByteArray::Base64UrlEncoding));
            file_in.close();

            QFileInfo fi(destFile);
            auto storage = m_GMailCache->sqlite_storage();
            if (storage) {
                storage->update_attachment_local_file_db(m, a, fi.fileName());
            }
            emit attachmentsDownloaded(m, a);
        }
        else {
            qWarning() << "Failed to create attachment file" << destFile;
        }
        rv->completed_callback();
    });
    return rv;
};

void mail_cache::GmailCacheRoutes::refreshLabels()
{
    refreshLabels_Async()->waitForResultAndRelease();
};

std::list<mail_cache::LabelData*> mail_cache::GmailCacheRoutes::getLoadedLabels(std::set<QString>* in_optional_idset)
{
    ensureCache();
    auto storage = m_GMailCache->sqlite_storage();
    if (!storage) {
        std::list<mail_cache::LabelData*> on_error;
        return on_error;
    }
    return storage->getLabelsInSet(in_optional_idset);
};

std::list<mail_cache::LabelData*> mail_cache::GmailCacheRoutes::getMessageLabels(mail_cache::MessageData* d)
{
    ensureCache();
    auto storage = m_GMailCache->sqlite_storage();
    if (!storage) {
        std::list<mail_cache::LabelData*> on_error;
        return on_error;
    }
    return storage->unpackLabels(d->labelsBitMap());
};

#define TRY_LABEL_MODIFY(F, M, S) bool rv = false;                      \
    try                                                                 \
        {                                                               \
            std::unique_ptr<messages::MessageResource> m = F(M, S)->waitForResultAndRelease(); \
            rv = (!m->id().isEmpty());                                  \
        }                                                               \
    catch (GoogleException& e)                                          \
        {                                                               \
            qWarning() << "setLabel Exception: " << e.what();           \
        }                                                               \
    return rv;                                                          \



bool mail_cache::GmailCacheRoutes::setStarred(mail_cache::MessageData* d, bool set_it)
{
    TRY_LABEL_MODIFY(setStarred_Async, d, set_it);
};

GoogleTask<messages::MessageResource>* mail_cache::GmailCacheRoutes::setStarred_Async(mail_cache::MessageData* d, bool set_it)
{
    return setLabel_Async(mail_cache::sysLabelId(mail_cache::SysLabel::STARRED), d, set_it, true);
};

bool mail_cache::GmailCacheRoutes::setUnread(mail_cache::MessageData* d, bool set_it)
{
    TRY_LABEL_MODIFY(setUnread_Async, d, set_it);
};

GoogleTask<messages::MessageResource>* mail_cache::GmailCacheRoutes::setUnread_Async(mail_cache::MessageData* d, bool set_it)
{
    return setLabel_Async(mail_cache::sysLabelId(mail_cache::SysLabel::UNREAD), d, set_it, true);
};

bool mail_cache::GmailCacheRoutes::setImportant(mail_cache::MessageData* d, bool set_it)
{
    TRY_LABEL_MODIFY(setImportant_Async, d, set_it);
};

GoogleTask<messages::MessageResource>* mail_cache::GmailCacheRoutes::setImportant_Async(mail_cache::MessageData* d, bool set_it)
{
    return setLabel_Async(mail_cache::sysLabelId(mail_cache::SysLabel::IMPORTANT), d, set_it, true);
};


GoogleTask<messages::MessageResource>* mail_cache::GmailCacheRoutes::setLabel_Async(QString label_id,
    mail_cache::MessageData* d,
    bool label_on,
    bool system_label)
{
    ensureCache();
    int accId = -1;
    auto storage = m_GMailCache->sqlite_storage();
    if (storage) {
        accId = d->accountId();
        if (accId == -1) {
            qWarning() << "ERROR. Invalid account Id" << d->id();
        }
        else {
            mail_cache::LabelData* lbl = storage->ensureLabel(accId, label_id, system_label);
            if (!lbl) {
                qWarning() << "failed to create label" << label_id;
            }
            else {
                if (label_on) {
                    d->m_labels |= lbl->labelMask();
                }
                else {
                    d->m_labels &= ~(lbl->labelMask());
                }
            }
        }
    }

    QString userId = storage->findUser(d->accountId());
    if (userId.isEmpty()) {
        qWarning() << "ERROR. Failed to locate userId" << d->id() << d->accountId();
    }

    QString msg_id = d->id();
    gmail::ModifyMessageArg arg(userId, msg_id);
    std::list <QString> labels;
    labels.push_back(label_id);
    if (label_on)
    {
        arg.setAddlabels(labels);
    }
    else
    {
        arg.setRemovelabels(labels);
    }

   // messages::MessagesRoutes* msg = getMessages();
    GoogleTask<messages::MessageResource>* t = m_gmail_routes.getMessages()->modify_Async(arg);
    QObject::connect(t, &EndpointRunnable::finished, [=]()
    {
        if (m_GMailCache &&
            m_GMailCache->hasLocalPersistentStorate() &&
            accId != -1)
        {
            auto storage = m_GMailCache->sqlite_storage();
            if (storage) {
                storage->update_message_labels_db(accId, msg_id, d->labelsBitMap());
            }
        }
    });
    return t;
};

bool mail_cache::GmailCacheRoutes::messageHasLabel(mail_cache::MessageData* d, QString label_id)const
{
    bool rv = false;
    ensureCache();
    auto storage = m_GMailCache->sqlite_storage();
    if (storage) {
        mail_cache::LabelData* lb = storage->findLabel(label_id);
        if (lb) {
            rv = d->inLabelFilter(lb->labelMask());
        }
    }
    return rv;
};

#ifdef API_QT_AUTOTEST
void mail_cache::GmailCacheRoutes::autotestParDBLoad(EDataState state, const std::list<QString>& id_list)
{
    mail_cache::GMailCacheQueryTask* r = getCacheMessages_Async(ApiAutotest::INSTANCE().userId(), state, id_list);
    mail_cache::data_list_uptr res = r->waitForResultAndRelease();
    ApiAutotest::INSTANCE() << QString("loaded/cached %1 messages, mem_cache-hit: %2, db-cache-hit: %3")
        .arg(res->result_list.size())
        .arg(r->mem_cache_hit_count())
        .arg(r->db_cache_hit_count());
};

void mail_cache::GmailCacheRoutes::runAutotest()
{
#define AUTOTEST_SIZE 100
#define AUTOTEST_GENERATE_BODY //autotestParDBLoad(EDataState::body, id_list);
#define AUTOTEST_GENERATE_SNIPPET autotestParDBLoad(EDataState::snippet, id_list);

    ApiAutotest::INSTANCE() << "start-mail-test";
    ApiAutotest::INSTANCE() << "1";
    ApiAutotest::INSTANCE() << "2";
    ApiAutotest::INSTANCE() << "3";
    ApiAutotest::INSTANCE() << "4";

    auto storage = m_GMailCache->sqlite_storage();

    auto m = gmail::SendMimeMessageArg::EXAMPLE(0, 0);
    QString rfc822_sample = m->toRfc822();
    ApiAutotest::INSTANCE() << "==== rcf822 ====";
    ApiAutotest::INSTANCE() << rfc822_sample;
    QJsonObject js;
    m->toJson(js);
    ApiAutotest::INSTANCE() << js["raw"].toString();
    ApiAutotest::INSTANCE() << "";

    std::list<QString> id_list;
    for (int i = 1; i <= AUTOTEST_SIZE; i++)
    {
        QString id = QString("idR_%1").arg(i);
        id_list.push_back(id);
    };

    ApiAutotest::INSTANCE().enableRequestLog(false);
    ApiAutotest::INSTANCE().enableAttachmentDataGeneration(false);
    ApiAutotest::INSTANCE().enableProgressEmulation(false);
    AUTOTEST_GENERATE_SNIPPET;
    AUTOTEST_GENERATE_SNIPPET;
    AUTOTEST_GENERATE_BODY;

    m_GMailCache->mem_clear();
    autotestParDBLoad(EDataState::snippet, id_list);

    std::function<void(void)>deleteFirst10 = [=]()
    {
        std::set<QString> set2remove;
        for (auto j = id_list.begin(); j != id_list.end(); j++)
        {
            set2remove.insert(*j);
            if (set2remove.size() >= 10)
                break;
        }

        m_GMailCache->persistent_clear(storage->m_accId, set2remove);
    };

    deleteFirst10();
    AUTOTEST_GENERATE_SNIPPET;
    AUTOTEST_GENERATE_BODY;

    deleteFirst10();
    AUTOTEST_GENERATE_BODY;
    AUTOTEST_GENERATE_SNIPPET;
    AUTOTEST_GENERATE_SNIPPET;
    AUTOTEST_GENERATE_BODY;

    int idx = 1;
    bool print_cache = true;
    if (print_cache)
    {
        using MSG_LIST = std::list<std::shared_ptr<mail_cache::MessageData>>;
        mail_cache::data_list_uptr lst = getCacheMessages(-1);
        for (auto& i : lst->result_list)
        {
            mail_cache::MessageData* m = i.get();
            QString s = QString("%1. %2 %3").arg(idx).arg(m->id()).arg(m->snippet());
            ApiAutotest::INSTANCE() << s;
            idx++;
            if (idx > 20)
            {
                ApiAutotest::INSTANCE() << "...";
                break;
            }
        }
        QString s = QString("Total messages %1").arg(lst->result_list.size());
        ApiAutotest::INSTANCE() << s;
    }
    //*** check latest emails
    bool check_email = true;
    if (check_email)
    {
        idx = 1;
        mail_cache::data_list_uptr lst2 = getNextCacheMessages(ApiAutotest::INSTANCE().userId());
        for (auto& i : lst2->result_list)
        {
            mail_cache::MessageData* m = i.get();
            QString s = QString("%1. %2 %3").arg(idx).arg(m->id()).arg(m->snippet());
            ApiAutotest::INSTANCE() << s;
            idx++;
        }
        QString s = QString("Next(new) messages %1").arg(lst2->result_list.size());
        ApiAutotest::INSTANCE() << s;
    }

    bool randomize_labels = true;
    if (randomize_labels)
    {
        std::list<mail_cache::LabelData*> labels = getLoadedLabels();

        if (labels.size() > 0)
        {
            std::set<QString> setG;
            setG.insert("INBOX");
            setG.insert("SENT");
            setG.insert("STARRED");
            setG.insert("IMPORTANT");
            setG.insert("DRAFT");
            setG.insert("TRASH");
            std::list<mail_cache::LabelData*> imperative_groups = getLoadedLabels(&setG);


            auto l_iterator = labels.begin();
            auto g_iterator = imperative_groups.begin();
            uint64_t lmask = (*l_iterator)->labelMask();
            uint64_t gmask = (*g_iterator)->labelMask();
            mail_cache::data_list_uptr lst = getCacheMessages(-1);
            int counter = 0;
            int totalNumber = lst->result_list.size();
            int imperative_group_size = totalNumber / imperative_groups.size();
            if (imperative_group_size == 0)
                imperative_group_size = 1;
            qDebug() << QString("Resetting labels: %1 on %2 messages, with imperative group size %3")
                .arg(labels.size())
                .arg(totalNumber)
                .arg(imperative_group_size);
            for (auto& i : lst->result_list)
            {
                mail_cache::MessageData* m = i.get();
                storage->update_message_labels_db(storage->m_accId, m->id(), lmask | gmask);

                counter++;
                l_iterator++; if (l_iterator == labels.end())l_iterator = labels.begin();
                lmask = (*l_iterator)->labelMask();

                if (counter > imperative_group_size) {
                    counter = 0;

                    g_iterator++; if (g_iterator == imperative_groups.end())g_iterator = imperative_groups.begin();
                    gmask = (*g_iterator)->labelMask();

                    qDebug() << QString("Next Label group: %1/%2")
                        .arg((*g_iterator)->labelName())
                        .arg(gmask);
                }
            }
        }
    }
    ApiAutotest::INSTANCE().enableProgressEmulation(true);
    ApiAutotest::INSTANCE().enableAttachmentDataGeneration(true);
    ApiAutotest::INSTANCE().enableRequestLog(true);

#undef AUTOTEST_GENERATE_BODY
#undef AUTOTEST_SIZE
};

#endif //API_QT_AUTOTEST
