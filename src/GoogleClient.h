#pragma once

#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"

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

    QString userId()const{return m_userId;}
    QString lastApiCall();
    void printLastApiCall();

    virtual bool refreshToken();
    
protected:
    std::unique_ptr<GmailRoutes>           m_gmail_routes;
    std::unique_ptr<GtaskRoutes>           m_gtask_routes;
    std::unique_ptr<GdriveRoutes>          m_gdrive_routes;
    std::unique_ptr<Endpoint>              m_endpoint;
    QString                                m_userId;
};

};
