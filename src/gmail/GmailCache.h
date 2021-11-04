#pragma once

#include "google/endpoint/BatchRunner.h"
#include "google/endpoint/Cache.h"
#include "gmail/messages/MessagesRoutes.h"
#include "gmail/threads/ThreadsRoutes.h"
#include "gmail/labels/LabelsLabelResource.h"
#include <QSqlDatabase>
#include <QSqlQuery>

namespace googleQt{
    class Endpoint;
    class GmailRoutes;

    namespace gcontact {
        class GContactCacheBase;
        class GContactCache;
    };

    namespace mail_cache{
        class GMailCacheQueryTask;
        class GMailSQLiteStorage;
        class MessageData;
        class LabelData;
        class AttachmentData;
        class AccountData;
        class GMailCache;
        class GThreadCache;
        class GmailCacheRoutes;
        class ThreadData;
        class QueryData;
        class LabelCreator;
        class LabelDeleter;

        using msg_ptr           = std::shared_ptr<googleQt::mail_cache::MessageData>;
        using thread_ptr        = std::shared_ptr<googleQt::mail_cache::ThreadData>;
        using query_ptr         = std::shared_ptr<googleQt::mail_cache::QueryData>;
        using msg_list          = std::vector<msg_ptr>;
        using msg_map           = qstring_hash_map<msg_ptr>;
        using thread_list       = std::vector<thread_ptr>;
        using thread_map        = qstring_hash_map<thread_ptr>;
        using query_map         = qstring_hash_map<query_ptr>;
        using query_db_map      = std::unordered_map<int, query_ptr>;
        using query_list        = std::vector<query_ptr>;
        using label_ptr         = std::shared_ptr<googleQt::mail_cache::LabelData>;
        using att_ptr           = std::shared_ptr<googleQt::mail_cache::AttachmentData>;
        using label_list        = std::vector<label_ptr>;
        using mdata_result      = std::unique_ptr<CacheDataResult<MessageData>>;
        using tdata_result      = std::unique_ptr<CacheDataResult<ThreadData>>;
        using ATTACHMENTS_LIST  = std::vector<att_ptr>;
        using acc_ptr           = std::shared_ptr<googleQt::mail_cache::AccountData>;
        using THREADS_LIST      = qstring_hash_map<thread_ptr>;

        /**
           reserved syslabels, they are looked up faster
           the order in enum is important since it goes to bitmask
        */
        enum class SysLabel
        {
            NONE = -1,
            IMPORTANT = 0,
            CHAT,
            SENT,
            INBOX,
            TRASH,
            DRAFT,
            SPAM,
            STARRED,
            UNREAD,
            CATEGORY_PERSONAL,
            CATEGORY_SOCIAL,
            CATEGORY_FORUMS,
            CATEGORY_UPDATES,
            CATEGORY_PROMOTIONS
        };

        extern QString                  sysLabelId(SysLabel l);
        extern QString                  sysLabelName(SysLabel l);
        extern uint64_t                 reservedSysLabelMask(SysLabel l);
        extern std::vector<SysLabel>    mask2reservedSysLabel(uint64_t);
        extern std::vector<QString>     mask2SysLabelIds(uint64_t);

        /**
           MessageData - local persistant rfc822 basic data.
           Number of headers limited to CC BCC From To Subject
           to optimize local storage. Can be partially loaded
           with only ID, snippet or fully loaded with html text.
           Stored as one record in SQL table. Can be deleted and 
           later dynamically restored from Gmail using ID.
           Containes labels as bitmap.
           MessageData can be constructed only during data load from
           server (messages::MessageResource - as partial or complete message)
           or during load from DB (local cache), that is why constructors should be made
           private with listed 'friends'/utility classes
        */
        class MessageData : public CacheData
        {
        public:
            ~MessageData();
            void  merge(CacheData* other)override;
            
            int     accountId()const { return m_accountId;}
            QString threadId()const{ return m_thread_id; }

            QString from()const { return m_from; }
            QString to()const { return m_to; }
            QString cc()const { return m_cc; }
            QString bcc()const { return m_bcc; }
            QString subject()const{return m_subject;}
            QString snippet()const { return m_snippet; }
            QString plain()const { return m_plain; }
            QString html()const { return m_html; }
            qlonglong internalDate()const { return m_internalDate; }
            QString references()const {return m_references;}

