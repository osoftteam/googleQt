#pragma once

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include <QJsonObject>
#include <QEventLoop>
#include "ApiException.h"
#include "ApiClient.h"

namespace googleQt{
    class ApiEndpoint
    {
        typedef std::set<QNetworkReply*> NET_REPLIES_IN_PROGRESS;
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

        virtual void            registerReply(QNetworkReply*);
        virtual void            unregisterReply(QNetworkReply*);
    protected:
        class GET_requester
        {
        public:
            GET_requester(ApiEndpoint& e):m_ep(e){}
            QNetworkReply * request(QNetworkRequest& r)
            {
                return m_ep.getData(r);
            }
        protected:
            ApiEndpoint& m_ep;
        };

        class DELETE_requester
        {
        public:
            DELETE_requester(ApiEndpoint& e):m_ep(e){}
            QNetworkReply * request(QNetworkRequest& r)
            {
                return m_ep.deleteData(r);
            }
        protected:
            ApiEndpoint& m_ep;
        };

        class POST_requester
        {
        public:
            POST_requester(ApiEndpoint& e, const QJsonObject& js):m_ep(e), m_js_out(js){}
            QNetworkReply * request(QNetworkRequest& r)
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
            ApiEndpoint& m_ep;
            const QJsonObject& m_js_out;
        };

        class PUT_requester
        {
        public:
            PUT_requester(ApiEndpoint& e, const QJsonObject& js):m_ep(e), m_js_out(js){}
            QNetworkReply * request(QNetworkRequest& r)
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
            ApiEndpoint& m_ep;
            const QJsonObject& m_js_out;
        };
        
        class POST_rfc822_requester
        {
        public:
            POST_rfc822_requester(ApiEndpoint& e, const QJsonObject& js) :m_ep(e), m_js_out(js) {}
            QNetworkReply * request(QNetworkRequest& r)
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
            ApiEndpoint& m_ep;
            const QJsonObject& m_js_out;
        };

        class UPLOAD_requester 
        {
        public:
            UPLOAD_requester(ApiEndpoint& e, QIODevice* readFrom) :m_ep(e), m_readFrom(readFrom){}
            QNetworkReply * request(QNetworkRequest& r)
            {
                qint64 fsize = m_readFrom ? m_readFrom->size() : 0;
                r.setRawHeader("Content-Type", "application/octet-stream");
                r.setRawHeader("Content-Length", QString("%1").arg(fsize).toStdString().c_str());
                return m_ep.postData(r, m_readFrom ? m_readFrom->readAll() : QByteArray());
            }
        protected:
            ApiEndpoint& m_ep;
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
