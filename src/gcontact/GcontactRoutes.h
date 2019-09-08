#pragma once
#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"

#include "gcontact/contacts/ContactsRoutes.h"
#include "gcontact/contact_group/ContactGroupRoutes.h"

namespace googleQt
{
    class Endpoint;

    namespace gcontact {
        class GcontactCacheRoutes;
        class GContactCacheBase;
    };

    class GcontactRoutes {
    public:

        GcontactRoutes(Endpoint&/*, gcontact::GContactCacheBase**/);
        Endpoint& endpoint() { return m_endpoint; }

        contacts::ContactsRoutes* getContacts();
        contact_group::ContactGroupRoutes* getContactGroup();
        googleQt::gcontact::GcontactCacheRoutes* cacheRoutes();

        static QString encodeGroupUri(QString userId, QString groupId);
        
#ifdef API_QT_AUTOTEST
        void autotest();
#endif

    protected:
        Endpoint&  m_endpoint;
        std::unique_ptr<contacts::ContactsRoutes> m_contacts;
        std::unique_ptr<contact_group::ContactGroupRoutes> m_contact_group;
        std::unique_ptr<googleQt::gcontact::GcontactCacheRoutes>    m_CacheRoutes;
        //gcontact::GContactCacheBase* m_contacts_cache{nullptr};
    };  
};//googleQt

