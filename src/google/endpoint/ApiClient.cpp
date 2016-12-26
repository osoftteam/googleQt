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

QString ApiClient::getAppKey()const 
{
    return m_app->getKey();
};

