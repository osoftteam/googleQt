/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "GoogleRouteBase.h"
#include "gmail/messages/MessagesMessageListRes.h"
#include "gmail/messages/MessagesMessageResource.h"

namespace googleQt{
namespace messages{


    class MessagesRoutes: public GoogleRouteBase{
    public:
        MessagesRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('delete')


            Deletes message

            */
        void deleteOperation(const gmail::IdArg& );
        GoogleVoidTask* deleteOperation_Async(const gmail::IdArg& arg);
        void deleteOperation_AsyncCB(
            const gmail::IdArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('get')


            Get the specified message

            */
        std::unique_ptr<MessageResource> get(const gmail::IdArg& arg);
        GoogleTask<MessageResource>* get_Async(const gmail::IdArg& arg);
        void get_AsyncCB(
            const gmail::IdArg&,
            std::function<void(std::unique_ptr<MessageResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('importMessage')


            Imports a message into only this user's mailbox, with standard email
            delivery scanning and classification similar to receiving via SMTP.
            Does not send a message.

            */
        std::unique_ptr<MessageResource> importMessage(const gmail::ImportMessageArg& arg, const MessageResource& body);
        GoogleTask<MessageResource>* importMessage_Async(const gmail::ImportMessageArg& arg, const MessageResource& body);
        void importMessage_AsyncCB(
            const gmail::ImportMessageArg&,
            const MessageResource& body,
            std::function<void(std::unique_ptr<MessageResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('insert')


            Directly inserts a message into only this user's mailbox similar to
            IMAP APPEND, bypassing most scanning and classification.

            */
        std::unique_ptr<MessageResource> insert(const gmail::InsertMessageArg& arg, const MessageResource& body);
        GoogleTask<MessageResource>* insert_Async(const gmail::InsertMessageArg& arg, const MessageResource& body);
        void insert_AsyncCB(
            const gmail::InsertMessageArg&,
            const MessageResource& body,
            std::function<void(std::unique_ptr<MessageResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Get messages list

            */
        std::unique_ptr<MessageListRes> list(const gmail::ListArg& arg);
        GoogleTask<MessageListRes>* list_Async(const gmail::ListArg& arg);
        void list_AsyncCB(
            const gmail::ListArg&,
            std::function<void(std::unique_ptr<MessageListRes>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('send')


            Sends the specified message to the recipients in the To, Cc, and Bcc
            headers.

            */
        std::unique_ptr<MessageResource> send(const gmail::SendMessageArg& arg, const MessageResource& body);
        GoogleTask<MessageResource>* send_Async(const gmail::SendMessageArg& arg, const MessageResource& body);
        void send_AsyncCB(
            const gmail::SendMessageArg&,
            const MessageResource& body,
            std::function<void(std::unique_ptr<MessageResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('trash')


            Moves the specified message to the trash.

            */
        void trash(const gmail::TrashMessageArg& );
        GoogleVoidTask* trash_Async(const gmail::TrashMessageArg& arg);
        void trash_AsyncCB(
            const gmail::TrashMessageArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('untrash')


            Removes the specified message from the trash.

            */
        void untrash(const gmail::UntrashMessageArg& );
        GoogleVoidTask* untrash_Async(const gmail::UntrashMessageArg& arg);
        void untrash_AsyncCB(
            const gmail::UntrashMessageArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//MessagesRoutes

}//messages
}//googleQt
