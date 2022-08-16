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
    class GoogleClient;

    namespace gcontact {
        class GContactCacheBase;
    };

    namespace mail_cache {
        class GmailCacheRoutes;
        class GMailSQLiteStorage;
    }


    using gclient_ptr = std::shared_ptr<GoogleClient>;

    gclient_ptr createClient(std::shared_ptr<googleQt::ApiAppInfo> appInfo, std::shared_ptr<googleQt::ApiAuthInfo> authInfo, gcontact::GContactCacheBase* custom_contacts_cache = nullptr);
    void releaseClient(gclient_ptr);

class GoogleClient: public googleQt::ApiClient{
    Q_OBJECT
public:
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
   bool       isQueryInProgress()const;
    
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

    /**
    * gmail poiners shortcuts
    */
    googleQt::mail_cache::GmailCacheRoutes*     gmail_cache_routes();
    googleQt::mail_cache::GMailSQLiteStorage*   gmail_storage();

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
    
    gcontact::GContactCacheBase*            contacts_cache() {return m_contacts_cache;}
    ApiEndpoint* endpoint()override;

protected:
    GoogleClient(std::shared_ptr<googleQt::ApiAppInfo> appInfo, std::shared_ptr<googleQt::ApiAuthInfo> authInfo, gcontact::GContactCacheBase* custom_contacts_cache = nullptr);
    std::unique_ptr<GmailRoutes>            m_gmail_routes;
    std::unique_ptr<GtaskRoutes>            m_gtask_routes;
    std::unique_ptr<GdriveRoutes>           m_gdrive_routes;
    std::unique_ptr<GcontactRoutes>         m_contact_routes;
    std::unique_ptr<Endpoint>               m_endpoint;
    gcontact::GContactCacheBase*            m_contacts_cache{nullptr};
    bool                                    m_own_contacts_cache{ false };

    friend gclient_ptr googleQt::createClient(std::shared_ptr<ApiAppInfo> appInfo, std::shared_ptr<ApiAuthInfo> authInfo, gcontact::GContactCacheBase* custom_contacts_cache);
};

};
