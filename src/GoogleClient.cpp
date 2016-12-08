#include <QFile>
#include "GoogleClient.h"
#include "Endpoint.h"
#include "google/endpoint/GoogleWebAuth.h"
#include "gmail/GmailRoutes.h"
#include "gtask/GtaskRoutes.h"
#include "gdrive/GdriveRoutes.h"

using namespace googleQt;

GoogleClient::GoogleClient(ApiAppInfo* appInfo,
                         ApiAuthInfo* authInfo,
                         const QString& userId)
    :ApiClient(appInfo, authInfo), m_userId(userId)
{
    m_endpoint.reset(new Endpoint(this));
};

GoogleClient::~GoogleClient(){

};

void GoogleClient::cancelAllRequests() 
{
	m_endpoint->cancelAll();
};

QString GoogleClient::lastApiCall()
{
    return m_endpoint->lastRequestInfo();
}

QByteArray GoogleClient::last200Response()
{
	return m_endpoint->last200Response();
};

void GoogleClient::printLastApiCall()
{
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "API call" << std::endl;
    std::cout << lastApiCall().toStdString() << std::endl;
};

void GoogleClient::printLast200Response() 
{
	std::cout << "-----------------------------------------" << std::endl;
	std::cout << "200 (OK) response" << std::endl;
	std::cout << last200Response().toStdString() << std::endl;
};

void GoogleClient::exportLast200Response(QString fileName) 
{
	QFile file_in(fileName);
	if (!file_in.open(QFile::WriteOnly)) {
		qWarning() << "Error opening file: " << fileName;
		return;
	}
	file_in.write(last200Response());
	file_in.close();
};

GmailRoutes* GoogleClient::gmail()
{
    if(!m_gmail_routes){
        m_gmail_routes.reset(new GmailRoutes(m_endpoint.get()));
    }
    return m_gmail_routes.get();
};

GtaskRoutes* GoogleClient::gtask()
{
    if(!m_gtask_routes){
      m_gtask_routes.reset(new GtaskRoutes(m_endpoint.get()));
    }
    return m_gtask_routes.get();
};

GdriveRoutes* GoogleClient::gdrive() 
{
    if (!m_gdrive_routes) {
        m_gdrive_routes.reset(new GdriveRoutes(m_endpoint.get()));
    }
    return m_gdrive_routes.get();
};

bool GoogleClient::refreshToken()
{
    bool rv = GoogleWebAuth::refreshToken(m_app.get(), m_auth.get());
    return rv;
};
