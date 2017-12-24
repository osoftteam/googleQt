/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "contacts"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gcontact/GcontactRequestArg.h"
#include "GoogleRouteBase.h"
#include "gcontact/contacts/ContactsContactCollectionRes.h"

namespace googleQt{
namespace contacts{


    class ContactsRoutes: public GoogleRouteBase{
    public:
        ContactsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('list')


            Returns all contacts for a user as a list

            */
        std::unique_ptr<ContactCollectionRes> list(const gcontact::ContactsListArg& arg);
        GoogleTask<ContactCollectionRes>* list_Async(const gcontact::ContactsListArg& arg);
        void list_AsyncCB(
            const gcontact::ContactsListArg&,
            std::function<void(std::unique_ptr<ContactCollectionRes>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//ContactsRoutes

}//contacts
}//googleQt
