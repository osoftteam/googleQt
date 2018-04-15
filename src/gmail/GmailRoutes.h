#pragma once

#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"
#include "google/endpoint/BatchRunner.h"

#include "gmail/messages/MessagesRoutes.h"
#include "gmail/attachments/AttachmentsRoutes.h"
#include "gmail/labels/LabelsRoutes.h"
#include "gmail/users/UsersRoutes.h"
#include "gmail/threads/ThreadsRoutes.h"
#include "gmail/history/HistoryRoutes.h"
#include "gmail/drafts/DraftsRoutes.h"

#include "google/endpoint/Cache.h"
#include "gmail/GmailCache.h"
#include "gmail/GmailCacheRoutes.h"

namespace googleQt{
    class Endpoint;

    namespace mail_cache {
        class MessagesReceiver;
        class MessageData;
        class GMailCacheQueryTask;
        class GMailCache;
    };  

    class GmailRoutes{
    public:
        GmailRoutes(Endpoint*);

        messages::MessagesRoutes*   getMessages();
        attachments::AttachmentsRoutes* getAttachments();
        labels::LabelsRoutes*       getLabels();
        users::UsersRoutes*         getUsers();
        threads::ThreadsRoutes*     getThreads();
        history::HistoryRoutes*     getHistory();
        drafts::DraftsRoutes*       getDrafts();

        Endpoint* endpoint() { return m_endpoint; }        

        /// call setupCache before access any cache functions
        googleQt::mail_cache::GmailCacheRoutes* setupCache(QString dbPath,
                            QString downloadPath,
                            QString contactCachePath,
                            QString dbName = "googleqt",
                            QString dbprefix = "api");

        googleQt::mail_cache::GmailCacheRoutes* cacheRoutes();
        
        ///if we have cache it has to be reloaded for new user that is in context
        void onUserReset();

    public:
#ifdef API_QT_AUTOTEST
        void autotest();
#endif

    protected:        
        std::unique_ptr<messages::MessagesRoutes>       m_MessagesRoutes;
        std::unique_ptr<attachments::AttachmentsRoutes> m_AttachmentsRoutes;
        std::unique_ptr<labels::LabelsRoutes>           m_LabelsRoutes;
        std::unique_ptr<users::UsersRoutes>             m_UsersRoutes;
        std::unique_ptr<threads::ThreadsRoutes>         m_ThreadsRoutes;
        std::unique_ptr<history::HistoryRoutes>         m_HistoryRoutes;
        std::unique_ptr<drafts::DraftsRoutes>           m_DraftsRoutes;
        std::unique_ptr<googleQt::mail_cache::GmailCacheRoutes>    m_CacheRoutes;
        Endpoint*  m_endpoint;
    };

};
