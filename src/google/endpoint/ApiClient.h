#pragma once
#include "ApiUtil.h"
#include "ApiAuthInfo.h"
#include "ApiAppInfo.h"

namespace googleQt{
    class ApiClient: public QObject{
        Q_OBJECT
    public:
        ApiClient(ApiAppInfo* appInfo, ApiAuthInfo* authInfo);
        virtual ~ApiClient() {};
        QString getToken()const;
        QString getAppKey()const;
        virtual bool refreshToken() = 0;
        virtual QString userId()const = 0;
        
    signals:
        void downloadProgress(qint64 bytesProcessed, qint64 total);
        void uploadProgress(qint64 bytesProcessed, qint64 total);

    protected:
        std::unique_ptr<ApiAppInfo>  m_app;
        std::unique_ptr<ApiAuthInfo> m_auth;
    };
};
