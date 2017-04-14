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

mail_cache::GMailCacheQueryResult* GmailRoutes::getNextCacheMessages_Async(int messagesCount /*= 40*/, 
    QString pageToken /*= ""*/, 
	QStringList* labels /*= nullptr*/,
    std::set<QString>* msg2skip /*= nullptr*/)
{
    mail_cache::GMailCacheQueryResult* rfetcher = newResultFetcher(EDataState::snippet);

    gmail::ListArg listArg;
    listArg.setMaxResults(messagesCount);
    listArg.setPageToken(pageToken);
	if (labels) 
	{
		listArg.labels() = *labels;
	}

    std::function<void(std::unique_ptr<messages::MessageListRes>)> id_list_completed_callback =
        [=](std::unique_ptr<messages::MessageListRes> mlist)
    {
        std::list<QString> id_list;
        for (auto& m : mlist->messages())
        {
            bool skip_msg = (msg2skip != nullptr && msg2skip->find(m.id()) != msg2skip->end());
            if (!skip_msg)
            {
                id_list.push_back(m.id());
            }
        }
        getCacheMessages_Async(EDataState::snippet, id_list, rfetcher);
    };
    
    
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback =
        [=](std::unique_ptr<GoogleException> ex)
    {
        rfetcher->failed_callback(std::move(ex));
    };    

    getMessages()->list_AsyncCB(listArg, id_list_completed_callback, failed_callback);

    return rfetcher;
};

std::unique_ptr<mail_cache::MessagesList> GmailRoutes::getNextCacheMessages(int messagesCount /*= 40*/, 
    QString pageToken /*= ""*/,
	QStringList* labels /*= nullptr*/,
    std::set<QString>* msg2skip /*= nullptr*/)
{
    return getNextCacheMessages_Async(messagesCount, 
		pageToken, 
		labels, 
		msg2skip)->waitForSortedResultListAndRelease();
};

std::unique_ptr<mail_cache::MessagesList> GmailRoutes::getCacheMessages(EDataState state, const std::list<QString>& id_list)
{
    return getCacheMessages_Async(state, id_list)->waitForSortedResultListAndRelease();
};

mail_cache::GMailCacheQueryResult* GmailRoutes::newResultFetcher(EDataState state)
{
    mail_cache::GMailCacheQueryResult* rfetcher = new mail_cache::GMailCacheQueryResult(state, *m_endpoint, *this, m_GMailCache.get());
    return rfetcher;
};

mail_cache::GMailCacheQueryResult* GmailRoutes::getCacheMessages_Async(EDataState state,
                                                                       const std::list<QString>& id_list,
                                                                        mail_cache::GMailCacheQueryResult* rfetcher /*= nullptr*/)
{
    ensureCache();
    if (!rfetcher)
    {
        rfetcher = newResultFetcher(state);
    }    
    m_GMailCache->query_Async(state, id_list, rfetcher);
    return rfetcher;
};

std::unique_ptr<mail_cache::MessagesList> GmailRoutes::getCacheMessages(int numberOfMessages, uint64_t labelFilter)
{
    ensureCache();
    mail_cache::GMailCacheQueryResult* rfetcher = newResultFetcher(EDataState::snippet);
    m_GMailCache->cacheData(rfetcher, numberOfMessages, labelFilter);
    return rfetcher->waitForSortedResultListAndRelease();
};

bool GmailRoutes::setupSQLiteCache(QString dbPath, QString dbName /*= "googleqt"*/, QString dbprefix /*= "api"*/) 
{
    ensureCache();

    if (m_GMailCache->hasLocalPersistentStorate()) 
    {
        return true;
    }

    std::unique_ptr<mail_cache::GMailSQLiteStorage> st(new mail_cache::GMailSQLiteStorage(m_GMailCache.get()));
    if (!st->init(dbPath, dbName, dbprefix)) 
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

	std::function<void()> refresh_label_list = [=]()
	{
		if (t->isCompleted())
		{
			labels::LabelsResultList* lst = t->get();
			for (auto& lbl : lst->labels())
			{
				QString label_id = lbl.id();
				auto db_lbl = storage->findLabel(label_id);
				if (db_lbl)
				{
					storage->updateDbLabel(lbl.id(), lbl.name(), lbl.messagesunread());
				}
				else 
				{
					storage->insertDbLabel(lbl.id(), 
						lbl.name(), 
						lbl.type(),
						lbl.messagesunread());
				}
			}
		}

		t->deleteLater();
		rv->completed_callback();
	};

	if (t->isFinished())
	{
		refresh_label_list();
	}
	else
	{
		QObject::connect(t, &googleQt::GoogleTask<labels::LabelsResultList>::finished,
			refresh_label_list);
	}

	return rv;
};

void GmailRoutes::refreshLabels() 
{
	refreshLabels_Async()->waitForResultAndRelease();
};

std::list<mail_cache::LabelData*> GmailRoutes::getLoadedLabels() 
{
	ensureCache();
	auto storage = m_GMailCache->sqlite_storage();
	return storage->getAllLabels();
};

std::list<mail_cache::LabelData*> GmailRoutes::getMessageLabels(mail_cache::MessageData* d)
{
	ensureCache();
	auto storage = m_GMailCache->sqlite_storage();
	return storage->unpackLabels(d->labelsBitMap());
};

bool GmailRoutes::setStarred(mail_cache::MessageData* d, bool starred_on) 
{
    bool rv = false;
    try
    {    
        std::unique_ptr<messages::MessageResource> m = setStarred_Async(d, starred_on)->waitForResultAndRelease();
        rv = (!m->id().isEmpty());
        //        qDebug() << "STAR changed" << m->id();
    }
    catch (GoogleException& e)
    {
        qWarning() << "GdriveRoutes::fileExists Exception: " << e.what();
    }
    return rv;    
};

