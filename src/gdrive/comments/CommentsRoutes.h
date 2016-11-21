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

            /**
            ApiRoute('delete')


            Delete a Comment

            */
        void deleteOperation(const gdrive::DeleteCommentArg& );

            /**
            ApiRoute('get')


            Get comment metadata.

            */
        std::unique_ptr<Comment> get(const gdrive::GetCommentArg& arg);

            /**
            ApiRoute('list')


            Lists a file's comments.

            */
        std::unique_ptr<CommentListResult> list(const gdrive::CommentListArg& arg);

    protected:
    };//CommentsRoutes

}//comments
}//googleQt
