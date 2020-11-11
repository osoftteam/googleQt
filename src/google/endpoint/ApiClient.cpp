#include "ApiClient.h"

using namespace googleQt;

ApiClient::ApiClient(ApiAppInfo* appInfo, ApiAuthInfo* authInfo)
{
    m_app.reset(appInfo);
    m_auth.reset(authInfo);
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
