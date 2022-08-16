#include "ApiClient.h"

using namespace googleQt;

ApiClient::ApiClient(std::shared_ptr<ApiAppInfo> appInfo, std::shared_ptr<ApiAuthInfo> authInfo)
{
    m_app = appInfo;
    m_auth = authInfo;
};

QString ApiClient::getToken()const
{
    return m_auth->getAccessToken();
}

int ApiClient::getAccessScope()const
{
    return m_auth->getScope();
};

QString ApiClient::getAppKey()const 
{
    return m_app->getKey();
};

QString ApiClient::userId()const
{
    return m_auth->getEmail();
};

void ApiClient::setUserId(QString email)
{
    m_auth->setEmail(email);
};

const std::shared_ptr<ApiAppInfo> &ApiClient::app() const
{
    return m_app;
}

const std::shared_ptr<ApiAuthInfo> &ApiClient::auth() const
{
    return m_auth;
}
