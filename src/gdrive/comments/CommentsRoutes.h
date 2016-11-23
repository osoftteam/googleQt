/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
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
        void create_Async(
            const gdrive::CreateCommentArg&,
            const Comment& body,
            std::function<void(std::unique_ptr<Comment>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('delete')


            Delete a Comment

            */
        void deleteOperation(const gdrive::DeleteCommentArg& );
        void deleteOperation_Async(
            const gdrive::DeleteCommentArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('get')


            Get comment metadata.

            */
        std::unique_ptr<Comment> get(const gdrive::GetCommentArg& arg);
        void get_Async(
            const gdrive::GetCommentArg&,
            std::function<void(std::unique_ptr<Comment>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Lists a file's comments.

            */
        std::unique_ptr<CommentListResult> list(const gdrive::CommentListArg& arg);
        void list_Async(
            const gdrive::CommentListArg&,
            std::function<void(std::unique_ptr<CommentListResult>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//CommentsRoutes

}//comments
}//googleQt
