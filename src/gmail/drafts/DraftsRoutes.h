/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "drafts"
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

            /**
            ApiRoute('list')


            Get drafts list

            */
        std::unique_ptr<DraftListRes> list(const gmail::DraftListArg& arg);

    protected:
    };//DraftsRoutes

}//drafts
}//googleQt
