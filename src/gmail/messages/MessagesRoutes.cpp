/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
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

std::unique_ptr<MessageResource> MessagesRoutes::importMessage(const gmail::ImportMessageArg& arg, const MessageResource& body){
    return importMessage_Async(arg, body)->waitForResultAndRelease();
}

GoogleTask<MessageResource>* MessagesRoutes::importMessage_Async(const gmail::ImportMessageArg& arg, const MessageResource& body)
{
    GoogleTask<MessageResource>* t = m_end_point->produceTask<MessageResource>();
    m_end_point->rfc822UploadStyle<
        MessageResource,
        MessageResource::factory,
        MessageResource>
        (m_end_point->buildGmailUploadlUrl("messages", arg),
        body,
        t);
    return t;
}

void MessagesRoutes::importMessage_AsyncCB(
    const gmail::ImportMessageArg& arg,
    const MessageResource& body,
    std::function<void(std::unique_ptr<MessageResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->rfc822UploadStyle
        <
        MessageResource,
        MessageResource::factory,
        MessageResource
        >
        (m_end_point->buildGmailUploadlUrl("messages", arg),
        body,
        completed_callback,
        failed_callback);
}

std::unique_ptr<MessageResource> MessagesRoutes::insert(const gmail::InsertMessageArg& arg, const MessageResource& body){
    return insert_Async(arg, body)->waitForResultAndRelease();
}

GoogleTask<MessageResource>* MessagesRoutes::insert_Async(const gmail::InsertMessageArg& arg, const MessageResource& body)
{
    GoogleTask<MessageResource>* t = m_end_point->produceTask<MessageResource>();
    m_end_point->rfc822UploadStyle<
        MessageResource,
        MessageResource::factory,
        MessageResource>
        (m_end_point->buildGmailUploadlUrl("messages", arg),
        body,
        t);
    return t;
}

void MessagesRoutes::insert_AsyncCB(
    const gmail::InsertMessageArg& arg,
    const MessageResource& body,
    std::function<void(std::unique_ptr<MessageResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->rfc822UploadStyle
        <
        MessageResource,
        MessageResource::factory,
        MessageResource
        >
        (m_end_point->buildGmailUploadlUrl("messages", arg),
        body,
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

std::unique_ptr<MessageResource> MessagesRoutes::send(const gmail::SendMessageArg& arg, const MessageResource& body){
    return send_Async(arg, body)->waitForResultAndRelease();
}

GoogleTask<MessageResource>* MessagesRoutes::send_Async(const gmail::SendMessageArg& arg, const MessageResource& body)
{
    GoogleTask<MessageResource>* t = m_end_point->produceTask<MessageResource>();
    m_end_point->rfc822UploadStyle<
        MessageResource,
        MessageResource::factory,
        MessageResource>
        (m_end_point->buildGmailUploadlUrl("messages", arg),
        body,
        t);
    return t;
}

void MessagesRoutes::send_AsyncCB(
    const gmail::SendMessageArg& arg,
    const MessageResource& body,
    std::function<void(std::unique_ptr<MessageResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->rfc822UploadStyle
        <
        MessageResource,
        MessageResource::factory,
        MessageResource
        >
        (m_end_point->buildGmailUploadlUrl("messages", arg),
        body,
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

