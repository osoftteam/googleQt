#pragma once

#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"
#include "google/endpoint/GoogleTask.h"

namespace googleQt{
    class Endpoint;
    class GmailRoutes;
    class GtaskRoutes;
    class GdriveRoutes;

class GoogleClient: public googleQt::ApiClient{
    Q_OBJECT
public:
    GoogleClient(googleQt::ApiAppInfo* appInfo, googleQt::ApiAuthInfo* authInfo, const QString& userId = "");
    ~GoogleClient();

    /**
    * gmail - v1 Google Gmail API
    */
    GmailRoutes*  gmail();

    /**
    * gtask - v1 Google GTasks API
    */
    GtaskRoutes*  gtask();

    /**
    * gdrive - v3 Google Drive API
    */
    GdriveRoutes* gdrive();

    /**
    * used by some API (gmail)
    */
    QString userId()const{return m_userId;}

    /**
    * cancell all requests and exit blocking call is any
    */
    void          cancelAllRequests();

    /**
    * refresh access token
    */
    virtual bool refreshToken();

    /*
        some debug functions, we might remove them in
        future release
    */
    QString lastApiCall();
    QByteArray lastResponse();
    void printLastApiCall();
    void printLastResponse();
    void exportLastResponse(QString fileName);    
    void setNetworkProxy(const QNetworkProxy& proxy);
    
protected:
    std::unique_ptr<GmailRoutes>           m_gmail_routes;
    std::unique_ptr<GtaskRoutes>           m_gtask_routes;
    std::unique_ptr<GdriveRoutes>          m_gdrive_routes;
    std::unique_ptr<Endpoint>              m_endpoint;
    QString                                m_userId;
};

};
