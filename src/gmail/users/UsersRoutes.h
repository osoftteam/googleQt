/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "users"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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
        std::unique_ptr<GetProfileResponse> profile();
        GoogleTask<GetProfileResponse>* profile_Async();
        void profile_AsyncCB(
            std::function<void(std::unique_ptr<GetProfileResponse>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//UsersRoutes

}//users
}//googleQt
