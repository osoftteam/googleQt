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

namespace googleQt{
    class Endpoint;

    namespace mail_cache {
        class MessagesReceiver;
        class MessageData;
        class GMailCacheQueryTask;
        class GMailCache;
    };  

    class GmailRoutes : public QObject {
        Q_OBJECT
    public:

        GmailRoutes(Endpoint*);

        Endpoint* endpoint(){return m_endpoint;}

        messages::MessagesRoutes*   getMessages();
        attachments::AttachmentsRoutes* getAttachments();
        labels::LabelsRoutes*       getLabels();
        users::UsersRoutes*         getUsers();
        threads::ThreadsRoutes*     getThreads();
        history::HistoryRoutes*     getHistory();
        drafts::DraftsRoutes*       getDrafts();
		mail_cache::cache_ptr		cache() {return m_GMailCache;}

        std::unique_ptr<UserBatchResult<QString, messages::MessageResource>>   getUserBatchMessages(QString userId, EDataState, const std::list<QString>& id_list);
        UserBatchRunner<QString, mail_cache::MessagesReceiver, messages::MessageResource>* getUserBatchMessages_Async(QString userId, EDataState, const std::list<QString>& id_list);

        /// check for new emails - get top messagesCount messages and update cache
        mail_cache::data_list_uptr getNextCacheMessages(QString userId, 
														int messagesCount = 40,
                                                        QString pageToken = "", 
                                                        QStringList* labels = nullptr);
        mail_cache::GMailCacheQueryTask* getNextCacheMessages_Async(QString userId, 
                                                                    int messagesCount = 40,
                                                                    QString pageToken = "", 
                                                                    QStringList* labels = nullptr);

        /// load emails by ID-list while updating local cache
        mail_cache::data_list_uptr getCacheMessages(QString userId, EDataState, const std::list<QString>& id_list);
        mail_cache::GMailCacheQueryTask* getCacheMessages_Async(QString userId, EDataState, const std::list<QString>& id_list,
                                                                mail_cache::GMailCacheQueryTask* rfetcher = nullptr);

        /// load messages from cache, numberOfMessages = -1 if all messages from cache
        mail_cache::data_list_uptr getCacheMessages(int numberOfMessages, uint64_t labelFilter = 0);

        bool trashCacheMessage(QString msg_id);

        /// init local cache table using SQlite DB, tables will have 'dbprefix' prefix
        /// file path and DB-name should be specified
        /// downloadPath - directory for attachment download
        bool setupSQLiteCache(QString dbPath, 
                              QString downloadPath, 
                              QString dbName = "googleqt", 
                              QString dbprefix = "api");

        /// if setupSQLiteCache was called before
        /// used to switch to a new user
        bool resetSQLiteCache();

        /// returns true if local cache has been setup
        bool hasCache()const;

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
        bool setUread(mail_cache::MessageData* d, bool set_it = true);
        GoogleTask<messages::MessageResource>* setUread_Async(mail_cache::MessageData* d, bool set_it = true);

        /// IMPORTANT label
        bool setImportant(mail_cache::MessageData* d, bool set_it = true);
        GoogleTask<messages::MessageResource>* setImportant_Async(mail_cache::MessageData* d, bool set_it = true);
        
        
    public:
#ifdef API_QT_AUTOTEST
        void autotest();
		void runAutotest();
        void autotestParLoad(EDataState state, const std::list<QString>& id_list);
        void autotestParDBLoad(EDataState state, const std::list<QString>& id_list);
#endif

    signals:
        void attachmentsDownloaded(googleQt::mail_cache::msg_ptr, googleQt::mail_cache::att_ptr);

    protected:
        void ensureCache()const;
        mail_cache::GMailCacheQueryTask* newResultFetcher(QString userId, EDataState state);
        GoogleTask<messages::MessageResource>* setLabel_Async(QString label_id,
                                                              mail_cache::MessageData* d,
                                                              bool label_on,
                                                              bool system_label);
        
        std::unique_ptr<messages::MessagesRoutes>       m_MessagesRoutes;
        std::unique_ptr<attachments::AttachmentsRoutes> m_AttachmentsRoutes;
        std::unique_ptr<labels::LabelsRoutes>           m_LabelsRoutes;
        std::unique_ptr<users::UsersRoutes>             m_UsersRoutes;
        std::unique_ptr<threads::ThreadsRoutes>         m_ThreadsRoutes;
        std::unique_ptr<history::HistoryRoutes>         m_HistoryRoutes;
        std::unique_ptr<drafts::DraftsRoutes>           m_DraftsRoutes;
        mutable mail_cache::cache_ptr					m_GMailCache;
        Endpoint*  m_endpoint;
    };

};
