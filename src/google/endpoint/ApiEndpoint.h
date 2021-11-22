#pragma once

#include <memory>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QEventLoop>
#include <QHttpMultiPart>
#include <QDebug>
#include "ApiException.h"
#include "ApiClient.h"

#ifdef API_QT_AUTOTEST
    #ifndef API_QT_DIAGNOSTICS
        #define API_QT_DIAGNOSTICS
    #endif
#else
    ///this is optional for real application
    #ifndef API_QT_DIAGNOSTICS
        #ifdef _DEBUG
        #define API_QT_DIAGNOSTICS
        #endif
    #endif
#endif

#ifdef API_QT_DIAGNOSTICS
#define ASYNC_TASK_DIAGNOSTICS(S) m_r.endpoint().diagnosticSetRequestTag(S);
#define ASYNC_ROUTE_DIAGNOSTICS(S) endpoint().diagnosticSetRequestTag(S);
#define GQ_TRAIL_LOG(S) {auto o = qWarning();o.noquote(); o << QDateTime::currentDateTime().toString("hh:mm:ss.zzz") << "[gapi/trace]" << S;}
#else
#define ASYNC_TASK_DIAGNOSTICS(S)
#define ASYNC_ROUTE_DIAGNOSTICS(S)
#define GQ_TRAIL_LOG(S)
#endif

#define TIMES_TO_REFRESH_TOKEN_BEFORE_GIVEUP 2

namespace googleQt{
    struct DiagnosticRequestInfo
    {
        QString context;
        QString tag;
        QString request;
    };
    using DGN_LIST = std::vector<DiagnosticRequestInfo>;

    class ApiEndpoint
    {
    public:
        ApiEndpoint(ApiClient* c);
        virtual ~ApiEndpoint();
        void          cancelAll();
        void          runEventsLoop()const;
        void          exitEventsLoop()const;
        bool          isQueryInProgress()const;

        DiagnosticRequestInfo   lastRequestInfo()const;
        const DGN_LIST&         diagnosticRequests()const;
        void                    diagnosticSetRequestTag(QString s);
        void                    diagnosticSetRequestContext(QString s);
        void                    diagnosticClearRequestsList();
        void                    diagnosticLogAsyncTask(EndpointRunnable* task, TaskState s);
        void                    diagnosticLogSQL(QString sql, QString prefix);
        static QString          diagnosticTimeStamp();
        QByteArray    lastResponse()const { return m_last_response; };
        void          setProxy(const QNetworkProxy& proxy);

        ApiClient*     apiClient() { return m_client; }
        const ApiClient*  apiClient()const { return m_client; }
        virtual TaskAggregator* produceAggregatorTask() = 0;

    protected:                
        virtual QNetworkReply*  getData(const QNetworkRequest &request);
        virtual QNetworkReply*  postData(const QNetworkRequest &request, const QByteArray& data);
        virtual QNetworkReply*  postData(const QNetworkRequest &request, QHttpMultiPart* mpart);
        virtual QNetworkReply*  putData(const QNetworkRequest &request, const QByteArray& data);
        virtual QNetworkReply*  deleteData(const QNetworkRequest &request);
        virtual QNetworkReply*  patchData(const QNetworkRequest &request, const QByteArray& data);
        
    protected:
        class requester
        {
        public:
            requester(ApiEndpoint& e);
            virtual ~requester(){};
            
            QNetworkReply * makeRequest(QNetworkRequest& r);

        protected:
            virtual QNetworkReply * request(QNetworkRequest& r) = 0;
            virtual void addAuthHeader(QNetworkRequest& request);
            virtual const char* getHostHeader()const;
        protected:
            ApiEndpoint& m_ep;            
        };
        
        using FINISHED_REQ = std::function<void(std::shared_ptr<requester>,
            QNetworkRequest,
            QNetworkReply *,
            int)>;

        typedef std::unordered_map<QNetworkReply*, std::shared_ptr<FINISHED_REQ>> NET_REPLIES_IN_PROGRESS;

        virtual void            registerReply(std::shared_ptr<requester>& rb, QNetworkReply*, std::shared_ptr<FINISHED_REQ>);
        virtual void            unregisterReply(QNetworkReply*);

        class GET_requester: public requester
        {
        public:
            GET_requester(ApiEndpoint& e):requester(e){}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                return m_ep.getData(r);
            }
        };

        
        
