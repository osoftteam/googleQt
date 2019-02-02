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

std::unique_ptr<ThreadResource> ThreadsRoutes::modify(const gmail::ModifyMessageArg& arg){
    return modify_Async(arg)->waitForResultAndRelease();
}

GoogleTask<ThreadResource>* ThreadsRoutes::modify_Async(const gmail::ModifyMessageArg& arg)
{
    GoogleTask<ThreadResource>* t = m_end_point->produceTask<ThreadResource>();
    m_end_point->postStyleB<
        ThreadResource,
        ThreadResource::factory
        ,gmail::ModifyMessageArg>
        (m_end_point->buildGmailUrl("threads", arg),
        arg,
        t);
    return t;
}

void ThreadsRoutes::modify_AsyncCB(
    const gmail::ModifyMessageArg& arg,
    std::function<void(std::unique_ptr<ThreadResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyleB
        <
        ThreadResource,
        ThreadResource::factory
        , gmail::ModifyMessageArg
        >
        (m_end_point->buildGmailUrl("threads", arg),
        arg,
        completed_callback,
        failed_callback);
}

void ThreadsRoutes::trash(const gmail::TrashMessageArg& arg ){
    trash_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* ThreadsRoutes::trash_Async(const gmail::TrashMessageArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->postStyle
        (m_end_point->buildGmailUrl("threads", arg),
        t);
    return t;
}

void ThreadsRoutes::trash_AsyncCB(
    const gmail::TrashMessageArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        (m_end_point->buildGmailUrl("threads", arg),
        completed_callback,
        failed_callback);
}

void ThreadsRoutes::untrash(const gmail::UntrashMessageArg& arg ){
    untrash_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* ThreadsRoutes::untrash_Async(const gmail::UntrashMessageArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->postStyle
        (m_end_point->buildGmailUrl("threads", arg),
        t);
    return t;
}

void ThreadsRoutes::untrash_AsyncCB(
    const gmail::UntrashMessageArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        (m_end_point->buildGmailUrl("threads", arg),
        completed_callback,
        failed_callback);
}