            bool hasLabel(uint64_t data)const;
            bool hasAllLabels(uint64_t data)const;
            bool hasReservedSysLabel(SysLabel l)const;
            void addSysLabel(SysLabel l);

            /// each label is a bit in int64
            uint64_t labelsBitMap()const{return m_labels;}
             
            ///will load attachments from DB if not loaded already
            const ATTACHMENTS_LIST& getAttachments(GMailSQLiteStorage* storage);
            void clearSnippet();
        protected:          
            void updateSnippet(QString from,
                               QString to,
                               QString cc,
                               QString bcc,
                               QString subject,
                               QString snippet,
                               uint64_t labels,
                               QString references);
            void updateBody(QString plain, QString html);
            void updateLabels(uint64_t labels);
        protected:
            int     m_accountId{-1};
            QString m_thread_id;
            QString m_from;
            QString m_to;
            QString m_cc;
            QString m_bcc;
            QString m_subject;
            QString m_snippet;
            QString m_plain;
            QString m_html;
            qlonglong m_internalDate;
            uint64_t m_labels;
            ATTACHMENTS_LIST m_attachments;
            QString m_references;
        private:
            ///constructor for small(snippet) object
            MessageData(int accId,
                        QString id,
                        QString thread_id,
                        QString from,
                        QString to,
                        QString cc,
                        QString bcc,
                        QString subject,
                        QString snippet,
                        qlonglong internalDate,
                        uint64_t labels,
                        QString references);

            ///constructor for big(snippet+context) object
            MessageData(int accId,
                        QString id,
                        QString thread_id,
                        QString from,
                        QString to,
                        QString cc,
                        QString bcc,
                        QString subject,
                        QString snippet,
                        QString plain,
                        QString html,
                        qlonglong internalDate,
                        uint64_t labels,
                        QString references);

            ///constructor for smalles(labels only) object
            MessageData(int accId,
                        QString id,
                        QString thread_id,
                        uint64_t labels);

            ///constructor for loading from DB
            MessageData(int accId,
                        QString thread_id,
                        int agg_state,
                        QString id,
                        QString from,
                        QString to, 
                        QString cc,
                        QString bcc,
                        QString subject,
                        QString snippet,
                        QString plain,
                        QString html,
                        qlonglong internalDate,
                        uint64_t labels,
                        QString references);

            friend class GMailCacheQueryTask;
            friend class GThreadCacheQueryTask;
            friend class GMailSQLiteStorage;
            friend class GMessagesStorage;
            friend class googleQt::mail_cache::GmailCacheRoutes;
        };      

        /**
           email attachment info, doesn't not contains
           actual data only details on it and possible 
           reference to local file name
        */
        class AttachmentData
        {
        public:
            enum EStatus {
                statusNotDownloaded     = 1,
                statusDownloadInProcess = 2,
                statusDownloaded        = 4
            };

            AttachmentData();
            AttachmentData(QString att_id,
                           QString mimetype,
                           QString filename,
                           quint64 size);
            AttachmentData(QString att_id,
                           QString mimetype,
                           QString filename,
                           QString local_filename,
                           quint64 size,
                           EStatus status);

            QString attachmentId()const { return m_attachment_id; }
            QString mimetype()const { return m_mimetype; }
            QString filename()const { return m_filename; }
            quint64 size()const { return m_size; }
            ///if attachment was downloaded, returns local file name
            QString localFilename()const { return m_local_filename; }

            EStatus status()const { return m_status; }
            
        protected:
            QString m_attachment_id;
            QString m_mimetype;
            QString m_filename;
            QString m_local_filename;
            quint64 m_size;
            EStatus m_status{ statusNotDownloaded };

            friend class GMailCacheQueryTask;
            friend class GMailSQLiteStorage;
            friend class googleQt::mail_cache::GmailCacheRoutes;
        };


        
        /**
           LabelData - system/user labels in gmail, implemented as bitmap,
           where one reference to the label is one bit in 64bit map
        */
        class LabelData 
        {
        public:
            LabelData();

