/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "threads"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/threads/ThreadsRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace threads;

std::unique_ptr<ThreadResource> ThreadsRoutes::get(const gmail::IdArg& arg){
    return get_Async(arg)->waitForResultAndRelease();
}

GoogleTask<ThreadResource>* ThreadsRoutes::get_Async(const gmail::IdArg& arg)
{
    GoogleTask<ThreadResource>* t = m_end_point->produceTask<ThreadResource>();
    m_end_point->getStyle<
        ThreadResource,
        ThreadResource::factory
        >
        (m_end_point->buildGmailUrl("threads", arg),
        t);
    return t;
}

void ThreadsRoutes::get_AsyncCB(
    const gmail::IdArg& arg,
    std::function<void(std::unique_ptr<ThreadResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        ThreadResource,
        ThreadResource::factory
        >
        (m_end_point->buildGmailUrl("threads", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<ThreadListRes> ThreadsRoutes::list(const gmail::ListArg& arg){
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<ThreadListRes>* ThreadsRoutes::list_Async(const gmail::ListArg& arg)
{
    GoogleTask<ThreadListRes>* t = m_end_point->produceTask<ThreadListRes>();
    m_end_point->getStyle<
        ThreadListRes,
        ThreadListRes::factory
        >
        (m_end_point->buildGmailUrl("threads", arg),
        t);
    return t;
}

void ThreadsRoutes::list_AsyncCB(
    const gmail::ListArg& arg,
    std::function<void(std::unique_ptr<ThreadListRes>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        ThreadListRes,
        ThreadListRes::factory
        >
        (m_end_point->buildGmailUrl("threads", arg),
        completed_callback,
        failed_callback);
}

