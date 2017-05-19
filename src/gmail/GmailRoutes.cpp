#include <QDir>
#include <QFile>
#include "GmailRoutes.h"
#include "Endpoint.h"

using namespace googleQt;

GmailRoutes::GmailRoutes(Endpoint* e):m_endpoint(e)
{

};

messages::MessagesRoutes* GmailRoutes::getMessages()
{
    if (!m_MessagesRoutes) {
        m_MessagesRoutes.reset(new messages::MessagesRoutes(m_endpoint));
    }
    return m_MessagesRoutes.get();
};

attachments::AttachmentsRoutes* GmailRoutes::getAttachments() 
{
	if (!m_AttachmentsRoutes) {
		m_AttachmentsRoutes.reset(new attachments::AttachmentsRoutes(m_endpoint));
	}
	return m_AttachmentsRoutes.get();
};

labels::LabelsRoutes* GmailRoutes::getLabels()
{
    if(!m_LabelsRoutes){
        m_LabelsRoutes.reset(new labels::LabelsRoutes(m_endpoint));
    }
    return m_LabelsRoutes.get();
};

users::UsersRoutes* GmailRoutes::getUsers()
{
    if(!m_UsersRoutes){
        m_UsersRoutes.reset(new users::UsersRoutes(m_endpoint));
    }
    return m_UsersRoutes.get();
};

threads::ThreadsRoutes* GmailRoutes::getThreads() 
{
    if (!m_ThreadsRoutes) {
        m_ThreadsRoutes.reset(new threads::ThreadsRoutes(m_endpoint));
    }
    return m_ThreadsRoutes.get();
};

history::HistoryRoutes* GmailRoutes::getHistory() 
{
    if (!m_HistoryRoutes) {
        m_HistoryRoutes.reset(new history::HistoryRoutes(m_endpoint));
    }
    return m_HistoryRoutes.get();
};

drafts::DraftsRoutes* GmailRoutes::getDrafts()
{
    if (!m_DraftsRoutes) {
        m_DraftsRoutes.reset(new drafts::DraftsRoutes(m_endpoint));
    }
    return m_DraftsRoutes.get();
};



std::unique_ptr<UserBatchResult<QString, 
                            messages::MessageResource>> GmailRoutes::getUserBatchMessages(EDataState f,
                                                                                      const std::list<QString>& id_list)
{
    return getUserBatchMessages_Async(f, id_list)->waitForResultAndRelease();
};

UserBatchRunner<QString,
            mail_cache::MessagesReceiver,
            messages::MessageResource>* GmailRoutes::getUserBatchMessages_Async(EDataState f, const std::list<QString>& id_list)
{
    mail_cache::MessagesReceiver* mr = new mail_cache::MessagesReceiver(*this, f);
    
    UserBatchRunner<QString,
                mail_cache::MessagesReceiver,
                messages::MessageResource>* r = new UserBatchRunner<QString,
                                                                mail_cache::MessagesReceiver,
                                                                messages::MessageResource>(id_list, mr, *m_endpoint);
    r->run();
    return r;
};

void GmailRoutes::ensureCache()const
{
    if (!m_GMailCache) {
        m_GMailCache.reset(new mail_cache::GMailCache(*m_endpoint));
    }
};

mail_cache::GMailCacheQueryTask* GmailRoutes::getNextCacheMessages_Async(int messagesCount /*= 40*/, 
    QString pageToken /*= ""*/, 
	QStringList* labels /*= nullptr*/)
{
    mail_cache::GMailCacheQueryTask* rfetcher = newResultFetcher(EDataState::snippet);

    gmail::ListArg listArg;
    listArg.setMaxResults(messagesCount);
    listArg.setPageToken(pageToken);
	if (labels) 
	{
		listArg.labels() = *labels;
	}	
	
	getMessages()->list_Async(listArg)->then([=](std::unique_ptr<messages::MessageListRes> mlist)
	{
		std::list<QString> id_list;
		for (auto& m : mlist->messages())
		{
			id_list.push_back(m.id());
		}
		rfetcher->setNextPageToken(mlist->nextpagetoken());
		getCacheMessages_Async(EDataState::snippet, id_list, rfetcher);
	},
		[=](std::unique_ptr<GoogleException> ex)
	{
		rfetcher->failed_callback(std::move(ex));
	});		

    return rfetcher;
};

