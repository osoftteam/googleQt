/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "revisions"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "GoogleRouteBase.h"
#include "gdrive/revisions/RevisionsRevisionResource.h"
#include "gdrive/revisions/RevisionsRevisionResourceCollection.h"

namespace googleQt{
namespace revisions{


    class RevisionsRoutes: public GoogleRouteBase{
    public:
        RevisionsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('delete')


            Permanently deletes a revision. This method is only applicable to
            files with binary content in Drive.

            */
        void deleteOperation(const gdrive::DeleteRevisionArg& );
        GoogleVoidTask* deleteOperation_Async(const gdrive::DeleteRevisionArg& arg);
        void deleteOperation_AsyncCB(
            const gdrive::DeleteRevisionArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('get')


            Gets a revision's metadata or content by ID

            */
        std::unique_ptr<RevisionResource> get(const gdrive::GetRevisionArg& arg);
        GoogleTask<RevisionResource>* get_Async(const gdrive::GetRevisionArg& arg);
        void get_AsyncCB(
            const gdrive::GetRevisionArg&,
            std::function<void(std::unique_ptr<RevisionResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Lists a file's revisions

            */
        std::unique_ptr<RevisionResourceCollection> list(const gdrive::ListRevisionArg& arg);
        GoogleTask<RevisionResourceCollection>* list_Async(const gdrive::ListRevisionArg& arg);
        void list_AsyncCB(
            const gdrive::ListRevisionArg&,
            std::function<void(std::unique_ptr<RevisionResourceCollection>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('update')


            Updates a revision with patch semantics.

            */
        std::unique_ptr<RevisionResource> update(const gdrive::UpdateRevisionArg& arg);
        GoogleTask<RevisionResource>* update_Async(const gdrive::UpdateRevisionArg& arg);
        void update_AsyncCB(
            const gdrive::UpdateRevisionArg&,
            std::function<void(std::unique_ptr<RevisionResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//RevisionsRoutes

}//revisions
}//googleQt
