#include <QDir>
#include <QFile>
#include "GmailCacheRoutes.h"
#include "GmailRoutes.h"
#include "Endpoint.h"
#include "gcontact/GcontactRoutes.h"
#include "gcontact/GcontactCache.h"

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
    m_GMsgCache.reset(new mail_cache::GMailCache(endpoint));
    m_GThreadCache.reset(new mail_cache::GThreadCache(endpoint));
};

RESULT_LIST<messages::MessageResource>&& mail_cache::GmailCacheRoutes::getUserBatchMessages(EDataState f,
                                                                                            const std::list<QString>& id_list)
{
    return getUserBatchMessages_Async(f, id_list)->waitForResultAndRelease();
};

mail_cache::GMailCacheQueryTask* mail_cache::GmailCacheRoutes::newMessageResultFetcher(EDataState state)
{
    auto rfetcher = new mail_cache::GMailCacheQueryTask(
        state,
        m_endpoint,
        *this,
        m_GMsgCache);
    return rfetcher;
};

mail_cache::GThreadCacheQueryTask* mail_cache::GmailCacheRoutes::newThreadResultFetcher()
{
	auto rfetcher = new mail_cache::GThreadCacheQueryTask(
		*this,
		m_GThreadCache);
	return rfetcher;
};



ConcurrentValueRunner<QString,
                mail_cache::MessagesReceiver,
                messages::MessageResource>* mail_cache::GmailCacheRoutes::getUserBatchMessages_Async(EDataState f, 
                                                                                         const std::list<QString>& id_list)
{
    std::unique_ptr<mail_cache::MessagesReceiver> mr(new mail_cache::MessagesReceiver(m_gmail_routes, f));
    
    ConcurrentValueRunner<QString,
                    mail_cache::MessagesReceiver,
                    messages::MessageResource>* r = new ConcurrentValueRunner<QString,
                                                                        mail_cache::MessagesReceiver,
                                                                        messages::MessageResource>(id_list, std::move(mr), m_endpoint);
    r->run();
    return r;
};

mail_cache::mdata_result mail_cache::GmailCacheRoutes::getCacheMessages(EDataState state, const std::list<QString>& id_list)
{
    return getCacheMessages_Async(state, id_list)->waitForResultAndRelease();
};


mail_cache::GMailCacheQueryTask* mail_cache::GmailCacheRoutes::getCacheMessages_Async(EDataState state,
    const std::list<QString>& id_list,
    mail_cache::GMailCacheQueryTask* rfetcher /*= nullptr*/)
{
    if (!rfetcher)
    {
        rfetcher = newMessageResultFetcher(state);
    }
    m_GMsgCache->query_Async(state, id_list, rfetcher);
    return rfetcher;
};

mail_cache::mdata_result mail_cache::GmailCacheRoutes::getCacheMessages(int numberOfMessages, uint64_t labelFilter)
{
    return m_GMsgCache->topCacheData(numberOfMessages, labelFilter);
};

mail_cache::tdata_result mail_cache::GmailCacheRoutes::getCacheThreads(int numberOfThreads, uint64_t labelFilter)
{
    return m_GThreadCache->topCacheData(numberOfThreads, labelFilter);
};

mail_cache::GMailCacheQueryTask* mail_cache::GmailCacheRoutes::getNextCacheMessages_Async(
    int messagesCount /*= 40*/,
    QString pageToken /*= ""*/,
    QStringList* labels /*= nullptr*/,
    QString q /*= ""*/)
{
    auto rfetcher = newMessageResultFetcher(EDataState::snippet);

    gmail::ListArg listArg;
    listArg.setMaxResults(messagesCount);
    listArg.setPageToken(pageToken);
    listArg.setQ(q);
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
        //        rfetcher->setNextPageToken(mlist->nextpagetoken());
        rfetcher->m_nextPageToken = mlist->nextpagetoken();
        getCacheMessages_Async(EDataState::snippet, id_list, rfetcher);
    },
        [=](std::unique_ptr<GoogleException> ex)
    {
        rfetcher->failed_callback(std::move(ex));
    });

    return rfetcher;
};

mail_cache::mdata_result mail_cache::GmailCacheRoutes::getNextCacheMessages(
    int messagesCount /*= 40*/,
    QString pageToken /*= ""*/,
    QStringList* labels /*= nullptr*/,
    QString q /*= ""*/)
{
    return getNextCacheMessages_Async(
        messagesCount,
        pageToken,        
        labels,
        q)->waitForResultAndRelease();
};