mail_cache::data_list_uptr GmailRoutes::getNextCacheMessages(int messagesCount /*= 40*/,
    QString pageToken /*= ""*/,
	QStringList* labels /*= nullptr*/)
{
    return getNextCacheMessages_Async(messagesCount, 
		pageToken, 
		labels)->waitForResultAndRelease();
};

mail_cache::data_list_uptr GmailRoutes::getCacheMessages(EDataState state, const std::list<QString>& id_list)
{
    return getCacheMessages_Async(state, id_list)->waitForResultAndRelease();
};

mail_cache::GMailCacheQueryTask* GmailRoutes::newResultFetcher(EDataState state)
{
    mail_cache::GMailCacheQueryTask* rfetcher = new mail_cache::GMailCacheQueryTask(state, *m_endpoint, *this, m_GMailCache.get());
    return rfetcher;
};

mail_cache::GMailCacheQueryTask* GmailRoutes::getCacheMessages_Async(EDataState state,
                                                                       const std::list<QString>& id_list,
                                                                        mail_cache::GMailCacheQueryTask* rfetcher /*= nullptr*/)
{
    ensureCache();
    if (!rfetcher)
    {
        rfetcher = newResultFetcher(state);
    }    
    m_GMailCache->query_Async(state, id_list, rfetcher);
    return rfetcher;
};

mail_cache::data_list_uptr GmailRoutes::getCacheMessages(int numberOfMessages, uint64_t labelFilter)
{
    ensureCache();
    mail_cache::GMailCacheQueryTask* rfetcher = newResultFetcher(EDataState::snippet);
    m_GMailCache->topCacheData(rfetcher, numberOfMessages, labelFilter);
    return rfetcher->waitForResultAndRelease();
};

bool GmailRoutes::trashCacheMessage(QString msg_id)
{
	ensureCache();
	googleQt::gmail::TrashMessageArg arg(msg_id);
	getMessages()->trash_Async(arg)->then([=]()
	{
		//clean up cache
		std::set<QString> set2remove;
		set2remove.insert(msg_id);
		m_GMailCache->persistent_clear(set2remove);
		auto storage = m_GMailCache->sqlite_storage();
		storage->deleteAttachmentsFromDb(msg_id);		
	});
	return true;
};

bool GmailRoutes::setupSQLiteCache(QString dbPath, 
	QString downloadPath, 
	QString dbName /*= "googleqt"*/, 
	QString dbprefix /*= "api"*/)
{
    ensureCache();

    if (m_GMailCache->hasLocalPersistentStorate()) 
    {
        return true;
    }

    std::unique_ptr<mail_cache::GMailSQLiteStorage> st(new mail_cache::GMailSQLiteStorage(m_GMailCache.get()));
    if (!st->init_db(dbPath, downloadPath, dbName, dbprefix))
    {
        qWarning() << "Failed to initialize SQLite storage" << dbPath << dbName << dbprefix;
        return false;
    }

    m_GMailCache->setupLocalStorage(st.release());

	refreshLabels();

    return true;
};

GoogleVoidTask* GmailRoutes::refreshLabels_Async()
{
	ensureCache();

	auto storage = m_GMailCache->sqlite_storage();

	GoogleVoidTask* rv = m_endpoint->produceVoidTask();

	googleQt::GoogleTask<labels::LabelsResultList>* t = getLabels()->list_Async();
	t->then([=](std::unique_ptr<labels::LabelsResultList> lst)
	{
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
		rv->completed_callback();
	});
	return rv;
};

