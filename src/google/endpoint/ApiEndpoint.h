#pragma once

#include <memory>
#include <map>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QEventLoop>
#include <QHttpMultiPart>
#include "ApiException.h"
#include "ApiClient.h"

#define TIMES_TO_REFRESH_TOKEN_BEFORE_GIVEUP 2

namespace googleQt{
    class ApiEndpoint
    {
    public:
        ApiEndpoint(ApiClient* c);
        void          cancelAll();
        void          runEventsLoop()const;
        void          exitEventsLoop()const;

        QString       lastRequestInfo()const { return m_last_req_info; }
        QByteArray    lastResponse()const { return m_last_response; };
        void          setProxy(const QNetworkProxy& proxy);

        ApiClient*     apiClient() { return m_client; }
        const ApiClient*  apiClient()const { return m_client; }

    protected:
        virtual void addAuthHeader(QNetworkRequest& request);
        
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
            requester(ApiEndpoint& e):m_ep(e){}
            virtual QNetworkReply * request(QNetworkRequest& r) = 0;
        protected:
            ApiEndpoint& m_ep;            
        };
        
        using FINISHED_REQ = std::function<void(std::shared_ptr<requester>,
            QNetworkRequest,
            QNetworkReply *,
            int)>;

        typedef std::map<QNetworkReply*, std::shared_ptr<FINISHED_REQ>> NET_REPLIES_IN_PROGRESS;

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

        class GET_requester4Contact : public requester
        {
        public:
            GET_requester4Contact(ApiEndpoint& e) :requester(e) {}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                r.setRawHeader("GData-Version", "3.0");
                return m_ep.getData(r);
            }
        };

        class DELETE_requester4Contact: public requester
        {
        public:
            DELETE_requester4Contact(ApiEndpoint& e, QString&& xml)
                :requester(e), m_etag(std::move(xml)){}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                r.setRawHeader("If-Match", m_etag.toStdString().c_str());
                return m_ep.deleteData(r);
            }
        protected:
            QString m_etag;
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

        class POST_requester4Contact : public requester
        {
        public:
            POST_requester4Contact(ApiEndpoint& e, QString&& xml)
                :requester(e), m_xml(std::move(xml)) {}
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

        class SimpleUpload_requester : public requester
        {
        public:
            SimpleUpload_requester(ApiEndpoint& e, QIODevice* readFrom)
                :requester(e), m_readFrom(readFrom) {}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                r.setRawHeader("Content-Type", "application/octet-stream");
                r.setRawHeader("Content-Length", QString("%1").arg(m_readFrom ? m_readFrom->size() : 0).toStdString().c_str());
                return m_ep.postData(r, m_readFrom ? m_readFrom->readAll() : QByteArray());
            }
        protected:
            QIODevice* m_readFrom;
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

        class DOWNLOAD_requester : public requester
        {
        public:
            DOWNLOAD_requester(ApiEndpoint& e, QIODevice* writeTo) 
                :requester(e), m_writeTo(writeTo){}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                QNetworkReply* reply = m_ep.getData(r);
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

    protected:
        void                  updateLastRequestInfo(QString s);
        
    protected:
        QNetworkAccessManager m_con_mgr;
        mutable QEventLoop    m_loop;
        ApiClient*            m_client;
        NET_REPLIES_IN_PROGRESS m_replies_in_progress;
        QString               m_last_req_info;
        QByteArray            m_last_response;
    };
}
