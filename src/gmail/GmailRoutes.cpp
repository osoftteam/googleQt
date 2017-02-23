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



std::unique_ptr<BatchResult<QString, 
                            messages::MessageResource>> GmailRoutes::getBatchMessages(EDataState f,
                                                                                      const std::list<QString>& id_list)
{
    return getBatchMessages_Async(f, id_list)->waitForResultAndRelease();
};

BatchRunner<QString,
            mail_batch::MessagesReceiver,
            messages::MessageResource>* GmailRoutes::getBatchMessages_Async(EDataState f, const std::list<QString>& id_list)
{
    mail_batch::MessagesReceiver* mr = new mail_batch::MessagesReceiver(*this, f);
    
    BatchRunner<QString,
                mail_batch::MessagesReceiver,
                messages::MessageResource>* r = new BatchRunner<QString,
                                                                mail_batch::MessagesReceiver,
                                                                messages::MessageResource>(id_list, mr, *m_endpoint);
    r->run();
    return r;
};

void GmailRoutes::checkCacheObj()
{
    if (!m_GMailCache) {
        m_GMailCache.reset(new mail_batch::GMailCache(*m_endpoint, *this));
    }
};

GoogleTask<mail_batch::MessagesList>* GmailRoutes::getNextCacheMessages_Async(int messagesCount /*= 40*/,
    QString pageToken /*= ""*/)
{

    GoogleTask<mail_batch::MessagesList>* t = m_endpoint->produceTask<mail_batch::MessagesList>();

    gmail::ListArg listArg;
    listArg.setMaxResults(messagesCount);
    listArg.setPageToken(pageToken);

    std::function<void(std::unique_ptr<messages::MessageListRes>)> id_list_completed_callback =
        [=](std::unique_ptr<messages::MessageListRes> mlist)
    {
        std::list<QString> id_list;
        for (auto& m : mlist->messages())
        {
            QString mid = m.id();
            id_list.push_back(mid);
            try
            {
                std::unique_ptr<mail_batch::MessagesList> lst = std::move(getCacheMessages(EDataState::snippet, id_list));
                lst->nextpage = mlist->nextpagetoken();
                t->completed_callback(std::move(lst));
            }
            catch (GoogleException& e)
            {
                std::unique_ptr<GoogleException> ex(new GoogleException(e));
                t->failed_callback(std::move(ex));
            }
        }
    };

    std::function<void(std::unique_ptr<GoogleException>)> failed_callback =
        [=](std::unique_ptr<GoogleException> ex)
    {
        t->failed_callback(std::move(ex));
    };


    getMessages()->list_AsyncCB(listArg, id_list_completed_callback, failed_callback);

    return t;
};

std::unique_ptr<mail_batch::MessagesList> GmailRoutes::getNextCacheMessages(int messagesCount /*= 40*/, QString pageToken /*= ""*/)
{
    return getNextCacheMessages_Async(messagesCount, pageToken)->waitForResultAndRelease();
};

/*

void getNextCacheMessages_AsyncCB(int messagesCount = 40,
QString pageToken = "",
std::function<void(mail_batch::MessagesList)> completed_callback = nullptr,
std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr
);

void GmailRoutes::getNextCacheMessages_AsyncCB(int messagesCount = 40, 
	QString pageToken = "",
	std::function<void(mail_batch::MessagesList)> completed_callback,
	std::function<void(std::unique_ptr<GoogleException>)> failed_callback
	)
{
	gmail::ListArg listArg;
	listArg.setMaxResults(messagesCount);
	listArg.setPageToken(pageToken);	

	std::function<void(std::unique_ptr<messages::MessageListRes>)> id_list_completed_callback = 
		[=](std::unique_ptr<messages::MessageListRes> mlist)
	{
		std::list<QString> id_list;
		for (auto& m : mlist->messages())
		{
			QString mid = m.id();
			id_list.push_back(mid);
			if (completed_callback)
			{
                mail_batch::MessagesList lst = getCacheMessages(EDataState::snippet, id_list);
                lst.nextpage = mlist->nextpagetoken();
				completed_callback(lst);
			}
		}
	};

	getMessages()->list_AsyncCB(listArg, id_list_completed_callback, failed_callback);
};
*/

