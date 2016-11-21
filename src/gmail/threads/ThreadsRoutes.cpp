/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "threads"
 www.prokarpaty.net
***********************************************************/

#include "gmail/threads/ThreadsRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace threads;

std::unique_ptr<ThreadResource> ThreadsRoutes::get(const gmail::IdArg& arg){
    return m_end_point->getStyle<std::unique_ptr<ThreadResource>, ThreadResource::factory, NotAnException>(m_end_point->buildGmailUrl("threads", arg));
}

std::unique_ptr<ThreadListRes> ThreadsRoutes::list(const gmail::ListArg& arg){
    return m_end_point->getStyle<std::unique_ptr<ThreadListRes>, ThreadListRes::factory, NotAnException>(m_end_point->buildGmailUrl("threads", arg));
}