            QString     labelId()const { return m_label_id; }
            QString     labelName()const { return m_label_name; }
            uint64_t    labelMask()const { return m_label_mask; }
            int         labelMaskBase()const {return m_mask_base; }
            bool        isSystem()const { return m_is_system_label; }
            uint64_t    unreadMessages()const { return m_unread_messages; }

            bool        isStarred()const;
            bool        isUnread()const;
            bool        isImportant()const;
            bool        isSpam()const;
            bool        isTrash()const;
            bool        isDraft()const;            
            bool        isPromotionCategory()const;
            bool        isForumsCategory()const;
            bool        isUpdatesCategory()const;
            bool        isSocialCategory()const;
            bool        isPersonalCategory()const;
            
        protected:
            QString     m_label_id;
            QString     m_label_name;
            int         m_mask_base;
            uint64_t    m_label_mask;
            bool        m_is_system_label;
            uint64_t    m_unread_messages;
        private:
            LabelData(QString id,
                      QString name,
                      int mask_base,
                      bool as_system,
                      uint64_t unread_messages);

            friend class GMailSQLiteStorage;
        };

        /**
           mail thread data - a way to group messages that
           belongs to same conversation line
        */
        class ThreadData : public CacheDataWithHistory
        {
        public:
            virtual ~ThreadData();

            int     messagesCount()const { return m_messages_count; }
            QString snippet()const { return m_snippet; }
            void    merge(CacheData* other)override;
            msg_ptr findMessage(QString id);
            const msg_list& messages()const{return m_messages;}
            msg_list& messages(){return m_messages;}
            msg_ptr head() { return m_head; }
            qlonglong internalDate()const;

            /// each label is a bit in int64
            uint64_t    labelsBitMap()const{return m_thread_labels;}

            uint64_t    filterMask()const { return m_filter_mask; }
            void        addFilterMask(uint64_t f);

            bool hasLabel(uint64_t data)const;
            bool hasLimboLabel(uint64_t data)const;
            bool hasAllLabels(uint64_t data)const;
            void resortMessages();
        protected:          
            int         m_messages_count;
            QString     m_snippet;
            uint64_t    m_thread_labels{0},
                        m_limbo_labels{0};///labels not confirmed yet, we waiting for async call to complete but app might assume is succeded
            msg_list    m_messages;
            uint64_t    m_filter_mask{ 0 };
            msg_map     m_mmap;
            msg_ptr     m_head{nullptr};
        private:
            ThreadData(
                QString id,
                quint64 history_id,
                int     messages_count,
                QString snippet,
                uint64_t filter_mask);
            void add_msg(msg_ptr);
            void remove_msg(msg_ptr);
            void rebuildLabelsMap();
            void setupLimboLabels(const label_list& labels2add);

            friend class GThreadCacheQueryTask;
            friend class GMailCacheQueryTask;
            friend class GThreadsStorage;
            friend class GMessagesStorage;
            friend class GMailSQLiteStorage;
            friend class GmailCacheRoutes;
        };      

        /// query results - collection of threads       
        class QueryData
        {
        public:
            QString             qStr()const { return m_q; }
            void                setQStr(QString val) { m_q = val; }
            QString             labelid()const { return m_labelid; }
            void                setLabelid(QString val) { m_labelid = val; }
            bool                hasNewUnsavedThreads()const { return !m_qnew_thread_ids.empty(); }
            const thread_list&  threads_arr()const { return m_qthreads; }
            thread_list&        threads_arr(){ return m_qthreads; }
            const thread_map&   threads_map()const { return m_tmap; }
            QString             backendToken()const { return m_backendToken; }
            void                remove_threads(const std::set<QString>& ids2remove);
            bool                isFilter()const { return (m_filter_mask != 0); }
            uint64_t            filterMask()const { return m_filter_mask; }
            int                 unreadCount()const { return m_unread_count; };
        protected:
            void                recalcUnreadCount();

