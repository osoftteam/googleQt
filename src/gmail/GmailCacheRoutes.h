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

            mail_cache::mcache_ptr      mcache() { return m_GMsgCache; }
			mail_cache::tcache_ptr      tcache() { return m_GThreadCache; }
			mail_cache::storage_ptr		storage() {return m_lite_storage;};
			Endpoint&					endpoint() { return m_endpoint; }

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

            RESULT_LIST<messages::MessageResource>&&    getUserBatchMessages(EDataState, const std::list<QString>& id_list);
            ConcurrentValueRunner<QString, mail_cache::MessagesReceiver, messages::MessageResource>* getUserBatchMessages_Async(EDataState, const std::list<QString>& id_list);
            
            /// load emails by ID-list while updating local cache
            mail_cache::mdata_result getCacheMessages(EDataState, const std::list<QString>& id_list);
            mail_cache::GMailCacheQueryTask* getCacheMessages_Async(EDataState, const std::list<QString>& id_list,
                mail_cache::GMailCacheQueryTask* rfetcher = nullptr);

            /// load messages from cache, numberOfMessages = -1 if all messages from cache
            mail_cache::mdata_result getCacheMessages(int numberOfMessages, uint64_t labelFilter = 0);
            mail_cache::tdata_result getCacheThreads(int numberOfThreads, uint64_t labelFilter = 0);

            /// check for new emails - get top messagesCount messages and update cache
            mail_cache::mdata_result getNextCacheMessages(
                int messagesCount = 40,
                QString pageToken = "",
                QStringList* labels = nullptr,
                QString q = "");
            mail_cache::GMailCacheQueryTask* getNextCacheMessages_Async(
                int messagesCount = 40,
                QString pageToken = "",
                QStringList* labels = nullptr,
                QString q = "");

            GoogleVoidTask* trashCacheMessage_Async(QString userId, QString msg_id);

			ConcurrentValueRunner<QString, mail_cache::ThreadsReceiver, threads::ThreadResource>* getUserBatchThreads_Async(const std::list<QString>& id_list);

			/// check for new thread - get top threadsCount threads and update cache
			mail_cache::tdata_result getNextCacheThreads(
				int messagesCount = 40,
				QString pageToken = "",
				QStringList* labels = nullptr,
				QString q = "");
			mail_cache::GThreadCacheQueryTask* getNextCacheThreads_Async(
				int threadsCount = 40,
				QString pageToken = "",
				QStringList* labels = nullptr,
				QString q = "");

			/// load threads by ID-list while updating local cache
			//mail_cache::tdata_result getCacheThreads(const std::list<QString>& id_list);
			mail_cache::GThreadCacheQueryTask* getCacheThreads_Async(const std::list<HistId>& id_list,
				mail_cache::GThreadCacheQueryTask* rfetcher = nullptr);

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
            void autotestThreadDBLoad(const std::list<HistId>& id_list);
#endif
        signals:
            void attachmentsDownloaded(googleQt::mail_cache::msg_ptr, googleQt::mail_cache::att_ptr);

        protected:
            mail_cache::GMailCacheQueryTask* newMessageResultFetcher(EDataState state);
			mail_cache::GThreadCacheQueryTask* newThreadResultFetcher();
            GoogleTask<messages::MessageResource>* setLabel_Async(QString label_id,
                mail_cache::MessageData* d,
                bool label_on,
                bool system_label);


        protected:
            Endpoint&    m_endpoint;
            GmailRoutes& m_gmail_routes;
			mutable mail_cache::storage_ptr m_lite_storage;
            mutable mail_cache::mcache_ptr m_GMsgCache;
			mutable mail_cache::tcache_ptr m_GThreadCache;
        };
    };
};
