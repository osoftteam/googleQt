#pragma once
#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"

#include "gcontact/contacts/ContactsRoutes.h"


namespace googleQt
{
    class Endpoint;

    class GcontactRoutes {
    public:

        GcontactRoutes(Endpoint*);
        Endpoint* endpoint() { return m_endpoint; }

        contacts::ContactsRoutes* getContacts();

#ifdef API_QT_AUTOTEST
        void autotest() {};
#endif

    protected:
        std::unique_ptr<contacts::ContactsRoutes> m_Contacts;
        Endpoint*  m_endpoint;
    };  
};//googleQt

