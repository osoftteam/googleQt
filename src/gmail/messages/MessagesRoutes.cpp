/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/messages/MessagesRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace messages;

void MessagesRoutes::deleteOperation(const gmail::IdArg& arg ){
    deleteOperation_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* MessagesRoutes::deleteOperation_Async(const gmail::IdArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->deleteStyle
        (m_end_point->buildGmailUrl("messages", arg),
        t);
    return t;
}

void MessagesRoutes::deleteOperation_AsyncCB(
    const gmail::IdArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->deleteStyle
        (m_end_point->buildGmailUrl("messages", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<MessageResource> MessagesRoutes::get(const gmail::IdArg& arg){
    return get_Async(arg)->waitForResultAndRelease();
}

GoogleTask<MessageResource>* MessagesRoutes::get_Async(const gmail::IdArg& arg)
{
    GoogleTask<MessageResource>* t = m_end_point->produceTask<MessageResource>();
    m_end_point->getStyle<
        MessageResource,
        MessageResource::factory
        >
        (m_end_point->buildGmailUrl("messages", arg),
        t);
    return t;
}

void MessagesRoutes::get_AsyncCB(
    const gmail::IdArg& arg,
    std::function<void(std::unique_ptr<MessageResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        MessageResource,
        MessageResource::factory
        >
        (m_end_point->buildGmailUrl("messages", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<MessageListRes> MessagesRoutes::list(const gmail::ListArg& arg){
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<MessageListRes>* MessagesRoutes::list_Async(const gmail::ListArg& arg)
{
    GoogleTask<MessageListRes>* t = m_end_point->produceTask<MessageListRes>();
    m_end_point->getStyle<
        MessageListRes,
        MessageListRes::factory
        >
        (m_end_point->buildGmailUrl("messages", arg),
        t);
    return t;
}

void MessagesRoutes::list_AsyncCB(
    const gmail::ListArg& arg,
    std::function<void(std::unique_ptr<MessageListRes>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        MessageListRes,
        MessageListRes::factory
        >
        (m_end_point->buildGmailUrl("messages", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<MessageResource> MessagesRoutes::modify(const gmail::ModifyMessageArg& arg){
    return modify_Async(arg)->waitForResultAndRelease();
}

GoogleTask<MessageResource>* MessagesRoutes::modify_Async(const gmail::ModifyMessageArg& arg)
{
    GoogleTask<MessageResource>* t = m_end_point->produceTask<MessageResource>();
    m_end_point->postStyleB<
        MessageResource,
        MessageResource::factory
        ,gmail::ModifyMessageArg>
        (m_end_point->buildGmailUrl("messages", arg),
        arg,
        t);
    return t;
}

void MessagesRoutes::modify_AsyncCB(
    const gmail::ModifyMessageArg& arg,
    std::function<void(std::unique_ptr<MessageResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyleB
        <
        MessageResource,
        MessageResource::factory
        , gmail::ModifyMessageArg
        >
        (m_end_point->buildGmailUrl("messages", arg),
        arg,
        completed_callback,
        failed_callback);
}

std::unique_ptr<MessageResource> MessagesRoutes::send(const gmail::SendMimeMessageArg& arg){
    return send_Async(arg)->waitForResultAndRelease();
}

GoogleTask<MessageResource>* MessagesRoutes::send_Async(const gmail::SendMimeMessageArg& arg)
{
    GoogleTask<MessageResource>* t = m_end_point->produceTask<MessageResource>();
    m_end_point->postStyleB<
        MessageResource,
        MessageResource::factory
        ,gmail::SendMimeMessageArg>
        (m_end_point->buildGmailUrl("messages", arg),
        arg,
        t);
    return t;
}

void MessagesRoutes::send_AsyncCB(
    const gmail::SendMimeMessageArg& arg,
    std::function<void(std::unique_ptr<MessageResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyleB
        <
        MessageResource,
        MessageResource::factory
        , gmail::SendMimeMessageArg
        >
        (m_end_point->buildGmailUrl("messages", arg),
        arg,
        completed_callback,
        failed_callback);
}

void MessagesRoutes::trash(const gmail::TrashMessageArg& arg ){
    trash_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* MessagesRoutes::trash_Async(const gmail::TrashMessageArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->postStyle
        (m_end_point->buildGmailUrl("messages", arg),
        t);
    return t;
}

void MessagesRoutes::trash_AsyncCB(
    const gmail::TrashMessageArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        (m_end_point->buildGmailUrl("messages", arg),
        completed_callback,
        failed_callback);
}

void MessagesRoutes::untrash(const gmail::UntrashMessageArg& arg ){
    untrash_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* MessagesRoutes::untrash_Async(const gmail::UntrashMessageArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->postStyle
        (m_end_point->buildGmailUrl("messages", arg),
        t);
    return t;
}

void MessagesRoutes::untrash_AsyncCB(
    const gmail::UntrashMessageArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        (m_end_point->buildGmailUrl("messages", arg),
        completed_callback,
        failed_callback);
}