            int                 m_db_id{ -1 };
            QString             m_q, m_labelid, m_backendToken;
            thread_list         m_qthreads;
            thread_map          m_tmap;
            STRING_LIST         m_qnew_thread_ids;
            uint64_t            m_filter_mask{0};
            time_t              m_last_run_time{0};
            int                 m_unread_count{0};
            bool                m_threads_db_loaded{false};
        private:
            QueryData(int dbid, QString qstr, QString lbid, QString backend_token, uint64_t filter_mask);
            static QString      format_qhash(QString qstr, QString lblid, bool as_filter);
            friend class GQueryStorage;
            friend class GMailSQLiteStorage;
            friend class GmailCacheRoutes;
            friend class GThreadCacheQueryTask;
        };

        class DiagnosticData 
        {
        public:
            int getBatchMessagesClouldQueried()const {return m_msg_cloud_batch_queried;}
            int getMessagesClouldLoaded()const {return m_msg_cloud_email_loaded;}
        protected:
            int m_msg_cloud_batch_queried{0};
            int m_msg_cloud_email_loaded{0};
            friend class GMailSQLiteStorage;
        };

        class AccountData 
        {
        public:
            AccountData(){}
            AccountData(int accId, QString userId):m_accountId(accId), m_userId(userId){}
            int accountId()const { return m_accountId; }
            QString userId()const { return m_userId; }
            void setUserId(QString s) { m_userId = s; }

            const DiagnosticData& diagnostic()const { return m_diagnostic; }

        protected:
            int m_accountId {0};
            QString m_userId{""};
            DiagnosticData m_diagnostic;
            friend class GMailSQLiteStorage;
        };

        class GMailCache;
        class GMailCacheQueryTask: public CacheQueryTask<MessageData>
        {
        public:
            GMailCacheQueryTask(EDataState load,
                                ApiClient* cl,
                                googleQt::mail_cache::GmailCacheRoutes& r,
                                GMailCache* c);
            void fetchFromCloud_Async(const STRING_LIST& id_list)override;

            mdata_result waitForResultAndRelease();
            QString nextPageToken()const{return m_nextPageToken;}
            
        protected:
            void fetchMessage(messages::MessageResource* m);
            void loadHeaders(messages::MessageResource* m,
                             QString& from,
                             QString& to,
                             QString& cc,
                             QString& bcc,
                             QString& subject,
                             QString& references);
            void loadAttachments(messages::MessageResource* m, ATTACHMENTS_LIST& lst);
        protected:
            googleQt::mail_cache::GmailCacheRoutes&  m_r;
            QString m_nextPageToken;
            friend class googleQt::mail_cache::GmailCacheRoutes;
        };

        class GThreadCacheQueryTask : public CacheQueryTask<ThreadData>
        {
        public:
            GThreadCacheQueryTask(googleQt::mail_cache::GmailCacheRoutes& r,
                                  GThreadCache* c,
                                  query_ptr q = nullptr);
            void fetchFromCloud_Async(const STRING_LIST& id_list)override;
            void notifyOnCompletedFromCache()override;
            QString nextPageToken()const{return m_nextPageToken;}
            tdata_result waitForResultAndRelease();
        protected:
            GoogleVoidTask* loadMessagesSnippetsFromCloud_Async(const STRING_LIST& msg_id_list, googleQt::TaskProgress* p);
            GoogleVoidTask* loadMessagesLabelsFromCloud_Async(const STRING_LIST& msg_id_list);

            googleQt::mail_cache::GmailCacheRoutes&  m_r;
            query_ptr m_threads_query;
            QString m_nextPageToken;
            thread_list m_updated_threads;
            thread_list m_new_threads;
            friend class googleQt::mail_cache::GmailCacheRoutes;
        };
        
        class GMailCache : public GoogleCache<MessageData, GMailCacheQueryTask>
        {
        public:
            GMailCache(Endpoint& ept);
            mail_cache::mdata_result topCacheData(int number2load, uint64_t labelFilter);
            void reorder_data_on_completed_fetch(const CACHE_LIST<MessageData>& )override {}
            Endpoint& endpoint() { return m_endpoint; }
        protected:
            Endpoint& m_endpoint;
        };


        class GThreadCache : public GoogleCache<ThreadData, GThreadCacheQueryTask>
        {
        public:
            GThreadCache(ApiEndpoint& ept);
            mail_cache::tdata_result topCacheData(int number2load, uint64_t labelFilter);
            ///remove empty threads - without any messages
            void verifyData();
            void reorder_data_on_completed_fetch(const CACHE_LIST<ThreadData>& from_cloud)override;
        };

