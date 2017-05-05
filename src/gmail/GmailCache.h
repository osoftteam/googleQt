#pragma once

#include "google/endpoint/BatchRunner.h"
#include "google/endpoint/Cache.h"
#include "gmail/messages/MessagesRoutes.h"
#include "gmail/labels/LabelsLabelResource.h"
#include <QSqlDatabase>
#include <QSqlQuery>

namespace googleQt{
	class Endpoint;
	class GmailRoutes;

	namespace mail_cache{
        class GMailCacheQueryResult;
        class GMailSQLiteStorage;
		class MessageData;
		class LabelData;
		class AttachmentData;

		using msg_ptr	= std::shared_ptr<googleQt::mail_cache::MessageData>;
		using label_ptr = std::shared_ptr<googleQt::mail_cache::LabelData>;
		using att_ptr	= std::shared_ptr<googleQt::mail_cache::AttachmentData>;
		using ATTACHMENTS_LIST = std::list<att_ptr>;

		/**
			MessageData - local persistant rfc822 basic data.
			Number of headers limited to CC BCC From To Subject
			to optimize local storage. Can be partially loaded
			with only ID, snippet or fully loaded with html text.
			Stored as one record in SQL table. Can be deleted and 
			later dynamically restored from Gmail using ID.
			Containes labels as bitmap.
		*/
        class MessageData : public CacheData
        {
        public:
			MessageData(QString id, 
                        QString from, 
                        QString to, 
                        QString cc,
                        QString bcc,
                        QString subject, 
                        QString snippet, 
                        qlonglong internalDate,
                        qlonglong labels);
			MessageData(QString id, 
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

            void  merge(CacheData* other);
            
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
             
			/// userPtr - custom user data pointer
			void* userPtr()const {return m_user_ptr;}
			void  setUserPtr(void* p)const { m_user_ptr = p; }

			bool hasAttachment()const;
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
			int m_not_loaded_att_count{0};///indicative there is attachments info in local cache
			mutable void*    m_user_ptr{nullptr};
     private:
            MessageData(int agg_state,
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
                        qlonglong labels,
						int not_loaded_att_count);

			friend class GMailCacheQueryResult;
			friend class GMailSQLiteStorage;
			friend class googleQt::GmailRoutes;
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

			friend class GMailCacheQueryResult;
			friend class GMailSQLiteStorage;
			friend class GmailRoutes;
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
        
		/**
			LabelData - system/user labels in gmail, implemented as bitmap,
			where one reference to the label is one bit in 64bit map
		*/
		class LabelData 
		{
		public:
			LabelData();

			QString		labelId()const { return m_label_id; }
			QString		labelName()const { return m_label_name; }
			uint64_t	labelMask()const { return m_label_mask; }
			bool		isSystem()const { return m_is_system_label; }
			uint64_t	unreadMessages()const { return m_unread_messages; }

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
            
		protected:
			QString		m_label_id;
			QString		m_label_name;
			int			m_mask_base;
			uint64_t	m_label_mask;
			bool		m_is_system_label;
			uint64_t	m_unread_messages;
		private:
			LabelData(QString id,
				QString name,
				int mask_base,
				bool as_system,
				uint64_t unread_messages);

			friend class GMailSQLiteStorage;
		};

        struct MessagesList
        {
            std::list<msg_ptr> messages;
            std::map<QString, msg_ptr> messages_map;
            EDataState state;
            QString    nextpage;
        };

		class GMailCache;
        class GMailCacheQueryResult: public CacheQueryResult<MessageData>
        {
        public:
            GMailCacheQueryResult(EDataState load, ApiEndpoint& ept, GmailRoutes& r, GMailCache* c);
            void fetchFromCloud_Async(const std::list<QString>& id_list)override;

            std::unique_ptr<MessagesList> waitForSortedResultListAndRelease();
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
            GmailRoutes&  m_r;
        };
        
        class GMailCache : public GoogleCache<MessageData, GMailCacheQueryResult>
        {
        public:
            GMailCache(ApiEndpoint& ept);
			GMailSQLiteStorage* sqlite_storage();
        };

		/**
			GMailSQLiteStorage - utility class as helper for GMailCache.
			Implements SQL manipulation functions.
		*/
        class GMailSQLiteStorage: public LocalPersistentStorage<MessageData, GMailCacheQueryResult>
        {
        public:
            GMailSQLiteStorage(GMailCache* c):m_mem_cache(c){};
            bool init_db(QString dbPath, 
				QString downloadPath,
				QString dbName, 
				QString db_meta_prefix);
            std::list<QString> load_db(EDataState state, const std::list<QString>& id_list,
                                    GMailCacheQueryResult* cr)override;
            void update_db(EDataState state, CACHE_QUERY_RESULT_LIST<MessageData>& r)override;
            bool isValid()const override{return m_initialized;};
            void remove_db(const std::set<QString>& ids2remove)override;            
			///directory for attachments
			QString downloadDir()const { return m_downloadDir; }
			QString findAttachmentFile(att_ptr att)const;

			LabelData* findLabel(QString label_id);
            LabelData* findLabel(SysLabel sys_label);
			LabelData* ensureLabel(QString label_id, bool system_label);
			void update_message_labels_db(QString msg_id, uint64_t flags);
			void update_attachment_local_file_db(googleQt::mail_cache::msg_ptr m, 
				googleQt::mail_cache::att_ptr a, 
				QString file_name);
			/// load label info by set of label IDs, if null return all available labels
			std::list<mail_cache::LabelData*> getLabelsInSet(std::set<QString>* in_optional_idset = nullptr);

			uint64_t packLabels(const std::list <QString>& labels);
			std::list<mail_cache::LabelData*> unpackLabels(const uint64_t& data)const;

			bool loadAttachmentsFromDb(MessageData& m);
        protected:
            bool execQuery(QString sql);
            QSqlQuery* prepareQuery(QString sql);
            QSqlQuery* selectQuery(QString sql);
			bool loadMessagesFromDb();
			bool loadLabelsFromDb();
			bool updateDbLabel(const labels::LabelResource& lbl);			
			LabelData* insertDbLabel(const labels::LabelResource& lbl);
			LabelData* insertDbLabel(QString label_id,
				QString name, 
				QString label_type);
			LabelData* createAndInsertLabel(
				QString label_id,
				QString label_name,
				bool label_is_system,
				uint64_t unread_messages,
				int mask_base
			);
			void insertDbAttachmentData(const MessageData& m);
            std::shared_ptr<MessageData> loadMessageFromDB(QSqlQuery* q);			
        protected:
            bool m_initialized {false};
            QSqlDatabase     m_db;
            std::unique_ptr<QSqlQuery>   m_query{nullptr};
            QString m_db_meta_prefix;
            GMailCache*      m_mem_cache;
			std::map<QString, std::shared_ptr<LabelData>> m_labels;
			std::map<int, std::shared_ptr<LabelData>> m_maskbase2labels;
			std::set<int> m_avail_label_base;
			QString m_downloadDir;
			friend class googleQt::GmailRoutes;
        };//GMailSQLiteStorage

		class MessagesReceiver
		{
		public:
			MessagesReceiver(GmailRoutes& r, EDataState f) :m_r(r), m_msg_format(f) {};
			GoogleTask<messages::MessageResource>* routeSingleBatchRequest(QString message_id);
		protected:
			GmailRoutes&    m_r;
			EDataState      m_msg_format;
		};
        
    };
};
