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
    m_end_point->deleteStyle<std::unique_ptr<VoidType>, VoidType, NotAnException>(m_end_point->buildGmailUrl("messages", arg) );
}

std::unique_ptr<MessageResource> MessagesRoutes::get(const gmail::IdArg& arg){
    return m_end_point->getStyle<std::unique_ptr<MessageResource>, MessageResource::factory, NotAnException>(m_end_point->buildGmailUrl("messages", arg));
}

std::unique_ptr<MessageResource> MessagesRoutes::importMessage(const gmail::ImportMessageArg& arg, const MessageResource& body){
    return m_end_point->rfc822UploadStyle<std::unique_ptr<MessageResource>, MessageResource::factory, MessageResource>(m_end_point->buildGmailUploadlUrl("messages", arg), body);
}

std::unique_ptr<MessageResource> MessagesRoutes::insert(const gmail::InsertMessageArg& arg, const MessageResource& body){
    return m_end_point->rfc822UploadStyle<std::unique_ptr<MessageResource>, MessageResource::factory, MessageResource>(m_end_point->buildGmailUploadlUrl("messages", arg), body);
}

std::unique_ptr<MessageListRes> MessagesRoutes::list(const gmail::ListArg& arg){
    return m_end_point->getStyle<std::unique_ptr<MessageListRes>, MessageListRes::factory, NotAnException>(m_end_point->buildGmailUrl("messages", arg));
}

std::unique_ptr<MessageResource> MessagesRoutes::send(const gmail::SendMessageArg& arg, const MessageResource& body){
    return m_end_point->rfc822UploadStyle<std::unique_ptr<MessageResource>, MessageResource::factory, MessageResource>(m_end_point->buildGmailUploadlUrl("messages", arg), body);
}

void MessagesRoutes::trash(const gmail::TrashMessageArg& arg ){
    m_end_point->postStyle<std::unique_ptr<VoidType>, VoidType, NotAnException>(m_end_point->buildGmailUrl("messages", arg) );
}

void MessagesRoutes::untrash(const gmail::UntrashMessageArg& arg ){
    m_end_point->postStyle<std::unique_ptr<VoidType>, VoidType, NotAnException>(m_end_point->buildGmailUrl("messages", arg) );
}