        class GMessagesStorage : public LocalPersistentStorage<MessageData, GMailCacheQueryTask>
        {
        public:
            GMessagesStorage(GMailSQLiteStorage* s);
            virtual ~GMessagesStorage(){}

            bool isValid()const override;
            void update_db(EDataState state, CACHE_LIST<MessageData>& r)override;
            void remove_db(const std::set<QString>& ids2remove)override;
            void insert_db(EDataState state, CACHE_LIST<MessageData>& r)override;
        protected:
            bool insertDbInBatch(EDataState state, CACHE_LIST<MessageData>& r);
            bool updateDbInBatch(EDataState state, CACHE_LIST<MessageData>& r);
            ///create message object from db and attach to a thread
            std::shared_ptr<MessageData> loadMessageFromDb(thread_ptr t, QSqlQuery* q, int msg_field_start_index = 0);
            void insertDbAttachmentData(const MessageData& m);
            bool loadMessagesFromDb();
            QString insertSnippetSQL()const;
            QString insertBodySQL()const;
            QString updateSnippetSQL()const;
            QString updateBodySQL()const;
            QString insertLabelsSQL()const;
            QString updateLabelsSQL()const;
            bool execOutOfBatchSQL(EDataState state, QSqlQuery* q, mail_cache::MessageData* t);
            void bindSQL(EDataState state, QSqlQuery* q, CACHE_LIST<MessageData>& r);
        protected:
            GMailSQLiteStorage*     m_storage;
            friend class GMailSQLiteStorage;
            friend class GThreadsStorage;
            friend class GQueryStorage;
        };


        class GThreadsStorage : public LocalPersistentStorage<ThreadData, GThreadCacheQueryTask>
        {
        public:
            GThreadsStorage(GMailSQLiteStorage* s);
            virtual ~GThreadsStorage(){}
            void update_db(EDataState state, CACHE_LIST<ThreadData>& r)override;
            void remove_db(const std::set<QString>& ids2remove)override;
            void insert_db(EDataState state, CACHE_LIST<ThreadData>& r)override;
            bool isValid()const override;
        protected:
            std::shared_ptr<ThreadData> loadThread(QSqlQuery* q);
            bool loadThreadsFromDb();
            thread_list loadThreadsByIdsFromDb(const std::vector<QString>& thread_ids);
            ///remove empty threads - without any messages
            void verifyThreads();

            QString insertSQL()const;
            QString updateSQL()const;
            QString update_filterSQL()const;
            bool insertDbInBatch(CACHE_LIST<ThreadData>& r);
            bool updateDbInBatch(CACHE_LIST<ThreadData>& r);
            bool execOutOfBatchSQL(QSqlQuery* q, mail_cache::ThreadData* t);
            void bindSQL(QSqlQuery* q, CACHE_LIST<ThreadData>& r);
            void bind_filterSQL(QSqlQuery* q, CACHE_LIST<ThreadData>& r);
        protected:
            GMailSQLiteStorage*     m_storage;
            friend class GMailSQLiteStorage;
            friend class GQueryStorage;
        };

        class LabelProcessorTask : public GoogleVoidTask
        {
        public:
            const STRING_LIST&   getCompletedIds()const { return m_completed_ids; }

        protected:
            LabelProcessorTask(ApiClient* cl) :GoogleVoidTask(cl) {}
            STRING_LIST      m_completed_ids;
            friend class GmailCacheRoutes;
        };

        /// GQueryStorage - we can store query results locally if needed
        /// this is for specific folder filters when we can store query 
        /// results offline, also improve GUI updates
        class GQueryStorage 
        {
        public:
            GQueryStorage(GThreadsStorage* s, GMessagesStorage* ms);
            query_ptr ensure_q(QString q_str, QString labelid, bool as_filter);
            query_ptr lookup_q(QString q_str, QString labelid, bool as_filter);
            bool remove_q(query_ptr q);
            bool update_q_backend_token(query_ptr q, QString token);
            void remove_threads_from_all_q(const std::set<QString>& ids2remove);
            std::vector<query_ptr> filterRules();
        protected:
            void insert_db_threads(query_ptr q);
            bool loadQueriesFromDb();
            bool loadQueryThreadsFromDb(query_ptr d);
            QString insertSQLthreads(query_ptr q)const;
            void bindSQL(QSqlQuery* q, STRING_LIST& r);
            uint64_t nextFilterMask()const;
        protected:
            GThreadsStorage*        m_tstorage{ nullptr };
            GMessagesStorage*       m_mstorage{ nullptr };
            query_map               m_qmap;
            query_db_map            m_q_dbmap;
            uint64_t                m_all_filters_mask{0};
            friend class GMailSQLiteStorage;
            friend class GThreadCacheQueryTask;
            friend class GmailCacheRoutes;
        };
        
