#pragma once

#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"
#include "google/endpoint/GoogleTask.h"

namespace googleQt{
    class Endpoint;
    class GmailRoutes;
    class GtaskRoutes;
    class GdriveRoutes;
    class GcontactRoutes;

class GoogleClient: public googleQt::ApiClient{
    Q_OBJECT
public:
    GoogleClient(googleQt::ApiAppInfo* appInfo, googleQt::ApiAuthInfo* authInfo);
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
     * gcontact - v3 Google Contacts API 
     */
    GcontactRoutes* gcontact();

    /**
    * cancell all requests and exit blocking call if any
    */
    void          cancelAllRequests();

   /// return true when running query
   bool	      isQueryInProgress()const;
    
    /**
    * event loop functions, not needed in event-driven application,
    * like gui-based. But in basic console app might be usefull
    */    
    void          runEventsLoop()const;
    void          exitEventsLoop()const;

    /**
    * refresh access token
    */
    virtual bool refreshToken()override;

    /**
     * when user ID changes and cache is setup
     * we have to reload it for new user
     */
    void setUserId(QString email)override;

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

    ///it should be protected method, normally some specialized
    ///methods must be used. But for debugging purpose it's ok
    Endpoint* endpoint();
    
protected:
    std::unique_ptr<GmailRoutes>           m_gmail_routes;
    std::unique_ptr<GtaskRoutes>           m_gtask_routes;
    std::unique_ptr<GdriveRoutes>          m_gdrive_routes;
    std::unique_ptr<GcontactRoutes>        m_contact_routes;
    std::unique_ptr<Endpoint>              m_endpoint;
};

};