GoogleVoidTask* mail_cache::GmailCacheRoutes::trashCacheMessage_Async(QString userId, QString msg_id)
{
    GoogleVoidTask* rv = m_endpoint.produceVoidTask();
    googleQt::gmail::TrashMessageArg arg(userId, msg_id);
    m_gmail_routes.getMessages()->trash_Async(arg)->then([=]()
    {
        //clean up cache
		if (m_lite_storage) {
			std::set<QString> set2remove;
			set2remove.insert(msg_id);
			m_GMsgCache->persistent_clear(set2remove);
			m_lite_storage->deleteAttachmentsFromDb(msg_id);
		}
        rv->completed_callback();
    },
        [=](std::unique_ptr<GoogleException> ex) {
        rv->failed_callback(std::move(ex));
    });
    return rv;
};


ConcurrentValueRunner<QString, 
	mail_cache::ThreadsReceiver, 
	threads::ThreadResource>* mail_cache::GmailCacheRoutes::getUserBatchThreads_Async(const std::list<QString>& id_list)
{
	std::unique_ptr<mail_cache::ThreadsReceiver> tr(new mail_cache::ThreadsReceiver(m_gmail_routes));
	auto r = new ConcurrentValueRunner<QString,
							mail_cache::ThreadsReceiver,
							threads::ThreadResource>(id_list, std::move(tr), m_endpoint);
	r->run();
	return r;
};

mail_cache::tdata_result mail_cache::GmailCacheRoutes::getNextCacheThreads(
	int messagesCount /*= 40*/,
	QString pageToken /*= ""*/,
	QStringList* labels /*= nullptr*/,
	QString q /*= ""*/) 
{
	return getNextCacheThreads_Async(
		messagesCount,
		pageToken,
		labels,
		q)->waitForResultAndRelease();
};


mail_cache::GThreadCacheQueryTask* mail_cache::GmailCacheRoutes::getNextCacheThreads_Async(
	int threadsCount /*= 40*/,
	QString pageToken /*= ""*/,
	QStringList* labels /*= nullptr*/,
	QString q /*= ""*/)
{
	auto rfetcher = newThreadResultFetcher();
	gmail::ListArg listArg;
	listArg.setMaxResults(threadsCount);
	listArg.setPageToken(pageToken);
	listArg.setQ(q);
	if (labels)
	{
		listArg.labels() = *labels;
	}

	///this will return list of thread Ids with HistoryId
	m_gmail_routes.getThreads()->list_Async(listArg)->then([=](std::unique_ptr<threads::ThreadListRes> tlist)
	{
		std::list<HistId> id_list;
		for (auto& m : tlist->threads())
		{
			HistId h;
			h.id = m.id();
			h.hid = m.historyid();
			id_list.push_back(h);
		}
        rfetcher->m_nextPageToken = tlist->nextpagetoken();
        getCacheThreads_Async(id_list, rfetcher);
	},
		[=](std::unique_ptr<GoogleException> ex)
	{
		rfetcher->failed_callback(std::move(ex));
	});

	return rfetcher;
};



mail_cache::GThreadCacheQueryTask* mail_cache::GmailCacheRoutes::getCacheThreads_Async(
	const std::list<HistId>& id_list,
	mail_cache::GThreadCacheQueryTask* rfetcher /*= nullptr*/)
{
	if (!rfetcher)
	{
		rfetcher = newThreadResultFetcher();
	}
	m_GThreadCache->queryWithHistory_Async(id_list, rfetcher);
	return rfetcher;
};


bool mail_cache::GmailCacheRoutes::setupSQLiteCache(QString dbPath,
    QString downloadPath,
    QString contactCachePath,
    QString dbName /*= "googleqt"*/,
    QString dbprefix /*= "api"*/)
{
    EXPECT_STRING_VAL(m_endpoint.client()->userId(), "UserId");
    EXPECT_STRING_VAL(dbPath, "DB path");
    EXPECT_STRING_VAL(downloadPath, "Download path");
    EXPECT_STRING_VAL(contactCachePath, "contactCachePath path");
    EXPECT_STRING_VAL(dbName, "DB name");
    EXPECT_STRING_VAL(dbprefix, "DB prefix");

    gcontact::contact_cache_ptr cc = m_endpoint.client()->gcontact()->cacheRoutes()->cache();

	m_lite_storage.reset(new mail_cache::GMailSQLiteStorage(m_GMsgCache, m_GThreadCache, cc));
    cc->attachSQLStorage(m_lite_storage);

    if (!m_lite_storage->init_db(dbPath, downloadPath, contactCachePath, dbName, dbprefix))
    {
		m_lite_storage->close_db();
        m_GMsgCache->invalidate();
        qWarning() << "Failed to initialize SQLite storage" << dbPath << dbName << dbprefix;
        return false;
    }

    return true;
};