        class DELETE_requester: public requester
        {
        public:
            DELETE_requester(ApiEndpoint& e):requester(e){}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                return m_ep.deleteData(r);
            }
        };

        class POST_requester: public requester
        {
        public:
            POST_requester(ApiEndpoint& e, QJsonObject&& js)
                :requester(e), m_js_out(std::move(js)){}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                QByteArray bytes2post;
                QJsonDocument doc(m_js_out);
                if(m_js_out.isEmpty()){
                    bytes2post.append("null");
                }
                else{
                    QJsonDocument doc(m_js_out);
                    bytes2post = doc.toJson(QJsonDocument::Compact);
                }           
                r.setRawHeader("Content-Type", "application/json; charset=utf-8");
                return m_ep.postData(r, bytes2post);
            }
        protected:
            QJsonObject m_js_out;
        };


        class PUT_requester: public requester
        {
        public:
            PUT_requester(ApiEndpoint& e, QJsonObject&& js)
                :requester(e), m_js_out(std::move(js)){}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                QByteArray bytes2post;
                QJsonDocument doc(m_js_out);
                if(m_js_out.isEmpty()){
                    bytes2post.append("null");
                }
                else{
                    QJsonDocument doc(m_js_out);
                    bytes2post = doc.toJson(QJsonDocument::Compact);
                }           
                r.setRawHeader("Content-Type", "application/json; charset=utf-8");
                return m_ep.putData(r, bytes2post);
            }
        protected:
            QJsonObject m_js_out;
        };

        class UPDATE_requester: public requester
        {
        public:
            UPDATE_requester(ApiEndpoint& e, QJsonObject&& js)
                :requester(e), m_js_out(std::move(js)){}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                QByteArray bytes2post = QByteArray();
                QJsonDocument doc(m_js_out);
                if(m_js_out.isEmpty()){
                    bytes2post.append("null");
                }
                else{
                    QJsonDocument doc(m_js_out);
                    bytes2post = doc.toJson(QJsonDocument::Compact);
                }           
                r.setRawHeader("Content-Type", "application/json; charset=utf-8");
                r.setRawHeader("Content-Length", QString("%1").arg(bytes2post.size()).toStdString().c_str());
                return m_ep.patchData(r, bytes2post);
            }
        protected:
            QJsonObject m_js_out;
        };
        
        class POST_rfc822_requester: public requester
        {
        public:
            POST_rfc822_requester(ApiEndpoint& e, QJsonObject&& js)
                :requester(e), m_js_out(std::move(js)) {}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                QByteArray bytes2post;
                QJsonDocument doc(m_js_out);
                if (m_js_out.isEmpty()) {
                    bytes2post.append("null");
                }
                else {
                    QJsonDocument doc(m_js_out);
                    bytes2post = doc.toJson(QJsonDocument::Compact);
                }
                r.setRawHeader("Content-Type", "message/rfc822");
                r.setRawHeader("Content-Length", QString("%1").arg(bytes2post.size()).toStdString().c_str());
                return m_ep.postData(r, bytes2post);
            }
        protected:
            QJsonObject m_js_out;
        };


        class MPartUpload_requester : public requester
        {
        public:
            MPartUpload_requester(ApiEndpoint& e, QJsonObject&& js, QIODevice* readFrom)
                :requester(e), m_js_out(std::move(js)), m_readFrom(readFrom) {}
            QNetworkReply * request(QNetworkRequest& r)override;
        protected:
            QJsonObject m_js_out;
            QIODevice* m_readFrom;
        };

        
        template <class R>
        class DOWNLOAD_base_requester : public R
        {
        public:
            DOWNLOAD_base_requester(ApiEndpoint& e, QIODevice* writeTo) 
                :R(e), m_writeTo(writeTo){}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                QNetworkReply* reply = R::m_ep.getData(r);
                if (reply != nullptr) {
                    QObject::connect(reply, &QNetworkReply::readyRead, [=]()
                    {
                        qint64 sz = reply->bytesAvailable();
                        if (sz > 0 && m_writeTo != NULL) {
                            QByteArray data = reply->read(sz);
                            m_writeTo->write(data);
                        }
                    });
                }
                return reply;
            }
        protected:
            QIODevice* m_writeTo;
        };

        template <class R>
        class UPLOAD_base_requester : public R
        {
        public:
            UPLOAD_base_requester(ApiEndpoint& e, QIODevice* readFrom)
                :R(e), m_readFrom(readFrom) {}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                r.setRawHeader("Content-Type", "application/octet-stream");
                r.setRawHeader("Content-Length", QString("%1").arg(m_readFrom ? m_readFrom->size() : 0).toStdString().c_str());
                return R::m_ep.postData(r, m_readFrom ? m_readFrom->readAll() : QByteArray());
            }
        protected:
            QIODevice* m_readFrom;
        };
        
        class DOWNLOAD_requester : public DOWNLOAD_base_requester<requester>
        {
        public:
            DOWNLOAD_requester(ApiEndpoint& e, QIODevice* writeTo):
                DOWNLOAD_base_requester<requester>(e, writeTo){};
        };

        class UPLOAD_requester : public UPLOAD_base_requester<requester>
        {
        public:
            UPLOAD_requester(ApiEndpoint& e, QIODevice* readFrom):
                UPLOAD_base_requester<requester>(e, readFrom){};
        };

        
        ///BEGIN contacts requesters
        class contact_requester : public requester 
        {
        public:
            contact_requester(ApiEndpoint& e);
        protected:
            const char* getHostHeader()const override;
        };

        class GET_requester4Contact : public contact_requester
        {
        public:
            GET_requester4Contact(ApiEndpoint& e) :contact_requester(e) {}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                r.setRawHeader("GData-Version", "3.0");
                return m_ep.getData(r);
            }
        };

        class POST_requester4Contact : public contact_requester
        {
        public:
            POST_requester4Contact(ApiEndpoint& e, QString&& xml)
                :contact_requester(e), m_xml(std::move(xml)) {}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                QByteArray bytes2post(m_xml.toStdString().c_str());
                r.setRawHeader("Content-Type", "application/atom+xml");
                r.setRawHeader("GData-Version", "3.0");
                return m_ep.postData(r, bytes2post);
            }
        protected:
            QString m_xml;
        };

        class PUT_requester4Contact : public contact_requester
        {
        public:
            PUT_requester4Contact(ApiEndpoint& e, QString&& etag, QString&& xml)
                :contact_requester(e), m_etag(std::move(etag)), m_xml(std::move(xml)) {}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                QByteArray bytes2post(m_xml.toStdString().c_str());
                r.setRawHeader("If-Match", m_etag.toStdString().c_str());
                r.setRawHeader("GData-Version", "3.0");
                r.setRawHeader("Content-Type", "application/atom+xml");
                return m_ep.putData(r, bytes2post);
            }
        protected:
            QString m_etag;
            QString m_xml;
        };


        class DELETE_requester4Contact : public contact_requester
        {
        public:
            DELETE_requester4Contact(ApiEndpoint& e, QString&& etag)
                :contact_requester(e), m_etag(std::move(etag)) {}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                r.setRawHeader("If-Match", m_etag.toStdString().c_str());
                return m_ep.deleteData(r);
            }
        protected:
            QString m_etag;
        };

        class DOWNLOAD_requester4Contact : public DOWNLOAD_base_requester<contact_requester>
        {
        public:
            DOWNLOAD_requester4Contact(ApiEndpoint& e, QIODevice* writeTo):
                DOWNLOAD_base_requester<contact_requester>(e, writeTo){};
        };

        class UPLOAD_photo_requester4Contact : public UPLOAD_base_requester<contact_requester>
        {
        public:
            UPLOAD_photo_requester4Contact(ApiEndpoint& e, QIODevice* readFrom, QString&& etag):
                UPLOAD_base_requester<contact_requester>(e, readFrom), m_etag(std::move(etag)){};

            QNetworkReply * request(QNetworkRequest& r)override
            {
                r.setRawHeader("If-Match", m_etag.toStdString().c_str());
                r.setRawHeader("Content-Type", "image/*");
                r.setRawHeader("Content-Length", QString("%1").arg(m_readFrom ? m_readFrom->size() : 0).toStdString().c_str());
                return m_ep.putData(r, m_readFrom ? m_readFrom->readAll() : QByteArray());
            }
        protected:
            QString m_etag;            
        };        
    ///END contacts

    protected:
        void                    updateLastRequestInfo(QString s);
        void                    abortRequests();
    protected:
        QNetworkAccessManager m_con_mgr;
        mutable QEventLoop    m_loop;
        ApiClient*            m_client;
        NET_REPLIES_IN_PROGRESS m_replies_in_progress;
        QByteArray            m_last_response;
#ifdef API_QT_DIAGNOSTICS
        DGN_LIST                m_requests;
        QString                 m_diagnosticsRequestTag;
        QString                 m_diagnosticsRequestContext;
#endif //API_QT_DIAGNOSTICS
    };
}
