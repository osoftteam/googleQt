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

googleQt::mail_cache::GmailCacheRoutes* GmailRoutes::setupCache(QString dbPath,
    QString downloadPath, 
    QString contactCachePath,
    QString dbName,
    QString dbprefix) 
{
    if (m_CacheRoutes) {
        qDebug() << "Gmail cache already enabled, reseting";
        m_CacheRoutes.reset(nullptr);
    }

    m_CacheRoutes.reset(new googleQt::mail_cache::GmailCacheRoutes(*m_endpoint, *this));
    if (!m_CacheRoutes->setupSQLiteCache(dbPath, 
                        downloadPath,
                        contactCachePath,
        dbName,
        dbprefix)) 
    {
        qWarning() << "Failed to setup Gmail Cache";
        return nullptr;
    }

    return m_CacheRoutes.get();
};

googleQt::mail_cache::GmailCacheRoutes* GmailRoutes::cacheRoutes()
{
    if (!m_CacheRoutes) {
        qCritical() << "DB-Cache was not setup, please call GmailRoutes::setupCache first.";
    }

    return m_CacheRoutes.get();
};

void GmailRoutes::onUserReset()
{
    if (m_CacheRoutes) {
        m_CacheRoutes->resetSQLiteCache();
    }
};

#ifdef API_QT_AUTOTEST
void GmailRoutes::autotest() 
{
    QString autotest_db_file = "gmail_autotest.sqlite";
    if (QFile::exists(autotest_db_file)) {
        if (!QFile::remove(autotest_db_file)) {
            qWarning() << "Failed to delete autotest DB file" << autotest_db_file;
            return;
        }
    }

    QString userId = m_endpoint->client()->userId();
    if (!setupCache(autotest_db_file, "downloads", "contacts-cache"))
    {
        ApiAutotest::INSTANCE() << "Failed to setup SQL database";
        return;
    };

    m_CacheRoutes->runAutotest();

    for (int i = 1; i < 5; i++) {
        QString uid = QString("test%1@gmail.com").arg(i);
        m_endpoint->client()->setUserId(uid);
        m_CacheRoutes->runAutotest();
    }

    m_endpoint->client()->setUserId(userId);
}

#endif
