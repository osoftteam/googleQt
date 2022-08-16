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

#ifdef API_QT_AUTOTEST
#include <QTimer>
#include <QCoreApplication>
#endif

namespace googleQt{
    class Endpoint: public googleQt::ApiEndpoint
    {
    public:
        Endpoint(googleQt::ApiClient* c);       

        /**
            let compiler bind right endpoint function
        */
        DECL_STD_BOUND_TASK_CB      (postStyle);
        DECL_STD_BOUND_TASK_CB      (putStyle);
        DECL_STD_BOUND_TASK_CB      (rfc822UploadStyle);
        DECL_STD_BOUND_TASK_CB      (updateStyle);
        DECL_STD_BOUND_TASK_CB      (postStyleB);
        DECL_BODY_VOID_RES_BOUND_TASK_CB(postStyleB2Empty);
        DECL_STD_BOUND_TASK_CB      (postContactStyleB);
        DECL_STD_BOUND_TASK_CB      (postContactGroupStyleB);
        DECL_STD_BOUND_TASK_CB      (putContactStyleB);
        DECL_STD_BOUND_TASK_CB      (putContactGroupStyleB);
        DECL_BODYLESS_BOUND_TASK_CB (getStyle);
        DECL_BODYLESS_BOUND_TASK_CB (getContactStyle);
        DECL_BODYLESS_BOUND_TASK_CB (postStyle);
        DECL_VOID_BOUND_TASK_CB     (postStyle);
        DECL_VOID_BOUND_TASK_CB     (deleteStyle);
        DECL_BODY_VOID_RES_BOUND_TASK_CB(deleteContactStyleB);
        DECL_BODY_VOID_RES_BOUND_TASK_CB(deleteContactGroupStyleB);
        DECL_BODY_VOID_RES_BOUND_TASK_CB(deleteContactPhotoStyleB);        

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
            std::shared_ptr<requester> rb(new POST_requester(*this, std::move(js)));
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
            std::shared_ptr<requester> rb(new POST_requester(*this, QJsonObject()));
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
            std::function<void(std::unique_ptr<googleQt::VoidType>)> completed_with_type = nullptr;
            if (completed_callback != nullptr)
            {
                completed_with_type = [=](std::unique_ptr<googleQt::VoidType>)
                {
                    completed_callback();
                };
            }

            std::shared_ptr<requester> rb(new POST_requester(*this, QJsonObject()));
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
            std::shared_ptr<requester> rb(new PUT_requester(*this, std::move(js)));
            runRequest<RES, 
                RESULT_FACTORY>
                (url,
                 std::move(rb),
                 completed_callback,
                 failed_callback);
        }

        template <class RES, class RESULT_FACTORY, class BODY>
        void updateStyle(QUrl url, const BODY& body,
            std::function<void(std::unique_ptr<RES>)> completed_callback,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            QJsonObject js = body;
            std::shared_ptr<requester> rb(new UPDATE_requester(*this, std::move(js)));
            runRequest<RES, 
                RESULT_FACTORY>
                (url,
                 std::move(rb),
                 completed_callback,
                 failed_callback);
        }

        /// BEGIN contact routes ///
        template <class RES, class RESULT_FACTORY>
        void getContactStyle(QUrl url,
            std::function<void(std::unique_ptr<RES>)> completed_callback,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            std::shared_ptr<requester> rb(new GET_requester4Contact(*this));
            runRequest<RES, RESULT_FACTORY>
                (url,
                    std::move(rb),
                    completed_callback,
                    failed_callback);
        }

        template <class RES,
            class RESULT_FACTORY,
            class BODY>
            void postContactStyleB(QUrl url,
                const BODY& body,
                std::function<void(std::unique_ptr<RES>)> completed_callback,
                std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            QString xml = body.toXml(client()->userId());
            std::shared_ptr<requester> rb(new POST_requester4Contact(*this, std::move(xml)));
            runRequest<RES,
                RESULT_FACTORY>
                (url,
                    std::move(rb),
                    completed_callback,
                    failed_callback);
        }

