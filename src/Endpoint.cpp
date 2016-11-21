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

QString Endpoint::prepareErrorInfo(int status_code, const QUrl& url, const QByteArray& data) 
{
	QString rv = QString("ERROR. Unexpected status %1 %2 ").arg(status_code).arg(url.url());
	rv += data;
	rv += "\n";
	rv += lastRequestInfo();
	return rv;
};

void Endpoint::addAppKeyParameter(QUrl& url)const
{
	QUrlQuery q(url);
	q.addQueryItem("key", client()->getAppKey());
	url.setQuery(q);
};
