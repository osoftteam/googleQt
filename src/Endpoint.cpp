#include <QUrlQuery>
#include "Endpoint.h"

using namespace googleQt;

Endpoint::Endpoint(ApiClient* c):ApiEndpoint(c)
{
    
}


GoogleClient* Endpoint::client()
{
    GoogleClient* rv = dynamic_cast<GoogleClient*>(m_client);
    return rv;
};

const GoogleClient* Endpoint::client()const
{
    const GoogleClient* rv = dynamic_cast<const GoogleClient*>(m_client);
    return rv;
};

void Endpoint::onErrorUnauthorized(const errors::ErrorInfo*)
{
    if(!client()->refreshToken())
        {
            qWarning() << "Failed to refresh token";
        };
};

QString Endpoint::prepareErrorSummary(int status_code)
{
    QString s;
    switch(status_code)
        {
        case 400:s = "Bad Request"; break;
        case 403:s = "Invalid access token. You have to get new access token.";break;
        case 404:s = "Resource not found on server.";break;
        case 500:s = "Internal Server Error. Generic server error."; break;
        }
    QString rv = QString("%1 - %2").arg(status_code).arg(s);
    return rv;
};

QString Endpoint::prepareErrorInfo(int status_code, const QUrl& url, const QByteArray& data) 
{
    QString rv = QString("ERROR. Unexpected status %1 %2 ").arg(status_code).arg(url.url());
    rv += data;
    rv += "\n";
    rv += lastRequestInfo().request;
    return rv;
};

void Endpoint::addAppKeyParameter(QUrl& url)const
{
    QUrlQuery q(url);
    q.addQueryItem("key", client()->getAppKey());
    url.setQuery(q);
};