        template <class RES,
            class RESULT_FACTORY,
            class BODY>
            void putContactStyleB(QUrl url,
                const BODY& body,
                std::function<void(std::unique_ptr<RES>)> completed_callback,
                std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            QString etag = body.etag();
            QString xml = body.toXml();
            std::shared_ptr<requester> rb(new PUT_requester4Contact(*this, std::move(etag), std::move(xml)));
            runRequest<RES,
                RESULT_FACTORY>
                (url,
                    std::move(rb),
                    completed_callback,
                    failed_callback);
        }

        template <class BODY>
            void deleteContactStyleB(QUrl url,
                const BODY& body,
                std::function<void(void)> completed_callback,
                std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            std::function<void(std::unique_ptr<googleQt::VoidType>)> completed_with_type = nullptr;
            if (completed_callback != nullptr)
            {
                completed_with_type = [=](std::unique_ptr<googleQt::VoidType>)
                {
                    completed_callback();
                };
            }
            
            QString etag = body.etag();
            std::shared_ptr<requester> rb(new DELETE_requester4Contact(*this, std::move(etag)));
            runRequest<VoidType, VoidType>
                (url,
                    std::move(rb),
                    completed_with_type,
                    failed_callback);
        }
        
            template <class RES,
                class RESULT_FACTORY,
                class BODY>
                void postContactGroupStyleB(QUrl url,
                    const BODY& body,
                    std::function<void(std::unique_ptr<RES>)> completed_callback,
                    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
            {
                QString xml = body.toXml(client()->userId());
                std::shared_ptr<requester> rb(new POST_requester4Contact(*this, std::move(xml)));
                runRequest<RES,
                    RESULT_FACTORY>
                    (url,
                        std::move(rb),
                        completed_callback,
                        failed_callback);
            }

            template <class RES,
                class RESULT_FACTORY,
                class BODY>
                void putContactGroupStyleB(QUrl url,
                    const BODY& body,
                    std::function<void(std::unique_ptr<RES>)> completed_callback,
                    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
            {
                QString etag = body.etag();
                QString xml = body.toXml();
                std::shared_ptr<requester> rb(new PUT_requester4Contact(*this, std::move(etag), std::move(xml)));
                runRequest<RES,
                    RESULT_FACTORY>
                    (url,
                        std::move(rb),
                        completed_callback,
                        failed_callback);
            }

            template <class BODY>
            void deleteContactGroupStyleB(QUrl url,
                const BODY& body,
                std::function<void(void)> completed_callback,
                std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
            {
                std::function<void(std::unique_ptr<googleQt::VoidType>)> completed_with_type = nullptr;
                if (completed_callback != nullptr)
                {
                    completed_with_type = [=](std::unique_ptr<googleQt::VoidType>)
                    {
                        completed_callback();
                    };
                }

                QString etag = body.etag();
                std::shared_ptr<requester> rb(new DELETE_requester4Contact(*this, std::move(etag)));
                runRequest<VoidType, VoidType>
                    (url,
                        std::move(rb),
                        completed_with_type,
                        failed_callback);
            }

        ///photo
        void downloadContactPhotoStyle(QUrl url, QIODevice* writeTo, GoogleVoidTask* t)
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
            downloadContactPhotoStyle(url, writeTo, completed_callback, failed_callback);
        };

        void downloadContactPhotoStyle(QUrl url,
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

            std::shared_ptr<requester> rb(new DOWNLOAD_requester4Contact(*this, writeTo));
            runRequest<VoidType, VoidType>
                (url,
                    std::move(rb),
                    completed_with_type,
                    failed_callback);
        }

        void uploadContactPhotoStyle(QUrl url, QIODevice* writeTo, GoogleVoidTask* t)
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
            uploadContactPhotoStyle(url, writeTo, completed_callback, failed_callback);
        };


        void uploadContactPhotoStyle(QUrl url,
            QIODevice* readFrom,
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
            QString etag("*");//ykh - todo - has to come from parameter
            std::shared_ptr<requester> rb(new UPLOAD_photo_requester4Contact(*this, readFrom, std::move(etag)));
            runRequest<VoidType, VoidType>
                (url,
                    std::move(rb),
                    completed_with_type,
                    failed_callback);
        }

        template <class BODY>
        void deleteContactPhotoStyleB(QUrl url,
            const BODY& body,
            std::function<void(void)> completed_callback,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            std::function<void(std::unique_ptr<googleQt::VoidType>)> completed_with_type = nullptr;
            if (completed_callback != nullptr)
            {
                completed_with_type = [=](std::unique_ptr<googleQt::VoidType>)
                {
                    completed_callback();
                };
            }

            QString etag = body.etag();
            std::shared_ptr<requester> rb(new DELETE_requester4Contact(*this, std::move(etag)));
            runRequest<VoidType, VoidType>
                (url,
                    std::move(rb),
                    completed_with_type,
                    failed_callback);
        }

        /// END contact routes ///

        template <class RES, class RESULT_FACTORY, class BODY>
        void postStyleB(QUrl url, const BODY& body,
            std::function<void(std::unique_ptr<RES>)> completed_callback,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            postStyle<RES, RESULT_FACTORY, BODY>
                (url, body, completed_callback, failed_callback);
        }
        
        template <class BODY>
        void postStyleB2Empty(QUrl url, const BODY& body,
            std::function<void(void)> completed_callback,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
        {
            std::function<void(std::unique_ptr<googleQt::VoidType>)> completed_with_type = nullptr;
            if (completed_callback != nullptr)
            {
                completed_with_type = [=](std::unique_ptr<googleQt::VoidType>)
                {
                    completed_callback();
                };
            }

            QJsonObject js = body;
            std::shared_ptr<requester> rb(new POST_requester(*this, std::move(js)));
            runRequest<VoidType, VoidType>
                (url,
                    std::move(rb),
                    completed_with_type,
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
            std::shared_ptr<requester> rb(new MPartUpload_requester(*this, std::move(js), readFrom));
            runRequest<RES, RESULT_FACTORY>
                (url,
                 std::move(rb),
                 completed_callback,
                 failed_callback);
        }

        template <class RES, class RESULT_FACTORY>
        void simpleUploadStyle(QUrl url,
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

            simpleUploadStyle<RES, RESULT_FACTORY>(url, readFrom, completed_callback, failed_callback);
        }


        template <class RES, class RESULT_FACTORY>
        void simpleUploadStyle(QUrl url,
            QIODevice* readFrom,
            std::function<void(std::unique_ptr<RES>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            std::shared_ptr<requester> rb(new UPLOAD_requester(*this, readFrom));
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
            std::shared_ptr<requester> rb(new POST_rfc822_requester(*this, std::move(js)));
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
                    .arg(a.userId().isEmpty() ? client()->userId() : a.userId()).arg(namespace_path), url);
            return url;
        }

        template <class ARG>
        QUrl buildGmailAttachmentUrl(const ARG& a)const
        {
            QUrl url;
            a.build(QString("https://www.googleapis.com/gmail/v1/users/%1/messages/")
                .arg(a.userId().isEmpty() ? client()->userId() : a.userId()), url);
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
        QUrl buildGdriveMPartUploadUrl(const QString&, const ARG& a)const
        {
            QUrl url;
            a.build("https://www.googleapis.com/upload/drive/v3/files", url);
            addAppKeyParameter(url);
            return url;
        }

        template <class ARG>
        QUrl buildGdriveSimpleMediaUploadUrl(const QString&, const ARG&)const
        {
            QUrl url("https://www.googleapis.com/upload/drive/v3/files?uploadType=media");
            addAppKeyParameter(url);
            return url;
        }

        template <class ARG>
        QUrl buildContactUrl(const ARG& a)const
        {
            QUrl url;        
            a.build(QString("https://www.google.com/m8/feeds/contacts/%1/full")
                .arg(client()->userId()), url);
            return url;
        }

        template <class ARG>
        QUrl buildContactGroupUrl(const ARG& a)const
        {
            QUrl url;
            a.build(QString("https://www.google.com/m8/feeds/groups/%1/full")
                    .arg(client()->userId()), url);
            return url;
        }

        template <class ARG>
        QUrl buildContactBatchUrl(const ARG& a)const
        {
            QUrl url;
            a.build(QString("https://www.google.com/m8/feeds/contacts/%1/full/batch")
                .arg(client()->userId()), url);
            return url;
        }

        template <class ARG>
        QUrl buildContactGroupBatchUrl(const ARG& a)const
        {
            QUrl url;
            a.build(QString("https://www.google.com/m8/feeds/groups/%1/full/batch")
                .arg(client()->userId()), url);
            return url;
        }    

        template <class ARG>
        QUrl buildContactPhotoUrl(const ARG& a)const
        {
            QUrl url;
            QByteArray d = QUrl::toPercentEncoding(client()->userId());
            a.build(QString("https://www.google.com/m8/feeds/photos/media/%1")
                    .arg(d.data()), url);            
            return url;
        }
        
        virtual void onErrorUnauthorized(const errors::ErrorInfo* er);

        GoogleClient* client();
        const GoogleClient* client()const;

        template <class T>
        GoogleTask<T>* produceTask()
        {
            GoogleTask<T>* rv = new GoogleTask<T>(apiClient());
            return rv;
        };

        GoogleVoidTask* produceVoidTask()
        {
            return new GoogleVoidTask(apiClient());
        }

        TaskAggregator* produceAggregatorTask()override
        {
            return new TaskAggregator(apiClient());
        }

    protected:
        QString prepareErrorInfo(int status_code, const QUrl& url, const QByteArray& data);
        QString prepareErrorSummary(int status_code);
        void addAppKeyParameter(QUrl& url)const;


        template <class RES, class RESULT_FACTORY>
        void runRequest(QUrl url, 
                        std::shared_ptr<requester> firstBuilder,
                        std::function<void(std::unique_ptr<RES>)> completed_callback = nullptr,
                        std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr)
        {
            QNetworkRequest firstRequest(url);
            RESULT_FACTORY factory;

            QNetworkReply *firstReply = firstBuilder->makeRequest(firstRequest);
            if (!firstReply)
                {
#ifdef API_QT_AUTOTEST
                //emulate asynchronous call in autotest as in real net-request
                //so client processing would be same - async based
                //also emulate download proress
                if (completed_callback != nullptr)
                {                    
                    ApiAutotest::INSTANCE().emulateAutotestDownloadProgress(m_client);
                    QTimer::singleShot(10, [=]() {
                        if (ApiAutotest::INSTANCE().isCancelRequested()) {
                            qDebug() << "CancelRequested/endpoint";
                        }
                        else
                        {
                            completed_callback(RES::EXAMPLE(0, 0));
                        }
                    });
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
                                     case 201:
                                         {
                                             if (completed_callback != nullptr)
                                                 {
                                                    m_last_response = reply->readAll();
                                                    completed_callback(factory.create(m_last_response));
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
                                             m_last_response = reply->readAll();
                                             if (authErrorsLimit > 0)
                                                 {
                                                     std::unique_ptr<errors::ErrorInfo> er = errors::ErrorInfo::factory::create(m_last_response);
                                                     onErrorUnauthorized(er.get());
                                                     //have to reset auth header as it got changed, side-effect..
                                                     authErrorsLimit--;
                                                     QNetworkReply *secondaryReply = rb->makeRequest(req);
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
                                                             std::string errorInfo = prepareErrorInfo(status_code, url, m_last_response).toStdString();
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
                                                 std::string summary = prepareErrorSummary(status_code).toStdString();
                                                 m_last_response = reply->readAll();
                                                 std::string errorInfo = prepareErrorInfo(status_code,
                                                                                          url,
                                                                                          m_last_response).toStdString();
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