bool mail_cache::GmailCacheRoutes::resetSQLiteCache()
{
    if (!m_GMsgCache) {
        qWarning("GMail cache is not defined. Please call setupSQLiteCache first.");
        return false;
    }

    EXPECT_STRING_VAL(m_endpoint.client()->userId(), "UserId");

    if (!m_lite_storage) {
        qWarning("DB storage of GMail cache is not defined. Please call setupSQLiteCache first.");
        return false;
    }
    QString dbPath = m_lite_storage->m_dbPath;
    QString downloadPath = m_lite_storage->m_downloadDir;
    QString contactCacheDir = m_lite_storage->m_contactCacheDir;
    QString dbName = m_lite_storage->m_dbName;
    QString dbprefix = m_lite_storage->m_metaPrefix;

    EXPECT_STRING_VAL(dbPath, "DB path");
    EXPECT_STRING_VAL(downloadPath, "Download path");
    EXPECT_STRING_VAL(contactCacheDir, "contactCacheDir path");
    EXPECT_STRING_VAL(dbName, "DB name");
    EXPECT_STRING_VAL(dbprefix, "DB prefix");

	if (m_lite_storage) {
		m_lite_storage->close_db();
	}

    if (m_GMsgCache) {
        m_GMsgCache->invalidate();
    }
    m_GMsgCache.reset(new mail_cache::GMailCache(m_endpoint));

    return setupSQLiteCache(dbPath, downloadPath, contactCacheDir, dbName, dbprefix);
};

