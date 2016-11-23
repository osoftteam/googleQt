/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "drafts"
 www.prokarpaty.net
***********************************************************/

#include "gmail/drafts/DraftsRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace drafts;

std::unique_ptr<DraftResource> DraftsRoutes::get(const gmail::IdArg& arg){
    GOOGLE_BLOCKING_CALL(get_Async, DraftResource, arg);
}

void DraftsRoutes::get_Async(
    const gmail::IdArg& arg,
    std::function<void(std::unique_ptr<DraftResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<DraftResource>,
        DraftResource::factory
        >
        (m_end_point->buildGmailUrl("drafts", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<DraftListRes> DraftsRoutes::list(const gmail::DraftListArg& arg){
    GOOGLE_BLOCKING_CALL(list_Async, DraftListRes, arg);
}

void DraftsRoutes::list_Async(
    const gmail::DraftListArg& arg,
    std::function<void(std::unique_ptr<DraftListRes>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<DraftListRes>,
        DraftListRes::factory
        >
        (m_end_point->buildGmailUrl("drafts", arg),
        completed_callback,
        failed_callback);
}