std::unique_ptr<mail_batch::MessagesList> GmailRoutes::getCacheMessages(EDataState state, const std::list<QString>& id_list)
{
    return getCacheMessages_Async(state, id_list)->waitForSortedResultListAndRelease();
};

mail_batch::GMailCacheQueryResult* GmailRoutes::getCacheMessages_Async(EDataState state,
                                                                       const std::list<QString>& id_list)
{
    checkCacheObj();
	return m_GMailCache->query_Async(state, id_list);
};

std::unique_ptr<mail_batch::MessagesList> GmailRoutes::getCacheMessages()
{
    checkCacheObj();
    return m_GMailCache->cacheData()->waitForSortedResultListAndRelease();
};

bool GmailRoutes::setupSQLiteCache(QString dbPath, QString dbName /*= "googleqt"*/, QString dbprefix /*= "api"*/) 
{
    if (!m_GMailCache) {
        m_GMailCache.reset(new mail_batch::GMailCache(*m_endpoint, *this));
    }

    if (m_GMailCache->hasLocalPersistentStorate()) 
    {
        return true;
    }

    std::unique_ptr<mail_batch::GMailSQLiteStorage> st(new mail_batch::GMailSQLiteStorage(m_GMailCache.get()));
    if (!st->init(dbPath, dbName, dbprefix)) 
    {
        qWarning() << "Failed to initialize SQLite storage" << dbPath << dbName << dbprefix;
        return false;
    }

    m_GMailCache->setupLocalStorage(st.release());
    return true;
};

#ifdef API_QT_AUTOTEST
void GmailRoutes::autotestParLoad(EDataState state, const std::list<QString>& id_list)
{
    /// check parallel requests ///
    //ApiAutotest::INSTANCE() << QString("=== checking gmail/batch-load of %1 ids").arg(id_list.size());
    std::unique_ptr<BatchResult<QString, messages::MessageResource>> br = getBatchMessages(state, id_list);
    RESULT_LIST<messages::MessageResource*> res = br->results();
    ApiAutotest::INSTANCE() << QString("par-loaded (avoid cache) %1 snippets").arg(res.size());
};

void GmailRoutes::autotestParDBLoad(EDataState state, const std::list<QString>& id_list) 
{
    /// check persistant cache update ///
    if (!setupSQLiteCache("gmail_autotest.sqlite"))
    {
        ApiAutotest::INSTANCE() << "Failed to setup SQL database";
        return;
    };

    mail_batch::GMailCacheQueryResult* r = getCacheMessages_Async(state, id_list);
    std::map<QString, std::shared_ptr<mail_batch::MessageData>> res = r->waitForResultAndRelease();
    ApiAutotest::INSTANCE() << QString("loaded/cached %1 messages, mem_cache-hit: %2, db-cache-hit: %3")
        .arg(res.size())
        .arg(r->mem_cache_hit_count())
        .arg(r->db_cache_hit_count());    
};

void GmailRoutes::autotest()
{
    std::list<QString> id_list;
    for (int i = 1; i <= 1000; i++)
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
        std::list<QString> list2remove;
        for (auto j = id_list.begin(); j != id_list.end(); j++)
        {
            list2remove.push_back(*j);
            if (list2remove.size() >= 10)
                break;
        }

        m_GMailCache->persistent_clear(list2remove);
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
        using MSG_LIST = std::list<std::shared_ptr<mail_batch::MessageData>>;
        std::unique_ptr<mail_batch::MessagesList> lst = getCacheMessages();
        for (auto& i : lst->messages)
        {
            mail_batch::MessageData* m = i.get();
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
        std::unique_ptr<mail_batch::MessagesList> lst2 = getNextCacheMessages();
        for (auto& i : lst2->messages)
        {
            mail_batch::MessageData* m = i.get();
            QString s = QString("%1. %2 %3").arg(idx).arg(m->id()).arg(m->snippet());
            ApiAutotest::INSTANCE() << s;
            idx++;
        }
        QString s = QString("Next(new) messages %1").arg(lst2->messages.size());
        ApiAutotest::INSTANCE() << s;
    }

    ApiAutotest::INSTANCE().enableRequestLog(true);
};
#endif
