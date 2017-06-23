#include <QFile>
#include "GoogleClient.h"
#include "Endpoint.h"
#include "google/endpoint/GoogleWebAuth.h"
#include "gmail/GmailRoutes.h"
#include "gtask/GtaskRoutes.h"
#include "gdrive/GdriveRoutes.h"

using namespace googleQt;

GoogleClient::GoogleClient(ApiAppInfo* appInfo,
                         ApiAuthInfo* authInfo)
    :ApiClient(appInfo, authInfo)
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

void GoogleClient::runEventsLoop()const
{
    m_endpoint->runEventsLoop();
};

void GoogleClient::exitEventsLoop()const
{
    m_endpoint->exitEventsLoop();
};

void GoogleClient::setUserId(QString email)
{
    if(email != userId()){
        ApiClient::setUserId(email);
        if(m_gmail_routes){
            m_gmail_routes->onUserReset();
        }
    }
};

QByteArray GoogleClient::lastResponse()
{
    return m_endpoint->lastResponse();
};

void GoogleClient::printLastApiCall()
{
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "API call" << std::endl;
    std::cout << lastApiCall().toStdString() << std::endl;
};

void GoogleClient::printLastResponse() 
{
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "API call" << std::endl;
    std::cout << lastApiCall().toStdString() << std::endl;    
    std::cout << "-----------------------------------------" << std::endl;
    std::cout << "response" << std::endl;
    std::cout << lastResponse().toStdString() << std::endl;
};

void GoogleClient::exportLastResponse(QString fileName) 
{
    QFile file_in(fileName);
    if (!file_in.open(QFile::WriteOnly)) {
        qWarning() << "Error opening file: " << fileName;
        return;
    }
    file_in.write(lastApiCall().toStdString().c_str());
    file_in.write("\n-----------------------------------------\n");
    file_in.write(lastResponse());
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

void GoogleClient::setNetworkProxy(const QNetworkProxy& proxy)
{
    m_endpoint->setProxy(proxy);
};
