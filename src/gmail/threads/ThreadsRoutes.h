/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "threads"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "GoogleRouteBase.h"
#include "gmail/threads/ThreadsThreadListRes.h"
#include "gmail/threads/ThreadsThreadResource.h"

namespace googleQt{
namespace threads{


    class ThreadsRoutes: public GoogleRouteBase{
    public:
        ThreadsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('get')


            Get the specified message

            */
        std::unique_ptr<ThreadResource> get(const gmail::IdArg& arg);
        GoogleTask<ThreadResource>* get_Async(const gmail::IdArg& arg);
        void get_AsyncCB(
            const gmail::IdArg&,
            std::function<void(std::unique_ptr<ThreadResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

        std::unique_ptr<ThreadListRes> list(const gmail::ListArg& arg);
        GoogleTask<ThreadListRes>* list_Async(const gmail::ListArg& arg);
        void list_AsyncCB(
            const gmail::ListArg&,
            std::function<void(std::unique_ptr<ThreadListRes>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('modify')


            Modifies the labels on the specified thread.

            */
        std::unique_ptr<ThreadResource> modify(const gmail::ModifyMessageArg& arg);
        GoogleTask<ThreadResource>* modify_Async(const gmail::ModifyMessageArg& arg);
        void modify_AsyncCB(
            const gmail::ModifyMessageArg&,
            std::function<void(std::unique_ptr<ThreadResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('trash')


            Moves the specified thread to the trash.

            */
        void trash(const gmail::TrashMessageArg& );
        GoogleVoidTask* trash_Async(const gmail::TrashMessageArg& arg);
        void trash_AsyncCB(
            const gmail::TrashMessageArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('untrash')


            Removes the specified thread from the trash.

            */
        void untrash(const gmail::UntrashMessageArg& );
        GoogleVoidTask* untrash_Async(const gmail::UntrashMessageArg& arg);
        void untrash_AsyncCB(
            const gmail::UntrashMessageArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//ThreadsRoutes

}//threads
}//googleQt
