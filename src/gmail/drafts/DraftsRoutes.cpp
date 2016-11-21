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
    return m_end_point->getStyle<std::unique_ptr<DraftResource>, DraftResource::factory, NotAnException>(m_end_point->buildGmailUrl("drafts", arg));
}

std::unique_ptr<DraftListRes> DraftsRoutes::list(const gmail::DraftListArg& arg){
    return m_end_point->getStyle<std::unique_ptr<DraftListRes>, DraftListRes::factory, NotAnException>(m_end_point->buildGmailUrl("drafts", arg));
}

