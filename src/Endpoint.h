#pragma once

#include <iostream>
#include <functional>
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

        template <class RES, 
            class RESULT_FACTORY>
        void getStyle(QUrl url,
            std::function<void(RES)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            GET_requester rb(*this);
            runRequest<RES, 
                RESULT_FACTORY, 
                GET_requester>(url, 
                    rb,
                    completed_callback,
                    failed_callback);
        }

        template <class RES, 
            class RESULT_FACTORY, 
            class BODY>
        void postStyle(QUrl url, 
            const BODY& body,
            std::function<void(RES)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            QJsonObject js = body;
            POST_requester rb(*this, js);
            runRequest<RES, 
                RESULT_FACTORY, 
                POST_requester>(url, rb,
                    completed_callback,
                    failed_callback);
        }

        template <class RES, 
            class RESULT_FACTORY>
        void postStyle(QUrl url,
            std::function<void(RES)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            QJsonObject js = QJsonObject();
            POST_requester rb(*this, js);
            runRequest<RES, 
                RESULT_FACTORY, 
                POST_requester>(url, rb,
                    completed_callback,
                    failed_callback);
        }

		void postStyle(QUrl url,
			std::function<void(void)> completed_callback = nullptr,
			std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
		{
			QJsonObject js = QJsonObject();
			std::function<void(std::unique_ptr<googleQt::VoidType>)> completed_with_type = nullptr;
			if (completed_callback != nullptr)
			{
				completed_with_type = [=](std::unique_ptr<googleQt::VoidType>)
				{
					completed_callback();
				};
			}

			POST_requester rb(*this, js);
			runRequest<std::unique_ptr<VoidType>,
				VoidType,
				POST_requester>(url, rb,
					completed_with_type,
					failed_callback);
		}

        template <class RES, class RESULT_FACTORY, class BODY>
        void putStyle(QUrl url, const BODY& body,
            std::function<void(RES)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            QJsonObject js = body;
            PUT_requester rb(*this, js);
            runRequest<RES, 
                RESULT_FACTORY, 
                PUT_requester>(url, rb,
                    completed_callback,
                    failed_callback);
        }
        
        void deleteStyle(QUrl url,
            std::function<void(void)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
			std::function<void(std::unique_ptr<googleQt::VoidType>)> completed_with_type = nullptr;
			if (completed_callback != nullptr)
			{
				completed_with_type = [=](std::unique_ptr<googleQt::VoidType>)
				{
					completed_callback();
				};
			}


            DELETE_requester rb(*this);
            runRequest<std::unique_ptr<VoidType>,
				VoidType,
                DELETE_requester>(url, rb,
					completed_with_type,
                    failed_callback);
					
        }

        template <class RES, class RESULT_FACTORY>
        void uploadStyle(QUrl url, QIODevice* readFrom,
			std::function<void(RES)> completed_callback = nullptr,
			std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            UPLOAD_requester rb(*this, readFrom);
            runRequest<RES, 
                RESULT_FACTORY, 
                UPLOAD_requester>(url, rb,
                    completed_callback,
                    failed_callback);
        }

        template <class RES, class RESULT_FACTORY, class BODY>
        void rfc822UploadStyle(QUrl url, const BODY& body,
			std::function<void(RES)> completed_callback = nullptr,
			std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            QJsonObject js = body;
            POST_rfc822_requester rb(*this, js);
            runRequest<RES, 
                RESULT_FACTORY, 
                POST_rfc822_requester>(url, rb,
                    completed_callback,
                    failed_callback);
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

        template <class RES, 
                class RESULT_FACTORY, 
                class REQUEST_BUILDER>
        void runRequest(QUrl url, 
                        REQUEST_BUILDER& rb,
            std::function<void(RES)> completed_callback = nullptr, 
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            QNetworkRequest req(url);
            addAuthHeader(req);
            bool completed_flag = false;
            RESULT_FACTORY factory;

            int reauthLimit = 1;
            bool unhandled_error = false;
            while (!completed_flag && !unhandled_error)
            {
                static int Z = 0;
                std::cout << "h:" << Z++ << " limit:" << reauthLimit << std::endl;
                if(Z > 10){
                    return;//spinning
                }
                QNetworkReply *reply = rb.request(req);
				if (!reply)
				{
#ifdef API_QT_AUTOTEST
					if (completed_callback != nullptr)
					{
						completed_callback(factory.create(QByteArray()));
					}
#else
					if (failed_callback != nullptr)
					{
						std::string errorInfo = prepareErrorInfo(0, url, QByteArray()).toStdString();
						std::unique_ptr<GoogleException> ex(new GoogleException(errorInfo,
							0,
							"Failed to execure network request. No reply object was created."));
						failed_callback(std::move(ex));
					}
#endif //API_QT_AUTOTEST
					return;
				}
                QObject::connect(reply, &QNetworkReply::finished, [&]()
                {
                    int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                    switch (status_code)
                    {
                    case 200:
                    {
                        if (completed_callback != nullptr)
                        {
                            QByteArray data = reply->readAll();
                            completed_callback(factory.create(data));
                        }
                        completed_flag = true;
                    }break;
                    case 202:
                    case 204:
                    {
                        if (completed_callback != nullptr) 
                        {
                            completed_callback(factory.create(QByteArray()));
                        }
                        completed_flag = true;
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
                            if (failed_callback != nullptr)
                            {
                                std::string errorInfo = prepareErrorInfo(status_code, url, data).toStdString();
                                std::unique_ptr<GoogleException> ex(new GoogleException(errorInfo,
                                    status_code,
                                    ""));
                                failed_callback(std::move(ex));
                            }
                        }
                    }break;
                    default:
                    {
                        unhandled_error = true;
                        if (failed_callback != nullptr) {
                            QByteArray exception_data = reply->readAll();
                            std::string errorInfo = prepareErrorInfo(status_code,
                                url,
                                exception_data).toStdString();
                            std::unique_ptr<GoogleException> ex(new GoogleException(errorInfo,
                                status_code,
                                ""));
                            failed_callback(std::move(ex));
                        }
                    }break;
                    }
                    unregisterReply(reply);
                });
            }//while
        }//runRequest
    };    
};