        /**
           GMailSQLiteStorage - utility class as helper for GMailCache.
           Implements SQL manipulation functions.
        */
        class GMailSQLiteStorage
        {
        public:
            GMailSQLiteStorage(GMailCache* mc,
                GThreadCache* tc);
            bool init_db(QString dbPath, 
                         QString downloadPath,
                         QString contactCachePath,
                         QString dbName, 
                         QString db_meta_prefix);
            void close_db();
            
            bool isValid()const {return m_initialized;}
            ///directory for attachments
            QString downloadDir()const { return m_downloadDir; }
            ///directory for contacts photos & thumbnails
            QString contactCacheDir()const{return m_contactCacheDir;}
            QString findAttachmentFile(att_ptr att)const;
            void invalidateAttachmentLocalCacheFile(att_ptr att);

            label_ptr findLabel(QString label_id);
            label_ptr findLabel(SysLabel sys_label);
            ///CaseInsensitive search for name
            label_ptr findLabelByName(QString name);
            LabelData* ensureLabel(int accId, QString label_id, bool system_label, int mask_base = -1);
            void update_message_labels_db(int accId, QString msg_id, uint64_t flags);
            void update_attachment_local_file_db(googleQt::mail_cache::msg_ptr m, 
                                                 googleQt::mail_cache::att_ptr a, 
                                                 QString file_name);
            /// load label info by set of label IDs, if null return all available labels
            std::vector<mail_cache::label_ptr> getLabelsInSet(std::set<QString>* in_optional_idset = nullptr);
            thread_list loadThreadsByIdsFromDb(const std::vector<QString>& thread_ids);

            uint64_t packLabels(const std::vector<QString>& labels);
            std::vector<mail_cache::label_ptr> unpackLabels(const uint64_t& data)const;

            bool loadAttachmentsFromDb(MessageData& m);
            bool deleteAttachmentsFromDb(QString msg_id);
            bool markMailAsTrashedInDb(MessageData& m);
            void updateMessagesDiagnostic(int inc_batch, int inc_msg);
            int getCacheMessagesCount(mail_cache::label_ptr);
            //bool updateMailGuiZoomInDb(MessageData& m, qreal zoom);

            /// find account by ID
            QString findUser(int accId);
            /// find account by email
            int findAccount(QString userId);
            /// current selected account
            acc_ptr currentAccount();
            int     currentAccountId()const {return m_accId;}
            /// returns accounts list, all in unique pointers and enforces move-semantics
            std::vector<acc_ptr> getAccounts();
            /// remove account
            bool deleteAccountFromDb(int accId);
            /// update userId in DB
            bool updateAccountInDb(int accId, QString userId);
            /// only account with no messages in cache can be renamed
            bool canRenameAccount(int accId);
            /// returns ID of new account or -1 in case of error            
            int addAccountDb(QString userId);

            GQueryStorage*      qstorage() { return m_qstorage.get(); }
            GMailCache*         mcache() { return m_msg_cache; }
            GThreadCache*       tcache() { return m_thread_cache; }


            thread_ptr          findThread(QString thread_id);
            msg_ptr             findMessage(QString msg_id);

            msg_list loadMessagesByIdsFromDb(const std::vector<QString>& msg_ids);

            uint64_t                lastHistoryId()const { return m_lastHistoryId; }
            void                    setHistoryId(uint64_t val){m_lastHistoryId = val;}
            int                     autoloadLimit()const {return m_cache_autoload_limit;}

