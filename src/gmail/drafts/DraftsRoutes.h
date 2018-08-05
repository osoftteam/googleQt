/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "drafts"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "GoogleRouteBase.h"
#include "gmail/drafts/DraftsDraftListRes.h"
#include "gmail/drafts/DraftsDraftResource.h"

namespace googleQt{
namespace drafts{


    class DraftsRoutes: public GoogleRouteBase{
    public:
        DraftsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('get')


            Get the specified draft

            */
        std::unique_ptr<DraftResource> get(const gmail::IdArg& arg);
        GoogleTask<DraftResource>* get_Async(const gmail::IdArg& arg);
        void get_AsyncCB(
            const gmail::IdArg&,
            std::function<void(std::unique_ptr<DraftResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Get drafts list

            */
        std::unique_ptr<DraftListRes> list(const gmail::DraftListArg& arg);
        GoogleTask<DraftListRes>* list_Async(const gmail::DraftListArg& arg);
        void list_AsyncCB(
            const gmail::DraftListArg&,
            std::function<void(std::unique_ptr<DraftListRes>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//DraftsRoutes

}//drafts
}//googleQt
