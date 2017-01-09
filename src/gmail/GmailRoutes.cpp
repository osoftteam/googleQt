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
            mail_batch::MesagesReciever,
            messages::MessageResource>* GmailRoutes::getBatchMessages_Async(EDataState f, const std::list<QString>& id_list)
{
    mail_batch::MesagesReciever* mr = new mail_batch::MesagesReciever(*this, f);
    
    BatchRunner<QString,
                mail_batch::MesagesReciever,
                messages::MessageResource>* r = new BatchRunner<QString,
                                                                mail_batch::MesagesReciever,
                                                                messages::MessageResource>(id_list, mr, *m_endpoint);
    r->run();
    return r;
};

std::map<QString, std::shared_ptr<mail_batch::MessageData>> GmailRoutes::getCacheMessages(EDataState state, const std::list<QString>& id_list)
{
	return getCacheMessages_Async(state, id_list)->waitForResultAndRelease();
};

std::unique_ptr<mail_batch::GMailCacheQueryResult> GmailRoutes::getCacheMessages_Async(EDataState state,
                                                                                       const std::list<QString>& id_list)
{
    if (!m_GMailCache) {
		m_GMailCache.reset(new mail_batch::GMailCache(*m_endpoint, *this));
    }
	return m_GMailCache->query_Async(state, id_list);
};

bool GmailRoutes::setupSQLiteCache(QString dbPath, QString dbName /*= "googleqt"*/, QString dbprefix /*= "api"*/) 
{
    if (!m_GMailCache) {
        m_GMailCache.reset(new mail_batch::GMailCache(*m_endpoint, *this));
    }

    if (m_GMailCache->hasLocalPersistentStorate()) 
    {
        qWarning() << "Local SQLite storage already setup";
        return false;
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
void GmailRoutes::autotest()
{
    if (!setupSQLiteCache("gmail_autotest.sqlite")) 
    {
        ApiAutotest::INSTANCE() << "Failed to setup SQL database";
        return;
    };
    std::list<QString> id_list;
    for (int i = 1; i <= 1000; i++) 
    {
        QString id = QString("id_%1").arg(i);
        id_list.push_back(id);
    };
    getCacheMessages(EDataState::snippet, id_list);
    getCacheMessages(EDataState::body, id_list);
    getCacheMessages(EDataState::snippet, id_list);
    getCacheMessages(EDataState::body, id_list);
};
#endif