            /// cached 'modifyBatch'
            void                    registerBatchUpdate(QString msg_id, SysLabel l);
            void                    clearBatchUpdate(const std::vector<QString>& msg_ids);
            const qstring_hash_map<int>&    getBatchUpdateRequests()const { return m_batch_request; };          
        protected:
            QString metaPrefix()const { return m_metaPrefix; }
            bool execQuery(QString sql);
            bool execContactQuery(QString sql);
                        
            QSqlQuery* selectQuery(QString sql)const;
            QSqlQuery* selectContactQuery(QString sql);
            QSqlQuery* prepareQuery(QString sql)const;
            QSqlQuery* prepareContactQuery(QString sql)const;
            
            QString lastSqlError()const;

            QSqlQuery* doPrepareQuery(std::unique_ptr<QSqlQuery>& q, QString sql)const;
            bool doExecQuery(std::unique_ptr<QSqlQuery>& q, QString sql);

            QSqlQuery* startTransaction(QString sql);
            bool startTransaction();
            bool rollbackTransaction();
            bool commitTransaction();
            
            bool loadLabelsFromDb();
            bool updateDbLabel(const labels::LabelResource& lbl, LabelData* db_lbl);
            LabelData* insertDbLabel(const labels::LabelResource& lbl);
            LabelData* insertDbLabel(int accId,
                                     QString label_id,
                                     QString name, 
                                     bool system_label,
                                     int mask_base = -1);
            LabelData* createAndInsertLabel(
                QString label_id,
                QString label_name,
                bool label_is_system,
                uint64_t unread_messages,
                int mask_base);
            bool deleteDbLabel(QString labelId);
            
            void reloadDbAccounts();
            bool reloadDbConfig();
            bool ensureMailTables();
            void loadBatchUpdateFromDb();
        protected:
            bool m_initialized {false};
            QSqlDatabase        m_gmail_db;
            mutable std::unique_ptr<QSqlQuery>  m_query{ nullptr }, m_contact_query{nullptr};
            GMailCache*                         m_msg_cache{ nullptr };
            GThreadCache*                       m_thread_cache{ nullptr };
            std::unique_ptr<GMessagesStorage>   m_mstorage;
            std::unique_ptr<GThreadsStorage>    m_tstorage;
            std::unique_ptr<GQueryStorage>      m_qstorage;
            std::map<QString, std::shared_ptr<LabelData>, CaseInsensitiveLess> m_acc_labels;
            std::vector<std::shared_ptr<LabelData>> m_maskbase2label;
            std::set<int>                       m_avail_label_base;
            std::unordered_map<int, mail_cache::acc_ptr>    m_id2acc;
            std::map<QString, mail_cache::acc_ptr, CaseInsensitiveLess> m_user2acc;
            qstring_hash_map<QString>                   m_configs;
            qstring_hash_map<int>                       m_batch_request;
            QString m_dbPath;
            QString m_downloadDir;
            QString m_contactCacheDir;
            QString m_dbName;
            QString m_metaPrefix;
            int     m_accId{-1};
            int     m_cache_autoload_limit{1000};
            uint64_t    m_lastHistoryId{0};///last valid history id
            friend class googleQt::mail_cache::GmailCacheRoutes;
            friend class googleQt::gcontact::GContactCache;
            friend class googleQt::mail_cache::GMessagesStorage;
            friend class googleQt::mail_cache::GThreadsStorage;
            friend class googleQt::mail_cache::GQueryStorage;
            friend class googleQt::mail_cache::LabelCreator;
            friend class googleQt::mail_cache::LabelDeleter;
        };//GMailSQLiteStorage


        /**
           we know how to prepare argument and query for email message by ID
           we work in context of current userId
        */
        class MessagesReceiver
        {
        public:
            MessagesReceiver(GmailRoutes& r, EDataState f);
            GoogleTask<messages::MessageResource>* routeRequest(QString message_id);
        protected:
            GmailRoutes&    m_r;
            EDataState      m_msg_format;
        };
      
        /**
           we know how to prepare argument and query for email thread by ID
        */
        class ThreadsReceiver
        {
        public:
            ThreadsReceiver(GmailRoutes& r);
            GoogleTask<threads::ThreadResource>* routeRequest(QString thread_id);
        protected:
            GmailRoutes&    m_r;
        };
    };
};
