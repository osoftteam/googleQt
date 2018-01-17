/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "contact_group"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gcontact/GcontactRequestArg.h"
#include "GoogleRouteBase.h"

namespace googleQt{
namespace contact_group{


    class ContactGroupRoutes: public GoogleRouteBase{
    public:
        ContactGroupRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('create')


            Create a new contacts group

            */
        std::unique_ptr<gcontact::ContactGroupListResult> create(const gcontact::CreateContactGroupArg& arg);
        GoogleTask<gcontact::ContactGroupListResult>* create_Async(const gcontact::CreateContactGroupArg& arg);
        void create_AsyncCB(
            const gcontact::CreateContactGroupArg&,
            std::function<void(std::unique_ptr<gcontact::ContactGroupListResult>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('deleteContactGroup')


            Delete contact group

            */
        void deleteContactGroup(const gcontact::DeleteContactGroupArg& );
        GoogleVoidTask* deleteContactGroup_Async(const gcontact::DeleteContactGroupArg& arg);
        void deleteContactGroup_AsyncCB(
            const gcontact::DeleteContactGroupArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Returns all contact groups for a user as a list or one group details
            if groupid is specified

            */
        std::unique_ptr<gcontact::ContactGroupListResult> list(const gcontact::ContactGroupListArg& arg);
        GoogleTask<gcontact::ContactGroupListResult>* list_Async(const gcontact::ContactGroupListArg& arg);
        void list_AsyncCB(
            const gcontact::ContactGroupListArg&,
            std::function<void(std::unique_ptr<gcontact::ContactGroupListResult>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('update')


            Update contacts group

            */
        std::unique_ptr<gcontact::ContactGroupListResult> update(const gcontact::UpdateContactGroupArg& arg);
        GoogleTask<gcontact::ContactGroupListResult>* update_Async(const gcontact::UpdateContactGroupArg& arg);
        void update_AsyncCB(
            const gcontact::UpdateContactGroupArg&,
            std::function<void(std::unique_ptr<gcontact::ContactGroupListResult>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//ContactGroupRoutes

}//contact_group
}//googleQt
