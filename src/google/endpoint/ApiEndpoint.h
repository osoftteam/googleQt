#pragma once

#include <memory>
#include <map>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonObject>
#include <QEventLoop>
#include "ApiException.h"
#include "ApiClient.h"

#define TIMES_TO_REFRESH_TOKEN_BEFORE_GIVEUP 2

namespace googleQt{
    class ApiEndpoint
    {
    public:
        ApiEndpoint(ApiClient* c);
        QString       lastRequestInfo()const{return m_last_req_info;}
        void          cancelAll();
        void          runEventsLoop()const;
        void          exitEventsLoop()const;

    protected:
        virtual void addAuthHeader(QNetworkRequest& request);
        
        virtual QNetworkReply*  getData(const QNetworkRequest &request);
        virtual QNetworkReply*  postData(const QNetworkRequest &request, const QByteArray& data);
        virtual QNetworkReply*  putData(const QNetworkRequest &request, const QByteArray& data);
        virtual QNetworkReply*  deleteData(const QNetworkRequest &request);

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

        virtual void            registerReply(QNetworkReply*, std::shared_ptr<FINISHED_REQ>);
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
            POST_requester(ApiEndpoint& e, const QJsonObject& js)
                :requester(e), m_js_out(js){}
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
            const QJsonObject& m_js_out;
        };

        class PUT_requester: public requester
        {
        public:
            PUT_requester(ApiEndpoint& e, const QJsonObject& js)
                :requester(e), m_js_out(js){}
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
            const QJsonObject& m_js_out;
        };
        
        class POST_rfc822_requester: public requester
        {
        public:
            POST_rfc822_requester(ApiEndpoint& e, const QJsonObject& js)
                :requester(e), m_js_out(js) {}
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
            const QJsonObject& m_js_out;
        };

        class UPLOAD_requester: public requester
        {
        public:
            UPLOAD_requester(ApiEndpoint& e, QIODevice* readFrom)
                :requester(e), m_readFrom(readFrom){}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                qint64 fsize = m_readFrom ? m_readFrom->size() : 0;
                r.setRawHeader("Content-Type", "application/octet-stream");
                r.setRawHeader("Content-Length", QString("%1").arg(fsize).toStdString().c_str());
                return m_ep.postData(r, m_readFrom ? m_readFrom->readAll() : QByteArray());
            }
        protected:
            QIODevice* m_readFrom;
        };

    protected:
        void                  updateLastRequestInfo(QString s);
        
    protected:
        QNetworkAccessManager m_con_mgr;
        mutable QEventLoop    m_loop;
        ApiClient*            m_client;
        NET_REPLIES_IN_PROGRESS m_replies_in_progress;
        QString               m_last_req_info;
    };
}
