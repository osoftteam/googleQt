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

            /**
            ApiRoute('get')


            Get the specified message

            */
        std::unique_ptr<MessageResource> get(const gmail::IdArg& arg);

            /**
            ApiRoute('importMessage')


            Imports a message into only this user's mailbox, with standard email
            delivery scanning and classification similar to receiving via SMTP.
            Does not send a message.

            */
        std::unique_ptr<MessageResource> importMessage(const gmail::ImportMessageArg& arg, const MessageResource& body);

            /**
            ApiRoute('insert')


            Directly inserts a message into only this user's mailbox similar to
            IMAP APPEND, bypassing most scanning and classification.

            */
        std::unique_ptr<MessageResource> insert(const gmail::InsertMessageArg& arg, const MessageResource& body);

            /**
            ApiRoute('list')


            Get messages list

            */
        std::unique_ptr<MessageListRes> list(const gmail::ListArg& arg);

            /**
            ApiRoute('send')


            Sends the specified message to the recipients in the To, Cc, and Bcc
            headers.

            */
        std::unique_ptr<MessageResource> send(const gmail::SendMessageArg& arg, const MessageResource& body);

            /**
            ApiRoute('trash')


            Moves the specified message to the trash.

            */
        void trash(const gmail::TrashMessageArg& );

            /**
            ApiRoute('untrash')


            Removes the specified message from the trash.

            */
        void untrash(const gmail::UntrashMessageArg& );

    protected:
    };//MessagesRoutes

}//messages
}//googleQt
