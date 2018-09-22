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

#include "gmail/GmailCache.h"
#include "google/endpoint/Cache.h"


namespace googleQt
{
    namespace mail_cache
    {
        class GmailCacheRoutes : public QObject
        {
            Q_OBJECT
        public:
            GmailCacheRoutes(Endpoint& endpoint, GmailRoutes& gmail_routes);

            mail_cache::cache_ptr       cache() { return m_GMailCache; }

            /// init local cache table using SQlite DB, tables will have 'dbprefix' prefix
            /// file path and DB-name should be specified
            /// downloadPath - directory for attachment download
            bool setupSQLiteCache(QString dbPath,
                QString downloadPath,
                QString contactCachePath,
                QString dbName = "googleqt",
                QString dbprefix = "api");

            /// if setupSQLiteCache was called before
            /// used to switch to a new user
            bool resetSQLiteCache();

            /// returns true if local cache has been setup
            bool hasCache()const;


            //std::unique_ptr<UserBatchResult<QString, messages::MessageResource>>   
            RESULT_LIST<messages::MessageResource>&&    getUserBatchMessages(QString userId, EDataState, const std::list<QString>& id_list);
            ConcurrentValueRunner<QString, mail_cache::MessagesReceiver, messages::MessageResource>* getUserBatchMessages_Async(QString userId, EDataState, const std::list<QString>& id_list);
            
            /// load emails by ID-list while updating local cache
            mail_cache::data_list_uptr getCacheMessages(QString userId, EDataState, const std::list<QString>& id_list);
            mail_cache::GMailCacheQueryTask* getCacheMessages_Async(QString userId, EDataState, const std::list<QString>& id_list,
                mail_cache::GMailCacheQueryTask* rfetcher = nullptr);

            /// load messages from cache, numberOfMessages = -1 if all messages from cache
            mail_cache::data_list_uptr getCacheMessages(int numberOfMessages, uint64_t labelFilter = 0);


            /// check for new emails - get top messagesCount messages and update cache
            mail_cache::data_list_uptr getNextCacheMessages(QString userId,
                int messagesCount = 40,
                QString pageToken = "",
                QStringList* labels = nullptr,
                QString q = "");
            mail_cache::GMailCacheQueryTask* getNextCacheMessages_Async(QString userId,
                int messagesCount = 40,
                QString pageToken = "",
                QStringList* labels = nullptr,
                QString q = "");

            GoogleVoidTask* trashCacheMessage_Async(QString userId, QString msg_id);

            /// async refresh labels DB table
            GoogleVoidTask* refreshLabels_Async();
            GoogleVoidTask* downloadAttachment_Async(googleQt::mail_cache::msg_ptr m,
                googleQt::mail_cache::att_ptr a,
                QString destinationFolder);
            void refreshLabels();
            std::list<mail_cache::LabelData*> getLoadedLabels(std::set<QString>* in_optional_idset = nullptr);
            std::list<mail_cache::LabelData*> getMessageLabels(mail_cache::MessageData* d);
            bool messageHasLabel(mail_cache::MessageData* d, QString label_id)const;


            /// STARRED label
            bool setStarred(mail_cache::MessageData* d, bool set_it = true);
            GoogleTask<messages::MessageResource>* setStarred_Async(mail_cache::MessageData* d, bool set_it = true);

            /// UNREAD label
            bool setUnread(mail_cache::MessageData* d, bool set_it = true);
            GoogleTask<messages::MessageResource>* setUnread_Async(mail_cache::MessageData* d, bool set_it = true);

            /// IMPORTANT label
            bool setImportant(mail_cache::MessageData* d, bool set_it = true);
            GoogleTask<messages::MessageResource>* setImportant_Async(mail_cache::MessageData* d, bool set_it = true);

#ifdef API_QT_AUTOTEST
            void runAutotest();
            void autotestParDBLoad(EDataState state, const std::list<QString>& id_list);
#endif
        signals:
            void attachmentsDownloaded(googleQt::mail_cache::msg_ptr, googleQt::mail_cache::att_ptr);

        protected:
            mail_cache::GMailCacheQueryTask* newResultFetcher(QString userId, EDataState state);
            GoogleTask<messages::MessageResource>* setLabel_Async(QString label_id,
                mail_cache::MessageData* d,
                bool label_on,
                bool system_label);


        protected:
            Endpoint&    m_endpoint;
            GmailRoutes& m_gmail_routes;
            mutable mail_cache::cache_ptr m_GMailCache;
        };
    };
};
