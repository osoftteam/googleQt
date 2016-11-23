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
    VOID_RESULT_GBC(deleteOperation_Async, arg);
}

void MessagesRoutes::deleteOperation_Async(
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
    GOOGLE_BLOCKING_CALL(get_Async, MessageResource, arg);
}

void MessagesRoutes::get_Async(
    const gmail::IdArg& arg,
    std::function<void(std::unique_ptr<MessageResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<MessageResource>,
        MessageResource::factory
        >
        (m_end_point->buildGmailUrl("messages", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<MessageResource> MessagesRoutes::importMessage(const gmail::ImportMessageArg& arg, const MessageResource& body){
    BODY_ARG_GBC(importMessage_Async, MessageResource, arg, body);
}

void MessagesRoutes::importMessage_Async(
    const gmail::ImportMessageArg& arg,
    const MessageResource& body,
    std::function<void(std::unique_ptr<MessageResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->rfc822UploadStyle
        <
        std::unique_ptr<MessageResource>,
        MessageResource::factory,
        MessageResource
        >
        (m_end_point->buildGmailUploadlUrl("messages", arg),
        body,
        completed_callback,
        failed_callback);
}

std::unique_ptr<MessageResource> MessagesRoutes::insert(const gmail::InsertMessageArg& arg, const MessageResource& body){
    BODY_ARG_GBC(insert_Async, MessageResource, arg, body);
}

void MessagesRoutes::insert_Async(
    const gmail::InsertMessageArg& arg,
    const MessageResource& body,
    std::function<void(std::unique_ptr<MessageResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->rfc822UploadStyle
        <
        std::unique_ptr<MessageResource>,
        MessageResource::factory,
        MessageResource
        >
        (m_end_point->buildGmailUploadlUrl("messages", arg),
        body,
        completed_callback,
        failed_callback);
}

std::unique_ptr<MessageListRes> MessagesRoutes::list(const gmail::ListArg& arg){
    GOOGLE_BLOCKING_CALL(list_Async, MessageListRes, arg);
}

void MessagesRoutes::list_Async(
    const gmail::ListArg& arg,
    std::function<void(std::unique_ptr<MessageListRes>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<MessageListRes>,
        MessageListRes::factory
        >
        (m_end_point->buildGmailUrl("messages", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<MessageResource> MessagesRoutes::send(const gmail::SendMessageArg& arg, const MessageResource& body){
    BODY_ARG_GBC(send_Async, MessageResource, arg, body);
}

void MessagesRoutes::send_Async(
    const gmail::SendMessageArg& arg,
    const MessageResource& body,
    std::function<void(std::unique_ptr<MessageResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->rfc822UploadStyle
        <
        std::unique_ptr<MessageResource>,
        MessageResource::factory,
        MessageResource
        >
        (m_end_point->buildGmailUploadlUrl("messages", arg),
        body,
        completed_callback,
        failed_callback);
}

void MessagesRoutes::trash(const gmail::TrashMessageArg& arg ){
    VOID_RESULT_GBC(trash_Async, arg);
}

void MessagesRoutes::trash_Async(
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
    VOID_RESULT_GBC(untrash_Async, arg);
}

void MessagesRoutes::untrash_Async(
    const gmail::UntrashMessageArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        (m_end_point->buildGmailUrl("messages", arg),
        completed_callback,
        failed_callback);
}