GoogleVoidTask* mail_cache::GmailCacheRoutes::refreshLabels_Async()
{
    GoogleVoidTask* rv = m_endpoint.produceVoidTask();

    googleQt::GoogleTask<labels::LabelsResultList>* t = m_gmail_routes.getLabels()->list_Async();
    t->then([=](std::unique_ptr<labels::LabelsResultList> lst)
    {
        if (m_lite_storage) {
            for (auto& lbl : lst->labels())
            {
                QString label_id = lbl.id();
                auto db_lbl = m_lite_storage->findLabel(label_id);
                if (db_lbl)
                {
					m_lite_storage->updateDbLabel(lbl);
                }
                else
                {
					m_lite_storage->insertDbLabel(lbl);
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
    GoogleVoidTask* rv = m_endpoint.produceVoidTask();

    if (a->status() == mail_cache::AttachmentData::statusDownloadInProcess) {
        qWarning() << "attachment download already in progress " << m->id() << a->attachmentId();
    }

	if (!m_lite_storage) {
        qWarning() << "ERROR. Expected storage object.";
        rv->completed_callback();
        return rv;
    }

    QString userId = m_lite_storage->findUser(m->accountId());
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
            while (idx++ < 1000) {
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
            if (m_lite_storage) {
				m_lite_storage->update_attachment_local_file_db(m, a, fi.fileName());
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
    if (!m_lite_storage) {
        std::list<mail_cache::LabelData*> on_error;
        return on_error;
    }
    return m_lite_storage->getLabelsInSet(in_optional_idset);
};

std::list<mail_cache::LabelData*> mail_cache::GmailCacheRoutes::getMessageLabels(mail_cache::MessageData* d)
{
    if (!m_lite_storage) {
        std::list<mail_cache::LabelData*> on_error;
        return on_error;
    }
    return m_lite_storage->unpackLabels(d->labelsBitMap());
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
    int accId = -1;
    if (m_lite_storage) {
        accId = d->accountId();
        if (accId == -1) {
            qWarning() << "ERROR. Invalid account Id" << d->id();
        }
        else {
            mail_cache::LabelData* lbl = m_lite_storage->ensureLabel(accId, label_id, system_label);
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

    QString userId = m_lite_storage->findUser(d->accountId());
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
        if (m_GMsgCache &&
			m_lite_storage &&
            accId != -1)
        {
			m_lite_storage->update_message_labels_db(accId, msg_id, d->labelsBitMap());
        }
    });
    return t;
};

bool mail_cache::GmailCacheRoutes::messageHasLabel(mail_cache::MessageData* d, QString label_id)const
{
    bool rv = false;
    if (m_lite_storage) {
        mail_cache::LabelData* lb = m_lite_storage->findLabel(label_id);
        if (lb) {
            rv = d->hasLabel(lb->labelMask());
        }
    }
    return rv;
};

#ifdef API_QT_AUTOTEST
void mail_cache::GmailCacheRoutes::autotestThreadDBLoad(const std::list<HistId>& id_list)
{
    auto* r = getCacheThreads_Async(id_list);
    auto res = r->waitForResultAndRelease();
    ApiAutotest::INSTANCE() << QString("loaded/cached %1 threads, mem_cache-hit: %2, db-cache-hit: %3")
        .arg(res->result_list.size())
        .arg(r->mem_cache_hit_count())
        .arg(r->db_cache_hit_count());
};

void mail_cache::GmailCacheRoutes::runAutotest()
{
//#define AUTOTEST_SIZE 100
#define AUTOTEST_SIZE 10
#define AUTOTEST_GENERATE_SNIPPET autotestThreadDBLoad(id_list);

    ApiAutotest::INSTANCE() << "start-mail-test";
    ApiAutotest::INSTANCE() << "1";
    ApiAutotest::INSTANCE() << "2";
    ApiAutotest::INSTANCE() << "3";
    ApiAutotest::INSTANCE() << "4";

    auto m = gmail::SendMimeMessageArg::EXAMPLE(0, 0);
    QString rfc822_sample = m->toRfc822();
    ApiAutotest::INSTANCE() << "==== rcf822 ====";
    ApiAutotest::INSTANCE() << rfc822_sample;
    QJsonObject js;
    m->toJson(js);
    ApiAutotest::INSTANCE() << js["raw"].toString();
    ApiAutotest::INSTANCE() << "";

    std::list<HistId> id_list;
    for (int i = 1; i <= AUTOTEST_SIZE; i++)
    {
		HistId hid;
		hid.id = QString("idR_%1").arg(i);
		hid.hid = 2 * i;
        id_list.push_back(hid);
    };

    ApiAutotest::INSTANCE().enableRequestLog(false);
    ApiAutotest::INSTANCE().enableAttachmentDataGeneration(false);
    ApiAutotest::INSTANCE().enableProgressEmulation(false);
    AUTOTEST_GENERATE_SNIPPET;
    AUTOTEST_GENERATE_SNIPPET;

	m_GMsgCache->mem_clear();
	AUTOTEST_GENERATE_SNIPPET;

    std::function<void(void)>deleteFirst10 = [=]()
    {
        std::set<QString> set2remove;
        for (auto j = id_list.begin(); j != id_list.end(); j++)
        {
            set2remove.insert(j->id);
            if (set2remove.size() >= 10)
                break;
        }

		m_GThreadCache->persistent_clear(set2remove);
    };

    deleteFirst10();
    AUTOTEST_GENERATE_SNIPPET;

    deleteFirst10();
    AUTOTEST_GENERATE_SNIPPET;
    AUTOTEST_GENERATE_SNIPPET;

    int idx = 1;
    bool print_cache = true;
    if (print_cache)
    {
        using MSG_LIST = std::list<std::shared_ptr<mail_cache::MessageData>>;
        mail_cache::mdata_result lst = getCacheMessages(-1);
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

	qDebug() << "check latest emails";
    bool check_email = true;
    if (check_email)
    {
        idx = 1;
        mail_cache::mdata_result lst2 = getNextCacheMessages();
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
            mail_cache::mdata_result lst = getCacheMessages(-1);
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
				m_lite_storage->update_message_labels_db(m_lite_storage->m_accId, m->id(), lmask | gmask);

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


	qDebug() << "check latest threads";
	bool check_thread = true;
	if (check_thread)
	{
		idx = 1;
		auto lst2 = getNextCacheThreads();
		for (auto& i : lst2->result_list)
		{
			auto* t = i.get();
			QString s = QString("%1. %2 %3 %4")
				.arg(idx)
				.arg(t->id())
				.arg(t->historyId())
				.arg(t->snippet());
			ApiAutotest::INSTANCE() << s;
			idx++;
		}
		QString s = QString("Next(new) threads %1").arg(lst2->result_list.size());
		ApiAutotest::INSTANCE() << s;
	}

    ApiAutotest::INSTANCE().enableProgressEmulation(true);
    ApiAutotest::INSTANCE().enableAttachmentDataGeneration(true);
    ApiAutotest::INSTANCE().enableRequestLog(true);

#undef AUTOTEST_SIZE
};

#endif //API_QT_AUTOTEST
