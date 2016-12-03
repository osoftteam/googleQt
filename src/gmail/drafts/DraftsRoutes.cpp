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
    GOOGLE_BLOCKING_CALL(get_AsyncCB, DraftResource, arg);
}

GoogleTask<DraftResource>* DraftsRoutes::get_Async(const gmail::IdArg& arg)
{
    GoogleTask<DraftResource>* t = new GoogleTask<DraftResource>();
    m_end_point->getStyle<
        DraftResource,
        DraftResource::factory
        >
        (m_end_point->buildGmailUrl("drafts", arg),
        t);
    return t;
}

void DraftsRoutes::get_AsyncCB(
    const gmail::IdArg& arg,
    std::function<void(std::unique_ptr<DraftResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        DraftResource,
        DraftResource::factory
        >
        (m_end_point->buildGmailUrl("drafts", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<DraftListRes> DraftsRoutes::list(const gmail::DraftListArg& arg){
    GOOGLE_BLOCKING_CALL(list_AsyncCB, DraftListRes, arg);
}

GoogleTask<DraftListRes>* DraftsRoutes::list_Async(const gmail::DraftListArg& arg)
{
    GoogleTask<DraftListRes>* t = new GoogleTask<DraftListRes>();
    m_end_point->getStyle<
        DraftListRes,
        DraftListRes::factory
        >
        (m_end_point->buildGmailUrl("drafts", arg),
        t);
    return t;
}

void DraftsRoutes::list_AsyncCB(
    const gmail::DraftListArg& arg,
    std::function<void(std::unique_ptr<DraftListRes>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        DraftListRes,
        DraftListRes::factory
        >
        (m_end_point->buildGmailUrl("drafts", arg),
        completed_callback,
        failed_callback);
}

