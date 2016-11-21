/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "users"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "GoogleRouteBase.h"
#include "gmail/users/UsersGetProfileResponse.h"

namespace googleQt{
namespace users{


    class UsersRoutes: public GoogleRouteBase{
    public:
        UsersRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('profile')


            Returns user profile information.

            */
        std::unique_ptr<GetProfileResponse> profile(void);

    protected:
    };//UsersRoutes

}//users
}//googleQt
