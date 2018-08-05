/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "users"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/users/UsersRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace users;

std::unique_ptr<GetProfileResponse> UsersRoutes::profile(void){
    return profile_Async()->waitForResultAndRelease();
}

GoogleTask<GetProfileResponse>* UsersRoutes::profile_Async()
{
    GoogleTask<GetProfileResponse>* t = m_end_point->produceTask<GetProfileResponse>();
    m_end_point->getStyle<
        GetProfileResponse,
        GetProfileResponse::factory
        >
        (m_end_point->buildGmailUrl("users", VoidType::instance()),
        t);
    return t;
}

void UsersRoutes::profile_AsyncCB(
    std::function<void(std::unique_ptr<GetProfileResponse>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        GetProfileResponse,
        GetProfileResponse::factory
        >
        (m_end_point->buildGmailUrl("users", VoidType::instance()),
        completed_callback,
        failed_callback);
}

