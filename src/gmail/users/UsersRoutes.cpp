/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "users"
 www.prokarpaty.net
***********************************************************/

#include "gmail/users/UsersRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace users;

std::unique_ptr<GetProfileResponse> UsersRoutes::profile(void){
    return m_end_point->getStyle<std::unique_ptr<GetProfileResponse>, GetProfileResponse::factory, NotAnException >(m_end_point->buildGmailUrl("users", VoidType::instance()));
}

