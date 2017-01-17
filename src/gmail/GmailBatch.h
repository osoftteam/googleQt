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
        class MesagesReciever
        {
        public:
            MesagesReciever(GmailRoutes& r, EDataState f) :m_r(r), m_msg_format(f){};
            GoogleTask<messages::MessageResource>* route(QString message_id);
        protected:
            GmailRoutes&    m_r;
            EDataState      m_msg_format;
        };

        class GMailCacheQueryResult;
        class GMailSQLiteStorage;
        class MessageData : public CacheData
        {
            friend class GMailCacheQueryResult;
            friend class GMailSQLiteStorage;
        public:
			MessageData(QString id, QString from, QString subject, QString snippet);
			MessageData(QString id, QString from, QString subject, QString snippet, QString plain, QString html);

            void  merge(CacheData* other);
            
            QString from()const { return m_from; }
            QString subject()const{return m_subject;}
            QString snippet()const { return m_snippet; }
            QString plain()const { return m_plain; }
            QString html()const { return m_html; }
#ifdef API_QT_AUTOTEST
            static std::unique_ptr<MessageData> EXAMPLE(EDataState st);
#endif //API_QT_AUTOTEST
        protected:
			void updateSnippet(QString from, QString subject, QString snippet);
            void updateBody(QString plain, QString html);
        protected:
            QString m_from;
            QString m_subject;
            QString m_snippet;
            QString m_plain;
            QString m_html;
        private:
            MessageData(int agg_state, QString id, QString from, QString subject, QString snippet, QString plain, QString html);
        };

		class GMailCache;

        class GMailCacheQueryResult: public CacheQueryResult<MessageData>
        {
        public:
            GMailCacheQueryResult(EDataState load, ApiEndpoint& ept, GmailRoutes& r, GMailCache* c);
            void fetchFromCloud_Async(const std::list<QString>& id_list)override;		
        protected:
            void fetchMessage(messages::MessageResource* m);
			void loadHeaders(messages::MessageResource* m, QString& from, QString& subject);
        protected:
            GmailRoutes&  m_r;
        };
        
        class GMailCache : public GoogleCache<MessageData, GMailCacheQueryResult>
        {
        public:
            GMailCache(ApiEndpoint& ept, GmailRoutes& r);
            std::unique_ptr<GMailCacheQueryResult> produceCloudResultFetcher(EDataState load, ApiEndpoint& ept)override;
        protected:
            GmailRoutes&    m_r;
        };

        class GMailSQLiteStorage: public LocalPersistentStorage<MessageData, GMailCacheQueryResult>
        {
        public:
            GMailSQLiteStorage(GMailCache* c):m_mem_cache(c){};
            bool init(QString dbPath, QString dbName, QString db_meta_prefix);
            std::list<QString> load(EDataState state, const std::list<QString>& id_list,
                                    std::unique_ptr<GMailCacheQueryResult>& cr)override;
            void update(EDataState state, CACHE_QUERY_RESULT_LIST<MessageData>& r)override;
            bool isValid()const override{return m_initialized;};
            void remove(const std::list<QString>& ids2remove)override;
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