GoogleTask<messages::MessageResource>* GmailRoutes::setStarred_Async(mail_cache::MessageData* d, bool starred_on)
{
	ensureCache();
	auto storage = m_GMailCache->sqlite_storage();

	mail_cache::LabelData* lbl = storage->ensureSystemLabel("STARRED");
	if (!lbl) {
		qWarning() << "failed to create STARRED label";
	}
	else {
		if (starred_on){
			d->m_labels |= lbl->labelMask();
		}
		else {
			d->m_labels &= ~(lbl->labelMask());
		}
	}

	QString msg_id = d->id();
	gmail::ModifyMessageArg arg(msg_id);
	std::list <QString> labels;
	labels.push_back("STARRED");
	if (starred_on)
	{
		arg.setAddlabels(labels);
	}
	else 
	{
		arg.setRemovelabels(labels);
	}

	messages::MessagesRoutes* msg = getMessages();
	GoogleTask<messages::MessageResource>* t = msg->modify_Async(arg);

	std::function<void()> onMessageLabelUpdated = [=]()
	{
		if (m_GMailCache &&
			m_GMailCache->hasLocalPersistentStorate())
		{
			m_GMailCache->update_persistent_labels(msg_id, d->labelsBitMap());
		}
	};

	if (t->isFinished())
	{
		onMessageLabelUpdated();
	}
	else
	{
		QObject::connect(t, &EndpointRunnable::finished, [=]()
		{
			onMessageLabelUpdated();
		});
	}
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
    mail_cache::GMailCacheQueryResult* r = getCacheMessages_Async(state, id_list);
    std::map<QString, std::shared_ptr<mail_cache::MessageData>> res = r->waitForResultAndRelease();
    ApiAutotest::INSTANCE() << QString("loaded/cached %1 messages, mem_cache-hit: %2, db-cache-hit: %3")
        .arg(res.size())
        .arg(r->mem_cache_hit_count())
        .arg(r->db_cache_hit_count());    
};

void GmailRoutes::autotest()
{
#define AUTOTEST_SIZE 1000

	/// check persistant cache update ///
	if (!setupSQLiteCache("gmail_autotest.sqlite"))
	{
		ApiAutotest::INSTANCE() << "Failed to setup SQL database";
		return;
	};

	auto storage = cache()->sqlite_storage();

	auto m = gmail::SendMimeMessageArg::EXAMPLE(0, 0);
	QString rfc822_sample = m->toRfc822();
	ApiAutotest::INSTANCE() << "rcf822" << rfc822_sample;
	ApiAutotest::INSTANCE() << "";
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
    autotestParLoad(EDataState::snippet, id_list);
    autotestParDBLoad(EDataState::snippet, id_list);
    autotestParDBLoad(EDataState::body, id_list);

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
    autotestParDBLoad(EDataState::snippet, id_list);
    autotestParDBLoad(EDataState::body, id_list);
    
    deleteFirst10();
    autotestParDBLoad(EDataState::body, id_list);
    autotestParDBLoad(EDataState::snippet, id_list);
    autotestParDBLoad(EDataState::snippet, id_list);
    autotestParDBLoad(EDataState::body, id_list);
    
    int idx = 1;
    bool print_cache = true;
    if (print_cache)
    {
        using MSG_LIST = std::list<std::shared_ptr<mail_cache::MessageData>>;
        std::unique_ptr<mail_cache::MessagesList> lst = getCacheMessages(-1);
        for (auto& i : lst->messages)
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
        QString s = QString("Total messages %1").arg(lst->messages.size());
        ApiAutotest::INSTANCE() << s;
    }
    //*** check latest emails
    bool check_email = true;
    if (check_email)
    {
        idx = 1;
        std::unique_ptr<mail_cache::MessagesList> lst2 = getNextCacheMessages();
        for (auto& i : lst2->messages)
        {
            mail_cache::MessageData* m = i.get();
            QString s = QString("%1. %2 %3").arg(idx).arg(m->id()).arg(m->snippet());
            ApiAutotest::INSTANCE() << s;
            idx++;
        }
        QString s = QString("Next(new) messages %1").arg(lst2->messages.size());
        ApiAutotest::INSTANCE() << s;
    }

	bool randomize_labels = true;
	if (randomize_labels)
	{
		std::list<mail_cache::LabelData*> labels = getLoadedLabels();				

		if (labels.size() > 0)
		{			
			auto l_iterator = labels.begin();
			uint64_t lmask = (*l_iterator)->labelMask();
			std::unique_ptr<mail_cache::MessagesList> lst = getCacheMessages(-1);
			int counter = 0;
			int totalNumber = lst->messages.size();
			int group_size = totalNumber / labels.size();
			qDebug() << QString("Resetting labels: %1 on %2 messages, with group size %3")
				.arg(labels.size())
				.arg(totalNumber)
				.arg(group_size);
			for (auto& i : lst->messages)
			{
				mail_cache::MessageData* m = i.get();
				storage->updateMessageLabels(m->id(), lmask);

				counter++;

				if (counter > group_size) {
					counter = 0;
					l_iterator++;
					if (l_iterator != labels.end()) {
						lmask |= (*l_iterator)->labelMask();
						qDebug() << QString("Next Label group: %1/%2")
							.arg((*l_iterator)->labelName())
							.arg(lmask);						
					}
					else 
					{
						break;
					}
				}
			}
		}
	}    

	ApiAutotest::INSTANCE().enableRequestLog(true);
};
#endif
