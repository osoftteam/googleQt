/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "threads"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "GoogleRouteBase.h"
#include "gmail/threads/ThreadsThreadListRes.h"
#include "gmail/threads/ThreadsThreadResource.h"

namespace googleQt{
namespace threads{


    class ThreadsRoutes: public GoogleRouteBase{
    public:
        ThreadsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('get')


            Get the specified message

            */
        std::unique_ptr<ThreadResource> get(const gmail::IdArg& arg);

        std::unique_ptr<ThreadListRes> list(const gmail::ListArg& arg);

    protected:
    };//ThreadsRoutes

}//threads
}//googleQt
