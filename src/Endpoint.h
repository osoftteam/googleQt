#pragma once

#include <iostream>
#include <QUrlQuery>
#include "google/endpoint/ApiEndpoint.h"
#include "gmail/GmailRequestArg.h"
#include "google/endpoint/GoogleHost.h"
#include "google/endpoint/ApiException.h"
#include "GoogleClient.h"
#include "gmail/errors/ErrorsErrorInfo.h"

namespace googleQt{
    class Endpoint: public googleQt::ApiEndpoint
    {
    public:
        Endpoint(googleQt::ApiClient* c);

        template <class RES, class RESULT_FACTORY, class BODY>
        RES getStyle(QUrl url)
        {
            GET_requester rb(*this);
            return runRequest<RES, RESULT_FACTORY, GET_requester>(url, rb);
        }

        template <class RES, class RESULT_FACTORY, class BODY>
        RES postStyle(QUrl url, const BODY& body)
        {
            QJsonObject js = body;
            POST_requester rb(*this, js);
            return runRequest<RES, RESULT_FACTORY, POST_requester>(url, rb);
        }

		template <class RES, class RESULT_FACTORY, class BODY>
		RES postStyle(QUrl url)
		{
			QJsonObject js = QJsonObject();
			POST_requester rb(*this, js);
			return runRequest<RES, RESULT_FACTORY, POST_requester>(url, rb);
		}

        template <class RES, class RESULT_FACTORY, class BODY>
        RES putStyle(QUrl url, const BODY& body)
        {
            QJsonObject js = body;
            PUT_requester rb(*this, js);
            return runRequest<RES, RESULT_FACTORY, PUT_requester>(url, rb);
        }
        
        template <class RES, class RESULT_FACTORY, class BODY>
        RES deleteStyle(QUrl url)
        {
            DELETE_requester rb(*this);
            return runRequest<RES, RESULT_FACTORY, DELETE_requester>(url, rb);
        }

		template <class RES, class RESULT_FACTORY, class BODY>
		RES uploadStyle(QUrl url, QIODevice* readFrom)
		{
			UPLOAD_requester rb(*this, readFrom);
			return runRequest<RES, RESULT_FACTORY, UPLOAD_requester>(url, rb);
		}

		/*
		template <class RES, class RESULT_FACTORY, class BODY>
		RES simpleUploadStyle(QUrl url, const BODY& body)
		{
			QJsonObject js = body;
			POST_requester rb(*this, js);
			return runRequest<RES, RESULT_FACTORY, POST_requester>(path, rb);
		}*/

		template <class RES, class RESULT_FACTORY, class BODY>
		RES rfc822UploadStyle(QUrl url, const BODY& body)
		{
			QJsonObject js = body;
			POST_rfc822_requester rb(*this, js);
			return runRequest<RES, RESULT_FACTORY, POST_rfc822_requester>(url, rb);
		}

		template <class ARG>
		QUrl buildGmailUrl(const QString& namespace_path, const ARG& a)const
		{
			QUrl url;
			a.build(QString("https://www.googleapis.com/gmail/v1/users/%1/%2").arg(client()->userId()).arg(namespace_path), url);
			return url;
		}

		template <class ARG>
		QUrl buildGmailUploadlUrl(const QString& namespace_path, const ARG& a)const
		{
			QUrl url;
			a.build(QString("https://www.googleapis.com/upload/gmail/v1/users/%1/%2").arg(client()->userId()).arg(namespace_path), url);
			return url;
		}

		template <class ARG>
		QUrl buildGtasklistUrl(const QString& , const ARG& a)const
		{
			QUrl url;
			a.build("https://www.googleapis.com/tasks/v1/users/@me/lists", url);
			return url;
		}

		template <class ARG>
		QUrl buildGtaskUrl(const QString&, const ARG& a)const
		{
			QUrl url;
			a.build("https://www.googleapis.com/tasks/v1/lists", url);
			addAppKeyParameter(url);
/*
			if (url.hasQuery())
			{
				QUrlQuery q(url);
				q.addQueryItem("key", client()->getAppKey());
				url.setQuery(q);
			}
			else 
			{
				QUrlQuery q;
				q.addQueryItem("key", client()->getAppKey());
				url.setQuery(q);
			}
			*/
			return url;
		}

		template <class ARG>
		QUrl buildGdriveUrl(const QString&, const ARG& a)const
		{
			QUrl url;
			a.build("https://www.googleapis.com/drive/v3", url);
			addAppKeyParameter(url);
			return url;
		}

		template <class ARG>
		QUrl buildGdriveUploadUrl(const QString&, const ARG& a)const
		{
			QUrl url;
			a.build("https://www.googleapis.com/upload/drive/v3/", url);
			addAppKeyParameter(url);
			return url;
		}


        virtual void onErrorUnauthorized(const errors::ErrorInfo* er);

        GoogleClient* client();
        const GoogleClient* client()const;

	protected:
		QString prepareErrorInfo(int status_code, const QUrl& url, const QByteArray& data);
		void addAppKeyParameter(QUrl& url)const;

		template <class RES, class RESULT_FACTORY, class REQUEST_BUILDER>
		RES runRequest(QUrl url, REQUEST_BUILDER& rb)
		{
			QNetworkRequest req(url);
			addAuthHeader(req);
			bool ok = false;
			QByteArray exception_data;
			std::string errorInfo;
			int status_code = 0;
			RES res;
			RESULT_FACTORY factory;

			int reauthLimit = 1;
			bool unhandled_error = false;
			int serverReqCount = 0;
			while (!ok && !unhandled_error)
			{
				QNetworkReply *reply = rb.request(req);
				if (!reply)return res;
				serverReqCount++;
				QObject::connect(reply, &QNetworkReply::downloadProgress, [&](qint64 bytesProcessed, qint64 total) {
					emit m_client->progress(bytesProcessed, total);
				});
				QObject::connect(reply, &QNetworkReply::uploadProgress, [&](qint64 bytesProcessed, qint64 total) {
					emit m_client->progress(bytesProcessed, total);
				});

				QObject::connect(reply, &QNetworkReply::finished, [&]()
				{
					status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
					switch (status_code)
					{
					case 200:
					{
						QByteArray data = reply->readAll();
						if (!data.isEmpty())
						{
							res = std::move(factory.create(data));
							ok = true;
						}
					}break;
					case 202:
					case 204:
					{
						ok = true;
					}break;
					case 401:
					{
						QByteArray data = reply->readAll();
						if (reauthLimit > 0)
						{
							std::unique_ptr<errors::ErrorInfo> er = errors::ErrorInfo::factory::create(data);
							onErrorUnauthorized(er.get());
							addAuthHeader(req);
							reauthLimit--;
						}
						else
						{
							unhandled_error = true;
							errorInfo = prepareErrorInfo(status_code, url, data).toStdString();
						}
					}break;
					default:
					{
						QByteArray data = reply->readAll();
						unhandled_error = true;
						errorInfo = prepareErrorInfo(status_code, url, data).toStdString();
					}break;
					}
					exitEventLoop(reply);
				});
				execEventLoop(reply);
			}//while
			if (!ok)
			{
				throw googleQt::ReplyException(errorInfo, status_code, "");
			}
			return res;
		}//runRequest

    };    
};
