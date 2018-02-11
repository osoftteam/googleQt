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
            ApiRoute('batch')


            Send a batch request for operations on contacts

            */
        std::unique_ptr<gcontact::BatchContactList> batch(const gcontact::BatchContactArg& arg);
        GoogleTask<gcontact::BatchContactList>* batch_Async(const gcontact::BatchContactArg& arg);
        void batch_AsyncCB(
            const gcontact::BatchContactArg&,
            std::function<void(std::unique_ptr<gcontact::BatchContactList>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('create')


            Create a new contact

            */
        std::unique_ptr<gcontact::ContactList> create(const gcontact::CreateContactArg& arg);
        GoogleTask<gcontact::ContactList>* create_Async(const gcontact::CreateContactArg& arg);
        void create_AsyncCB(
            const gcontact::CreateContactArg&,
            std::function<void(std::unique_ptr<gcontact::ContactList>)> completed_callback = nullptr,
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
            ApiRoute('deleteContactPhoto')


            Delete contact photo

            */
        void deleteContactPhoto(const gcontact::DeletePhotoArg& );
        GoogleVoidTask* deleteContactPhoto_Async(const gcontact::DeletePhotoArg& arg);
        void deleteContactPhoto_AsyncCB(
            const gcontact::DeletePhotoArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('getContactPhoto')


            Get contact photo content.

            */
        void getContactPhoto(const gcontact::DownloadPhotoArg& , QIODevice* writeTo);
        GoogleVoidTask* getContactPhoto_Async(const gcontact::DownloadPhotoArg& arg, QIODevice* data);
        void getContactPhoto_AsyncCB(
            const gcontact::DownloadPhotoArg&,
            QIODevice* data,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Returns all contacts for a user as a list or one contact details if
            contactid is specified

            */
        std::unique_ptr<gcontact::ContactList> list(const gcontact::ContactListArg& arg);
        GoogleTask<gcontact::ContactList>* list_Async(const gcontact::ContactListArg& arg);
        void list_AsyncCB(
            const gcontact::ContactListArg&,
            std::function<void(std::unique_ptr<gcontact::ContactList>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('update')


            Update contact

            */
        std::unique_ptr<gcontact::ContactList> update(const gcontact::UpdateContactArg& arg);
        GoogleTask<gcontact::ContactList>* update_Async(const gcontact::UpdateContactArg& arg);
        void update_AsyncCB(
            const gcontact::UpdateContactArg&,
            std::function<void(std::unique_ptr<gcontact::ContactList>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('uploadContactPhoto')


            Add/Update contact photo content.

            */
        void uploadContactPhoto(const gcontact::UploadPhotoArg& , QIODevice* readFrom);
        GoogleVoidTask* uploadContactPhoto_Async(const gcontact::UploadPhotoArg& arg, QIODevice* data);
        void uploadContactPhoto_AsyncCB(
            const gcontact::UploadPhotoArg&,
            QIODevice* data,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//ContactsRoutes

}//contacts
}//googleQt