GoogleVoidTask* GmailRoutes::downloadAttachment_Async(googleQt::mail_cache::msg_ptr m, 
	googleQt::mail_cache::att_ptr a, 
	QString destinationFolder)
{
	ensureCache();
	GoogleVoidTask* rv = m_endpoint->produceVoidTask();

	if (a->status() == mail_cache::AttachmentData::statusDownloadInProcess) {
		qWarning() << "attachment download already in progress " << m->id() << a->attachmentId();
	}

	a->m_status = mail_cache::AttachmentData::statusDownloadInProcess;
	gmail::AttachmentIdArg arg(m->id(), a->attachmentId());
	auto t = getAttachments()->get_Async(arg);
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
			storage->update_attachment_local_file_db(m, a, fi.fileName());
			emit attachmentsDownloaded(m, a);
		}
		else {
			qWarning() << "Failed to create attachment file" << destFile;
		}
		rv->completed_callback();
	});
	return rv;
};

void GmailRoutes::refreshLabels() 
{
	refreshLabels_Async()->waitForResultAndRelease();
};

std::list<mail_cache::LabelData*> GmailRoutes::getLoadedLabels(std::set<QString>* in_optional_idset)
{
	ensureCache();
	auto storage = m_GMailCache->sqlite_storage();
	return storage->getLabelsInSet(in_optional_idset);
};

std::list<mail_cache::LabelData*> GmailRoutes::getMessageLabels(mail_cache::MessageData* d)
{
	ensureCache();
	auto storage = m_GMailCache->sqlite_storage();
	return storage->unpackLabels(d->labelsBitMap());
};

#define TRY_LABEL_MODIFY(F, M, S) bool rv = false;\
    try                                           \
        {                                                               \
    std::unique_ptr<messages::MessageResource> m = F(M, S)->waitForResultAndRelease(); \
    rv = (!m->id().isEmpty());                                          \
        }                                                               \
    catch (GoogleException& e)                                          \
        {                                                               \
            qWarning() << "setLabel Exception: " << e.what();           \
        }                                                               \
    return rv;                                                          \



bool GmailRoutes::setStarred(mail_cache::MessageData* d, bool set_it)
{
    TRY_LABEL_MODIFY(setStarred_Async, d, set_it);
};

GoogleTask<messages::MessageResource>* GmailRoutes::setStarred_Async(mail_cache::MessageData* d, bool set_it)
{
    return setLabel_Async(mail_cache::sysLabelId(mail_cache::SysLabel::STARRED), d, set_it, true);
};

bool GmailRoutes::setUread(mail_cache::MessageData* d, bool set_it)
{
    TRY_LABEL_MODIFY(setUread_Async, d, set_it);
};

GoogleTask<messages::MessageResource>* GmailRoutes::setUread_Async(mail_cache::MessageData* d, bool set_it)
{
    return setLabel_Async(mail_cache::sysLabelId(mail_cache::SysLabel::UNREAD), d, set_it, true);
};

bool GmailRoutes::setImportant(mail_cache::MessageData* d, bool set_it)
{
    TRY_LABEL_MODIFY(setImportant_Async, d, set_it);
};

GoogleTask<messages::MessageResource>* GmailRoutes::setImportant_Async(mail_cache::MessageData* d, bool set_it)
{
    return setLabel_Async(mail_cache::sysLabelId(mail_cache::SysLabel::IMPORTANT), d, set_it, true);
};


