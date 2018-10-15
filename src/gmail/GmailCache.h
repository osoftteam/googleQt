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

        using msg_ptr           = std::shared_ptr<googleQt::mail_cache::MessageData>;
		using thread_ptr		= std::shared_ptr<googleQt::mail_cache::ThreadData>;
		using msg_list			= std::list<msg_ptr>;
		using msg_map			= std::map<QString, msg_ptr>;
		using thread_list		= std::list<thread_ptr>;
		using thread_map		= std::map<QString, thread_ptr>;
        using label_ptr         = std::shared_ptr<googleQt::mail_cache::LabelData>;
        using att_ptr           = std::shared_ptr<googleQt::mail_cache::AttachmentData>;
        using mdata_list_uptr    = std::unique_ptr<CacheDataList<MessageData>>;
		using tdata_list_uptr   = std::unique_ptr<CacheDataList<ThreadData>>;
        using ATTACHMENTS_LIST  = std::list<att_ptr>;
        using acc_ptr           = std::shared_ptr<googleQt::mail_cache::AccountData>;
        using mcache_ptr         = std::shared_ptr<mail_cache::GMailCache>;
		using tcache_ptr		= std::shared_ptr<mail_cache::GThreadCache>;		
		using storage_ptr		= std::shared_ptr<mail_cache::GMailSQLiteStorage>;
		using THREADS_LIST		= std::map<QString, thread_ptr>;

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

            bool inLabelFilter(uint64_t data)const;
            bool inLabelFilter(const std::set<uint64_t>& ANDfilter)const;
            
            /// each label is a bit in int64
            uint64_t labelsBitMap()const{return m_labels;}
             
            ///will load attachments from DB if not loaded already
            const ATTACHMENTS_LIST& getAttachments(GMailSQLiteStorage* storage);

        protected:          
            void updateSnippet(QString from,
                               QString to,
                               QString cc,
                               QString bcc,
                               QString subject,
                               QString snippet,
                               qlonglong labels);
            void updateBody(QString plain, QString html);
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
				qlonglong labels);

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
				qlonglong labels);

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
                        qlonglong labels);

            friend class GMailCacheQueryTask;
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
                statusNotDownloaded,
                statusDownloadInProcess,
                statusDownloaded
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

        extern QString sysLabelId(SysLabel l);
        extern QString sysLabelName(SysLabel l);
        
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
			int		messagesCount()const { return m_messages_count; }
			QString snippet()const { return m_snippet; }
			void	merge(CacheData* other)override;
			msg_ptr findMessage(QString id);
            const msg_list& messages()const{return m_messages;}
            msg_list& messages(){return m_messages;}
            msg_ptr head();
		protected:			
			int			m_messages_count;
			QString		m_snippet;
			msg_list	m_messages;
			msg_map		m_map;
            msg_ptr     m_head_msg;
		private:
			ThreadData(
				QString id,
				quint64 history_id,
				int		messages_count,
				QString snippet);			
			void add_msg(msg_ptr);

            friend class GThreadCacheQueryTask;
            friend class GMailCacheQueryTask;
            friend class GThreadsStorage;
			friend class GMessagesStorage;
			friend class GMailSQLiteStorage;
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
                                ApiEndpoint& ept,
                                googleQt::mail_cache::GmailCacheRoutes& r,
                                std::shared_ptr<GMailCache> c);
            void fetchFromCloud_Async(const std::list<QString>& id_list)override;

            mdata_list_uptr waitForResultAndRelease();
            QString nextPageToken()const{return m_nextPageToken;}
            
        protected:
            void fetchMessage(messages::MessageResource* m);
            void loadHeaders(messages::MessageResource* m,
                             QString& from,
                             QString& to,
                             QString& cc,
                             QString& bcc,
                             QString& subject);
            void loadLabels(messages::MessageResource* m, uint64_t& labels);
            void loadAttachments(messages::MessageResource* m, ATTACHMENTS_LIST& lst);
        protected:
            googleQt::mail_cache::GmailCacheRoutes&  m_r;
            QString m_nextPageToken;
            friend class googleQt::mail_cache::GmailCacheRoutes;
        };

		class GThreadCacheQueryTask : public CacheQueryTask<ThreadData>
		{
		public:
			GThreadCacheQueryTask(googleQt::mail_cache::GmailCacheRoutes& r, std::shared_ptr<GThreadCache> c);
			void fetchFromCloud_Async(const std::list<QString>& id_list);
            QString nextPageToken()const{return m_nextPageToken;}
			tdata_list_uptr waitForResultAndRelease();
		protected:
			///return list of messages
			std::list<QString> fetchThread(threads::ThreadResource* t);

		protected:
			googleQt::mail_cache::GmailCacheRoutes&  m_r;
            QString m_nextPageToken;
			thread_list m_updated_threads;
			thread_list m_new_threads;
            friend class googleQt::mail_cache::GmailCacheRoutes;
		};
        
        class GMailCache : public GoogleCache<MessageData, GMailCacheQueryTask>
        {
        public:
            GMailCache(ApiEndpoint& ept);
            mail_cache::mdata_list_uptr topCacheData(int number2load, uint64_t labelFilter);
        };


		class GThreadCache : public GoogleCache<ThreadData, GThreadCacheQueryTask>
		{
        public:
            GThreadCache(ApiEndpoint& ept);
            mail_cache::tdata_list_uptr topCacheData(int number2load, uint64_t labelFilter);
		};

		class GMessagesStorage : public LocalPersistentStorage<MessageData, GMailCacheQueryTask>
		{
		public:
			GMessagesStorage(GMailSQLiteStorage* s, mcache_ptr c);

			bool isValid()const override;
			std::list<QString> load_db(EDataState state, const std::list<QString>& id_list, GMailCacheQueryTask* cr)override;
			void update_db(EDataState state, CACHE_QUERY_RESULT_LIST<MessageData>& r)override;
			void remove_db(const std::set<QString>& ids2remove)override;
			void insert_db(EDataState state, CACHE_QUERY_RESULT_LIST<MessageData>& r)override;
		protected:
			///create message object from db and attach to a thread
            std::shared_ptr<MessageData> loadMessageFromDb(thread_ptr t, QSqlQuery* q);
			void insertDbAttachmentData(const MessageData& m);
            bool loadMessagesFromDb();
		protected:
			GMailSQLiteStorage*		m_storage;
			std::weak_ptr<mail_cache::GMailCache>	m_cache;
            friend class GMailSQLiteStorage;
		};


		class GThreadsStorage : public LocalPersistentStorage<ThreadData, GThreadCacheQueryTask>
		{
		public:
            GThreadsStorage(GMailSQLiteStorage* s, tcache_ptr c);
            std::list<QString> load_db(EDataState state, const std::list<QString>& id_list, GThreadCacheQueryTask* cr)override;
            void update_db(EDataState state, CACHE_QUERY_RESULT_LIST<ThreadData>& r)override;
            void remove_db(const std::set<QString>& ids2remove)override;
			void insert_db(EDataState state, CACHE_QUERY_RESULT_LIST<ThreadData>& r)override;
            bool isValid()const override;
        protected:
            std::shared_ptr<ThreadData> loadThread(QSqlQuery* q);
			bool loadThreadsFromDb();

			QString insertSQL()const;
			bool execSQLwithValues(QSqlQuery* q, mail_cache::ThreadData* t);
		protected:
			GMailSQLiteStorage*		m_storage;
			std::weak_ptr<mail_cache::GThreadCache>	m_cache;
            friend class GMailSQLiteStorage;            
		};

        
        /**
           GMailSQLiteStorage - utility class as helper for GMailCache.
           Implements SQL manipulation functions.
        */
        class GMailSQLiteStorage
        {
        public:
            GMailSQLiteStorage(mcache_ptr mc,
                               tcache_ptr tc,
                               std::shared_ptr<gcontact::GContactCache> cc);
            bool init_db(QString dbPath, 
                         QString downloadPath,
                         QString contactCachePath,
                         QString dbName, 
                         QString db_meta_prefix);
            void close_db();
            
            bool isValid()const {return m_initialized;};
            ///directory for attachments
            QString downloadDir()const { return m_downloadDir; }
            ///directory for contacts photos & thumbnails
            QString contactCacheDir()const{return m_contactCacheDir;}
            QString findAttachmentFile(att_ptr att)const;
            void invalidateAttachmentLocalCacheFile(att_ptr att);

            LabelData* findLabel(QString label_id);
            LabelData* findLabel(SysLabel sys_label);
            LabelData* ensureLabel(int accId, QString label_id, bool system_label);
            void update_message_labels_db(int accId, QString msg_id, uint64_t flags);
            void update_attachment_local_file_db(googleQt::mail_cache::msg_ptr m, 
                                                 googleQt::mail_cache::att_ptr a, 
                                                 QString file_name);
            /// load label info by set of label IDs, if null return all available labels
            std::list<mail_cache::LabelData*> getLabelsInSet(std::set<QString>* in_optional_idset = nullptr);

            uint64_t packLabels(const std::list <QString>& labels);
            std::list<mail_cache::LabelData*> unpackLabels(const uint64_t& data)const;

            bool loadAttachmentsFromDb(MessageData& m);
            bool deleteAttachmentsFromDb(QString msg_id);
            bool markMailAsTrashedInDb(MessageData& m);
			void updateMessagesDiagnostic(int inc_batch, int inc_msg);
			//bool updateMailGuiZoomInDb(MessageData& m, qreal zoom);

			/// find account by ID
            QString findUser(int accId);
			/// find account by email
            int findAccount(QString userId);
			/// current selected account
			acc_ptr currentAccount();
			int     currentAccountId()const {return m_accId;}
            /// returns accounts list, all in unique pointers and enforces move-semantics
            std::list<acc_ptr> getAccounts();
            /// remove account
            bool deleteAccountFromDb(int accId);
            /// update userId in DB
            bool updateAccountInDb(int accId, QString userId);
            /// only account with no messages in cache can be renamed
            bool canRenameAccount(int accId);
            /// returns ID of new account or -1 in case of error            
            int addAccountDb(QString userId);

			thread_ptr findThread(QString thread_id);
        protected:
			QString metaPrefix()const { return m_metaPrefix; }
			bool execQuery(QString sql);
			QSqlQuery* prepareQuery(QString sql);
			QSqlQuery* selectQuery(QString sql);
			QString lastSqlError()const;
            
            mcache_ptr lock_mcache();
            tcache_ptr lock_tcache();

            bool loadLabelsFromDb();
            bool updateDbLabel(const labels::LabelResource& lbl);           
            LabelData* insertDbLabel(const labels::LabelResource& lbl);
            LabelData* insertDbLabel(int accId,
                                     QString label_id,
                                     QString name, 
                                     QString label_type);
            LabelData* createAndInsertLabel(
                                            QString label_id,
                                            QString label_name,
                                            bool label_is_system,
                                            uint64_t unread_messages,
                                            int mask_base
                                            );
            
            void reloadDbAccounts();
            bool reloadDbConfig();
            bool ensureMailTables();            
        protected:
            bool m_initialized {false};
            QSqlDatabase     m_db;
            std::unique_ptr<QSqlQuery>          m_query{nullptr};
            std::weak_ptr<GMailCache>           m_msg_cache;
            std::weak_ptr<GThreadCache>         m_thread_cache;
            std::unique_ptr<GMessagesStorage>   m_mstorage;
            std::unique_ptr<GThreadsStorage>    m_tstorage;
            std::weak_ptr<gcontact::GContactCache>    m_contact_cache;
            std::map<QString, std::shared_ptr<LabelData>> m_acc_labels;
            std::map<int, std::shared_ptr<LabelData>> m_acc_maskbase2labels;
            std::set<int> m_avail_label_base;
            std::map<int, mail_cache::acc_ptr> m_id2acc;
            std::map<QString, mail_cache::acc_ptr, CaseInsensitiveLess> m_user2acc;
            std::map<QString, QString> m_configs;
            QString m_dbPath;
            QString m_downloadDir;
            QString m_contactCacheDir;
            QString m_dbName;
            QString m_metaPrefix;
            int     m_accId{-1};
            friend class googleQt::mail_cache::GmailCacheRoutes;
            friend class googleQt::gcontact::GContactCache;
            friend class googleQt::mail_cache::GMessagesStorage;
            friend class googleQt::mail_cache::GThreadsStorage;
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
		we know how to prepare argument and query for email message by ID
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
