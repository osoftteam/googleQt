/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "contacts"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gcontact/GcontactRequestArg.h"
#include "GoogleRouteBase.h"

namespace googleQt{
namespace contacts{


    class ContactsRoutes: public GoogleRouteBase{
    public:
        ContactsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('create')


            Create a new contact

            */
        std::unique_ptr<gcontact::ContactsListResult> create(const gcontact::CreateContactArg& arg);
        GoogleTask<gcontact::ContactsListResult>* create_Async(const gcontact::CreateContactArg& arg);
        void create_AsyncCB(
            const gcontact::CreateContactArg&,
            std::function<void(std::unique_ptr<gcontact::ContactsListResult>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('deleteContact')


            Delete contact

            */
        void deleteContact(const gcontact::DeleteContactArg& );
        GoogleVoidTask* deleteContact_Async(const gcontact::DeleteContactArg& arg);
        void deleteContact_AsyncCB(
            const gcontact::DeleteContactArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Returns all contacts for a user as a list

            */
        std::unique_ptr<gcontact::ContactsListResult> list(const gcontact::ContactsListArg& arg);
        GoogleTask<gcontact::ContactsListResult>* list_Async(const gcontact::ContactsListArg& arg);
        void list_AsyncCB(
            const gcontact::ContactsListArg&,
            std::function<void(std::unique_ptr<gcontact::ContactsListResult>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('update')


            Update contact

            */
        std::unique_ptr<gcontact::ContactsListResult> update(const gcontact::UpdateContactArg& arg);
        GoogleTask<gcontact::ContactsListResult>* update_Async(const gcontact::UpdateContactArg& arg);
        void update_AsyncCB(
            const gcontact::UpdateContactArg&,
            std::function<void(std::unique_ptr<gcontact::ContactsListResult>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//ContactsRoutes

}//contacts
}//googleQt
