/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "about"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/about/AboutRoutes.h"
#include "Endpoint.h"
#include "gdrive/GdriveRoutes.h"
using namespace googleQt;
using namespace about;

std::unique_ptr<AboutResource> AboutRoutes::get(const gdrive::AboutArg& arg){
    return m_end_point->getStyle<std::unique_ptr<AboutResource>, AboutResource::factory, NotAnException>(m_end_point->buildGdriveUrl("about", arg));
}

