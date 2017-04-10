#pragma once

#include "google/endpoint/BatchRunner.h"
#include "google/endpoint/Cache.h"
#include "gmail/messages/MessagesRoutes.h"
#include <QSqlDatabase>
#include <QSqlQuery>

namespace googleQt{
	class Endpoint;
	class GmailRoutes;

	namespace mail_batch{
        class MessagesReceiver
        {
        public:
            MessagesReceiver(GmailRoutes& r, EDataState f) :m_r(r), m_msg_format(f){};
            GoogleTask<messages::MessageResource>* route(QString message_id);
        protected:
            GmailRoutes&    m_r;
            EDataState      m_msg_format;
        };

        class GMailCacheQueryResult;
        class GMailSQLiteStorage;
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
                        qlonglong standardLabels);
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
                        qlonglong standardLabels);

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

            bool  isSpam()const { return (m_standardLabels.SPAM == 1); }
            bool  isTrash()const { return (m_standardLabels.TRASH == 1); }
            bool  isUnread()const { return (m_standardLabels.UNREAD == 1); }
            bool  isStarred()const { return (m_standardLabels.STARRED == 1); }
            bool  isImportant()const { return (m_standardLabels.IMPORTANT == 1); }

            bool inLabelFilter(uint64_t data)const;
            
            uint64_t stardardLabelsFlags()const{return m_standardLabels.flags;}
            std::list<QString> getLabels()const;
            
            static uint64_t packStardardLabels(const std::list <QString>& labels);
            static uint64_t labelMask(QString name);
        protected:
			void updateSnippet(QString from,
                               QString to,
                               QString cc,
                               QString bcc,
                               QString subject,
                               QString snippet,
                               qlonglong standardLabels);
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

			union LABEL_FLAGS
			{
				uint64_t flags;
				struct                
				{
                    unsigned IMPORTANT	: 1;
                    unsigned CHAT       : 1;
                    unsigned SENT       : 1;
					unsigned INBOX		: 1;
                    unsigned TRASH		: 1;
                    unsigned DRAFT      : 1;                    
					unsigned SPAM		: 1;
					unsigned STARRED	: 1;
					unsigned UNREAD		: 1;
                    unsigned CATEGORY_PERSONAL    :1;
                    unsigned CATEGORY_SOCIAL      :1;
                    unsigned CATEGORY_FORUMS      :1;
                    unsigned CATEGORY_UPDATES     :1;
                    unsigned CATEGORY_PROMOTIONS  :1;
				};
			} m_standardLabels;

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
                        qlonglong standardLabels);

			friend class GMailCacheQueryResult;
			friend class GMailSQLiteStorage;
			friend class googleQt::GmailRoutes;
        };

        struct MessagesList
        {
            std::list<std::shared_ptr<mail_batch::MessageData>> messages;
            std::map<QString, std::shared_ptr<mail_batch::MessageData>> messages_map;
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
            void loadLabels(messages::MessageResource* m, uint64_t& standardLabels);
        protected:
            GmailRoutes&  m_r;
        };
        
        class GMailCache : public GoogleCache<MessageData, GMailCacheQueryResult>
        {
        public:
            GMailCache(ApiEndpoint& ept);
            //void persistent_clear(const std::set<QString>& ids2delete) override;
        };

        class GMailSQLiteStorage: public LocalPersistentStorage<MessageData, GMailCacheQueryResult>
        {
        public:
            GMailSQLiteStorage(GMailCache* c):m_mem_cache(c){};
            bool init(QString dbPath, QString dbName, QString db_meta_prefix);
            std::list<QString> load(EDataState state, const std::list<QString>& id_list,
                                    GMailCacheQueryResult* cr)override;
            void update(EDataState state, CACHE_QUERY_RESULT_LIST<MessageData>& r)override;
            bool isValid()const override{return m_initialized;};
            void remove(const std::set<QString>& ids2remove)override;
            void updateStandardLabels(QString msg_id, uint64_t flags)override;

        protected:
            bool execQuery(QString sql);
            QSqlQuery* prepareQuery(QString sql);
            QSqlQuery* selectQuery(QString sql);
            std::shared_ptr<MessageData> loadObjFromDB(QSqlQuery* q);
        protected:
            bool m_initialized {false};
            QSqlDatabase     m_db;
            std::unique_ptr<QSqlQuery>   m_query{nullptr};
            QString m_db_meta_prefix;
            GMailCache*      m_mem_cache;
        };//GMailSQLiteStorage
    };
};
