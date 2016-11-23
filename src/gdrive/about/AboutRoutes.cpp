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
    GOOGLE_BLOCKING_CALL(get_Async, AboutResource, arg);
}

void AboutRoutes::get_Async(
    const gdrive::AboutArg& arg,
    std::function<void(std::unique_ptr<AboutResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<AboutResource>,
        AboutResource::factory
        >
        (m_end_point->buildGdriveUrl("about", arg),
        completed_callback,
        failed_callback);
}

