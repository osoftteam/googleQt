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



std::unique_ptr<UserBatchResult<QString, 
                            messages::MessageResource>> GmailRoutes::getUserBatchMessages(EDataState f,
                                                                                      const std::list<QString>& id_list)
{
    return getUserBatchMessages_Async(f, id_list)->waitForResultAndRelease();
};

UserBatchRunner<QString,
            mail_batch::MessagesReceiver,
            messages::MessageResource>* GmailRoutes::getUserBatchMessages_Async(EDataState f, const std::list<QString>& id_list)
{
    mail_batch::MessagesReceiver* mr = new mail_batch::MessagesReceiver(*this, f);
    
    UserBatchRunner<QString,
                mail_batch::MessagesReceiver,
                messages::MessageResource>* r = new UserBatchRunner<QString,
                                                                mail_batch::MessagesReceiver,
                                                                messages::MessageResource>(id_list, mr, *m_endpoint);
    r->run();
    return r;
};

void GmailRoutes::ensureCache()
{
    if (!m_GMailCache) {
        m_GMailCache.reset(new mail_batch::GMailCache(*m_endpoint));
    }
};

mail_batch::GMailCacheQueryResult* GmailRoutes::getNextCacheMessages_Async(int messagesCount /*= 40*/, 
    QString pageToken /*= ""*/, 
    std::set<QString>* msg2skip /*= nullptr*/)
{
    mail_batch::GMailCacheQueryResult* rfetcher = newResultFetcher(EDataState::snippet);

    gmail::ListArg listArg;
    listArg.setMaxResults(messagesCount);
    listArg.setPageToken(pageToken);

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

std::unique_ptr<mail_batch::MessagesList> GmailRoutes::getNextCacheMessages(int messagesCount /*= 40*/, 
    QString pageToken /*= ""*/,
    std::set<QString>* msg2skip /*= nullptr*/)
{
    return getNextCacheMessages_Async(messagesCount, pageToken, msg2skip)->waitForSortedResultListAndRelease();
};

std::unique_ptr<mail_batch::MessagesList> GmailRoutes::getCacheMessages(EDataState state, const std::list<QString>& id_list)
{
    return getCacheMessages_Async(state, id_list)->waitForSortedResultListAndRelease();
};

mail_batch::GMailCacheQueryResult* GmailRoutes::newResultFetcher(EDataState state)
{
    mail_batch::GMailCacheQueryResult* rfetcher = new mail_batch::GMailCacheQueryResult(state, *m_endpoint, *this, m_GMailCache.get());
    return rfetcher;
};

mail_batch::GMailCacheQueryResult* GmailRoutes::getCacheMessages_Async(EDataState state,
                                                                       const std::list<QString>& id_list,
                                                                        mail_batch::GMailCacheQueryResult* rfetcher /*= nullptr*/)
{
    ensureCache();
    if (!rfetcher)
    {
        rfetcher = newResultFetcher(state);
    }    
    m_GMailCache->query_Async(state, id_list, rfetcher);
    return rfetcher;
};

std::unique_ptr<mail_batch::MessagesList> GmailRoutes::getCacheMessages(int numberOfMessages)
{
    ensureCache();
    mail_batch::GMailCacheQueryResult* rfetcher = newResultFetcher(EDataState::snippet);
    m_GMailCache->cacheData(rfetcher, numberOfMessages);
    return rfetcher->waitForSortedResultListAndRelease();
};

bool GmailRoutes::setupSQLiteCache(QString dbPath, QString dbName /*= "googleqt"*/, QString dbprefix /*= "api"*/) 
{
    ensureCache();

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
    std::unique_ptr<UserBatchResult<QString, messages::MessageResource>> br = getUserBatchMessages(state, id_list);
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
        using MSG_LIST = std::list<std::shared_ptr<mail_batch::MessageData>>;
        std::unique_ptr<mail_batch::MessagesList> lst = getCacheMessages(-1);
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
