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

		/**
			let compiler bind right endpoint function
		*/
        DECL_STD_BOUND_TASK_CB		(postStyle);
        DECL_STD_BOUND_TASK_CB		(putStyle);
        DECL_STD_BOUND_TASK_CB		(rfc822UploadStyle);
        DECL_BODYLESS_BOUND_TASK_CB	(getStyle);
        DECL_BODYLESS_BOUND_TASK_CB	(postStyle);
		DECL_VOID_BOUND_TASK_CB		(postStyle);
		DECL_VOID_BOUND_TASK_CB		(deleteStyle);

        template <class RES, class RESULT_FACTORY>
        void getStyle(QUrl url,
            std::function<void(std::unique_ptr<RES>)> completed_callback,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            std::shared_ptr<requester> rb(new GET_requester(*this));
            runRequest<RES, RESULT_FACTORY>
                (url, 
                 std::move(rb),
                 completed_callback,
                 failed_callback);
        }
        

        template <class RES, 
            class RESULT_FACTORY, 
            class BODY>
        void postStyle(QUrl url, 
            const BODY& body,
            std::function<void(std::unique_ptr<RES>)> completed_callback,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            QJsonObject js = body;
            std::shared_ptr<requester> rb(new POST_requester(*this, js));
            runRequest<RES, 
                RESULT_FACTORY>
                (url,
                 std::move(rb),
                 completed_callback,
                 failed_callback);
        }

        template <class RES, 
            class RESULT_FACTORY>
        void postStyle(QUrl url,
            std::function<void(std::unique_ptr<RES>)> completed_callback,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            QJsonObject js = QJsonObject();
            std::shared_ptr<requester> rb(new POST_requester(*this, js));
            runRequest<RES, 
                RESULT_FACTORY>
                (url,
                 std::move(rb),
                 completed_callback,
                 failed_callback);
        }

        void postStyle(QUrl url,
            std::function<void(void)> completed_callback,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
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

            std::shared_ptr<requester> rb(new POST_requester(*this, js));
            runRequest<VoidType,VoidType>
                (url,
                 std::move(rb),
                 completed_with_type,
                 failed_callback);
        }


        template <class RES, class RESULT_FACTORY, class BODY>
        void putStyle(QUrl url, const BODY& body,
            std::function<void(std::unique_ptr<RES>)> completed_callback,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            QJsonObject js = body;
            std::shared_ptr<requester> rb(new PUT_requester(*this, js));
            runRequest<RES, 
                RESULT_FACTORY>
                (url,
                 std::move(rb),
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

            std::shared_ptr<requester> rb(new DELETE_requester(*this));
            runRequest<VoidType, VoidType>
                (url,
                 std::move(rb),
                 completed_with_type,
                 failed_callback);
                    
        }

        template <class RES, class RESULT_FACTORY, class BODY>
        void mpartUploadStyle(QUrl url,
            const BODY& body,
            QIODevice* readFrom, 
            GoogleTask<RES>* t)
        {
            std::function<void(std::unique_ptr<RES>)> completed_callback =
                [=](std::unique_ptr<RES> r)
            {
                t->completed_callback(std::move(r));
            };

            std::function<void(std::unique_ptr<GoogleException>)> failed_callback =
                [=](std::unique_ptr<GoogleException> ex)
            {
                t->failed_callback(std::move(ex));
            };

            mpartUploadStyle<RES, RESULT_FACTORY, BODY>(url, body, readFrom, completed_callback, failed_callback);
        }

        template <class RES, class RESULT_FACTORY, class BODY>
        void mpartUploadStyle(QUrl url, 
            const BODY& body,
            QIODevice* readFrom,
            std::function<void(std::unique_ptr<RES>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            QJsonObject js = body;
            std::shared_ptr<requester> rb(new MPartUpload_requester(*this, js, readFrom));
            runRequest<RES, RESULT_FACTORY>
                (url,
                 std::move(rb),
                 completed_callback,
                 failed_callback);
        }

        void downloadStyle(QUrl url, QIODevice* writeTo, GoogleVoidTask* t)
        {
            std::function<void(void)> completed_callback =
                [=](void)
            {
                t->completed_callback();
            };

            std::function<void(std::unique_ptr<GoogleException>)> failed_callback =
                [=](std::unique_ptr<GoogleException> ex)
            {
                t->failed_callback(std::move(ex));
            };
            downloadStyle(url, writeTo, completed_callback, failed_callback);
        };

        void downloadStyle(QUrl url,
            QIODevice* writeTo,
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

            std::shared_ptr<requester> rb(new DOWNLOAD_requester(*this, writeTo));
            runRequest<VoidType, VoidType>
                (url,
                    std::move(rb),
                    completed_with_type,
                    failed_callback);
        }



        template <class RES, class RESULT_FACTORY, class BODY>
        void rfc822UploadStyle(QUrl url, const BODY& body,
            std::function<void(std::unique_ptr<RES>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            QJsonObject js = body;
            std::shared_ptr<requester> rb(new POST_rfc822_requester(*this, js));
            runRequest<RES, 
                RESULT_FACTORY>
                (url,
                 std::move(rb),
                 completed_callback,
                 failed_callback);
        }

        template <class ARG>
        QUrl buildGmailUrl(const QString& namespace_path, const ARG& a)const
        {
            QUrl url;
            a.build(QString("https://www.googleapis.com/gmail/v1/users/%1/%2")
                    .arg(client()->userId()).arg(namespace_path), url);
            return url;
        }

        template <class ARG>
        QUrl buildGmailUploadlUrl(const QString& namespace_path, const ARG& a)const
        {
            QUrl url;
            a.build(QString("https://www.googleapis.com/upload/gmail/v1/users/%1/%2")
                    .arg(client()->userId()).arg(namespace_path), url);
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
        QUrl buildGdriveMPartUploadUrl(const QString&, const ARG&)const
        {
            QUrl url("https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart");
            //a.build("https://www.googleapis.com/upload/drive/v3/files?uploadType=multipart", url);
            addAppKeyParameter(url);
            return url;
        }


        virtual void onErrorUnauthorized(const errors::ErrorInfo* er);

        GoogleClient* client();
        const GoogleClient* client()const;

    protected:
        QString prepareErrorInfo(int status_code, const QUrl& url, const QByteArray& data);
        void addAppKeyParameter(QUrl& url)const;


        template <class RES, class RESULT_FACTORY>
        void runRequest(QUrl url, 
                        std::shared_ptr<requester> firstBuilder,
                        std::function<void(std::unique_ptr<RES>)> completed_callback = nullptr,
                        std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            QNetworkRequest firstRequest(url);
            addAuthHeader(firstRequest);
            RESULT_FACTORY factory;

            QNetworkReply *firstReply = firstBuilder->request(firstRequest);
            if (!firstReply)
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
            
            std::shared_ptr<FINISHED_REQ> finishedRequest(new FINISHED_REQ([=](std::shared_ptr<requester> rb,
                                                            QNetworkRequest req,
                                                            QNetworkReply *reply,
                                                            int authErrorsLimit)
                             {
                                 int status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                                 switch (status_code)
                                     {
                                     case 200:
                                         {
                                             if (completed_callback != nullptr)
                                                 {
                                                     QByteArray data = reply->readAll();
                                                     std::cout << "200-data:" << data.data() << std::endl;
                                                     completed_callback(factory.create(data));
                                                 }
                                         }break;
                                     case 202:
                                     case 204:
                                         {
                                             if (completed_callback != nullptr) 
                                                 {
                                                     completed_callback(factory.create(QByteArray()));
                                                 }
                                         }break;
                                     case 401:
                                         {
                                             QByteArray data = reply->readAll();
                                             if (authErrorsLimit > 0)
                                                 {
                                                     std::unique_ptr<errors::ErrorInfo> er = errors::ErrorInfo::factory::create(data);
                                                     onErrorUnauthorized(er.get());
                                                     //have to reset auth header as it got changed, side-effect..
                                                     addAuthHeader(req);
                                                     authErrorsLimit--;
                                                     QNetworkReply *secondaryReply = rb->request(req);
                                                     if(secondaryReply)
                                                         {
                                                            auto i = m_replies_in_progress.find(reply);
                                                            if (i != m_replies_in_progress.end() && i->second != nullptr)
                                                            {
                                                                std::shared_ptr<FINISHED_REQ> cb2 = i->second;
                                                                registerReply(rb, secondaryReply, cb2);
                                                                
                                                                QObject::connect(secondaryReply,
                                                                    &QNetworkReply::finished,
                                                                    std::bind(*(cb2.get()),
                                                                        rb,
                                                                        req,
                                                                        secondaryReply,
                                                                        authErrorsLimit));                                                                        
                                                            }                                                             
                                                         }
                                                 }
                                             else
                                                 {
                                                    qWarning() << "Error. Failed to refresh access token, giving up after "
                                                               << TIMES_TO_REFRESH_TOKEN_BEFORE_GIVEUP << " attempts";
                                                     if (failed_callback != nullptr)
                                                         {
                                                             std::string errorInfo = prepareErrorInfo(status_code, url, data).toStdString();
                                                             std::unique_ptr<GoogleException> ex(new GoogleException(errorInfo,
                                                                                                                     status_code,
                                                                                                                     "Failed to refresh access token"));
                                                             failed_callback(std::move(ex));
                                                         }
                                                 }
                                         }break;
                                     default:
                                         {
                                             if (failed_callback != nullptr) {
                                                 std::string summary;
                                                 if(status_code == 403){
                                                     summary = "Invalid access token. You have to get new access token.";
                                                 }
                                                 QByteArray exception_data = reply->readAll();
                                                 std::string errorInfo = prepareErrorInfo(status_code,
                                                                                          url,
                                                                                          exception_data).toStdString();
                                                 std::unique_ptr<GoogleException> ex(new GoogleException(errorInfo,
                                                                                                         status_code,
                                                                                                         summary));
                                                 failed_callback(std::move(ex));
                                             }
                                         }break;
                                     }//switch
                                 unregisterReply(reply);
                             }));//finished - lambda

            registerReply(firstBuilder, firstReply, finishedRequest);
            QObject::connect(firstReply,
                             &QNetworkReply::finished,
                             std::bind(*(finishedRequest.get()),
                                       std::move(firstBuilder),
                                       firstRequest,
                                       firstReply,
                                 TIMES_TO_REFRESH_TOKEN_BEFORE_GIVEUP));
                                       
        }//runRequest
    };    
};
