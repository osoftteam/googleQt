/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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
            ApiRoute('batchModify')


            Modifies the labels on list of specified messages.

            */
        void batchModify(const gmail::BatchModifyMessageArg& );
        GoogleVoidTask* batchModify_Async(const gmail::BatchModifyMessageArg& arg);
        void batchModify_AsyncCB(
            const gmail::BatchModifyMessageArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

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
            ApiRoute('modify')


            Modifies the labels on the specified message.

            */
        std::unique_ptr<MessageResource> modify(const gmail::ModifyMessageArg& arg);
        GoogleTask<MessageResource>* modify_Async(const gmail::ModifyMessageArg& arg);
        void modify_AsyncCB(
            const gmail::ModifyMessageArg&,
            std::function<void(std::unique_ptr<MessageResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('send')


            Sends the specified message to the recipients in the To, Cc, and Bcc
            headers.

            */
        std::unique_ptr<MessageResource> send(const gmail::SendMimeMessageArg& arg);
        GoogleTask<MessageResource>* send_Async(const gmail::SendMimeMessageArg& arg);
        void send_AsyncCB(
            const gmail::SendMimeMessageArg&,
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
