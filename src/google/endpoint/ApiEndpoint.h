#pragma once

#include <memory>
#include <map>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
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
		QByteArray	  lastResponse()const { return m_last_response; };
    protected:
        virtual void addAuthHeader(QNetworkRequest& request);
        
        virtual QNetworkReply*  getData(const QNetworkRequest &request);
        virtual QNetworkReply*  postData(const QNetworkRequest &request, const QByteArray& data);
		virtual QNetworkReply*  postData(const QNetworkRequest &request, QHttpMultiPart* mpart);
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

		//.........
		class SimpleUpload_requester : public requester
		{
		public:
			SimpleUpload_requester(ApiEndpoint& e, QIODevice* readFrom)
				:requester(e), m_readFrom(readFrom) {}
			QNetworkReply * request(QNetworkRequest& r)override
			{
				r.setRawHeader("Content-Type", "application/octet-stream");
				r.setRawHeader("Content-Length", QString("%1").arg(m_readFrom ? 0 : m_readFrom->size()).toStdString().c_str());
				return m_ep.postData(r, m_readFrom ? m_readFrom->readAll() : QByteArray());
			}
		protected:
			QIODevice* m_readFrom;
		};
		///........

        class MPartUpload_requester : public requester
        {
        public:
            MPartUpload_requester(ApiEndpoint& e, const QJsonObject& js, QIODevice* readFrom)
                :requester(e), m_js_out(js), m_readFrom(readFrom) {}
            QNetworkReply * request(QNetworkRequest& r)override
            {
                QByteArray meta_bytes;
                QJsonDocument doc(m_js_out);
                if (m_js_out.isEmpty()) {
                    meta_bytes.append("null");
                }
                else {
                    QJsonDocument doc(m_js_out);
                    meta_bytes = doc.toJson(QJsonDocument::Compact);
                }
                /*
				QHttpMultiPart *mpart = new QHttpMultiPart(QHttpMultiPart::RelatedType);

				QHttpPart metaPart;
				metaPart.setRawHeader("Content-Type", "application/json; charset = UTF-8");
				metaPart.setBody(meta_bytes);

				QHttpPart dataPart;
				dataPart.setRawHeader("Content-Type", "application/octet-stream");
				dataPart.setBodyDevice(m_readFrom);

				mpart->append(metaPart);
				mpart->append(dataPart);

				QNetworkReply* reply = m_ep.postData(r, mpart);
				mpart->setParent(reply);
				return reply;
                */
				
                QString delimiter("foo_bar_baz12321");
                QByteArray bytes2post = QString("\n--%1\n").arg(delimiter).toStdString().c_str();
                bytes2post += QString("Content - Type: application/json; charset = UTF-8\n").toStdString().c_str();             
                //bytes2post += "\n";
                bytes2post += meta_bytes;
                
                bytes2post += "\n";
                bytes2post += "\n";
                bytes2post += QString("--%1\n").arg(delimiter).toStdString().c_str();
                bytes2post += QString("Content-Type: application/octet-stream").toStdString().c_str();
                bytes2post += "\n";
                if (m_readFrom) {
                    bytes2post += m_readFrom->readAll();
                }
                bytes2post += "\n";
                bytes2post += QString("--%1--").arg(delimiter).toStdString().c_str();

                QString content_str = QString("multipart/related; boundary=%1").arg(delimiter);
                r.setRawHeader("Content-Type", content_str.toStdString().c_str());
                r.setRawHeader("Content-Length", QString("%1").arg(bytes2post.size()).toStdString().c_str());
                return m_ep.postData(r, bytes2post);				
            }
        protected:
            const QJsonObject& m_js_out;
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
		QByteArray			  m_last_response;
    };
}