GoogleTask<messages::MessageResource>* GmailRoutes::setLabel_Async(QString label_id,
                                                                   mail_cache::MessageData* d,
                                                                   bool label_on,
                                                                   bool system_label)
{
	ensureCache();
	auto storage = m_GMailCache->sqlite_storage();

	mail_cache::LabelData* lbl = storage->ensureLabel(label_id, system_label);
	if (!lbl) {
		qWarning() << "failed to create label" << label_id;
	}
	else {
		if (label_on){
			d->m_labels |= lbl->labelMask();
		}
		else {
			d->m_labels &= ~(lbl->labelMask());
		}
	}

	QString msg_id = d->id();
	gmail::ModifyMessageArg arg(msg_id);
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

	messages::MessagesRoutes* msg = getMessages();
	GoogleTask<messages::MessageResource>* t = msg->modify_Async(arg);
	QObject::connect(t, &EndpointRunnable::finished, [=]()
	{
		if (m_GMailCache &&
			m_GMailCache->hasLocalPersistentStorate())
		{
			auto storage = m_GMailCache->sqlite_storage();
			storage->update_message_labels_db(msg_id, d->labelsBitMap());
		}
	});
	return t;
};

bool GmailRoutes::messageHasLabel(mail_cache::MessageData* d, QString label_id)const 
{
	bool rv = false;
	ensureCache();
	auto storage = m_GMailCache->sqlite_storage();
	mail_cache::LabelData* lb = storage->findLabel(label_id);
	if (lb) {
		rv = d->inLabelFilter(lb->labelMask());
	}
	return rv;
};

#ifdef API_QT_AUTOTEST
void GmailRoutes::autotestParLoad(EDataState state, const std::list<QString>& id_list)
{
    /// check parallel requests ///
    //ApiAutotest::INSTANCE() << QString("=== checking gmail/batch-load of %1 ids").arg(id_list.size());
    std::unique_ptr<UserBatchResult<QString, messages::MessageResource>> br = getUserBatchMessages(state, id_list);
    RESULT_LIST<messages::MessageResource*> res = br->results();
    ApiAutotest::INSTANCE() << QString("par-loaded (avoid cache) %1 snippets").arg(res.size());
};

void GmailRoutes::autotestParDBLoad(EDataState state, const std::list<QString>& id_list) 
{
    mail_cache::GMailCacheQueryTask* r = getCacheMessages_Async(state, id_list);
	mail_cache::data_list_uptr res = r->waitForResultAndRelease();
    ApiAutotest::INSTANCE() << QString("loaded/cached %1 messages, mem_cache-hit: %2, db-cache-hit: %3")
        .arg(res->result_list.size())
        .arg(r->mem_cache_hit_count())
        .arg(r->db_cache_hit_count());    
};

void GmailRoutes::autotest()
{
#define AUTOTEST_SIZE 1000
#define AUTOTEST_GENERATE_BODY //autotestParDBLoad(EDataState::body, id_list);
#define AUTOTEST_GENERATE_SNIPPET autotestParDBLoad(EDataState::snippet, id_list);

	ApiAutotest::INSTANCE() << "start-mail-test";
	ApiAutotest::INSTANCE() << "1";
	ApiAutotest::INSTANCE() << "2";
	ApiAutotest::INSTANCE() << "3";
	ApiAutotest::INSTANCE() << "4";

	/// check persistant cache update ///
	if (!setupSQLiteCache("gmail_autotest.sqlite", "downloads"))
	{
		ApiAutotest::INSTANCE() << "Failed to setup SQL database";
		return;
	};

	auto storage = cache()->sqlite_storage();

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

        m_GMailCache->persistent_clear(set2remove);
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
		mail_cache::data_list_uptr lst2 = getNextCacheMessages();
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
			int group_size = totalNumber / labels.size();
			qDebug() << QString("Resetting labels: %1 on %2 messages, with group size %3")
				.arg(labels.size())
				.arg(totalNumber)
				.arg(group_size);
			for (auto& i : lst->result_list)
			{
				mail_cache::MessageData* m = i.get();
				storage->update_message_labels_db(m->id(), lmask | gmask);

				counter++;
				l_iterator++; if (l_iterator == labels.end())l_iterator = labels.begin();
				lmask = (*l_iterator)->labelMask();

				if (counter > group_size) {
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
#endif
