/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "GoogleRouteBase.h"
#include "gdrive/comments/CommentsComment.h"
#include "gdrive/comments/CommentsCommentListResult.h"

namespace googleQt{
namespace comments{


    class CommentsRoutes: public GoogleRouteBase{
    public:
        CommentsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('create')


            Create new Comment

            */
        std::unique_ptr<Comment> create(const gdrive::CreateCommentArg& arg, const Comment& body);
        GoogleTask<Comment>* create_Async(const gdrive::CreateCommentArg& arg, const Comment& body);
        void create_AsyncCB(
            const gdrive::CreateCommentArg&,
            const Comment& body,
            std::function<void(std::unique_ptr<Comment>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('delete')


            Delete a Comment

            */
        void deleteOperation(const gdrive::DeleteCommentArg& );
        GoogleVoidTask* deleteOperation_Async(const gdrive::DeleteCommentArg& arg);
        void deleteOperation_AsyncCB(
            const gdrive::DeleteCommentArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('get')


            Get comment metadata.

            */
        std::unique_ptr<Comment> get(const gdrive::GetCommentArg& arg);
        GoogleTask<Comment>* get_Async(const gdrive::GetCommentArg& arg);
        void get_AsyncCB(
            const gdrive::GetCommentArg&,
            std::function<void(std::unique_ptr<Comment>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Lists a file's comments.

            */
        std::unique_ptr<CommentListResult> list(const gdrive::CommentListArg& arg);
        GoogleTask<CommentListResult>* list_Async(const gdrive::CommentListArg& arg);
        void list_AsyncCB(
            const gdrive::CommentListArg&,
            std::function<void(std::unique_ptr<CommentListResult>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//CommentsRoutes

}//comments
}//googleQt
