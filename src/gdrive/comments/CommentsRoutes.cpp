/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/comments/CommentsRoutes.h"
#include "Endpoint.h"
#include "gdrive/GdriveRoutes.h"
using namespace googleQt;
using namespace comments;

std::unique_ptr<Comment> CommentsRoutes::create(const gdrive::CreateCommentArg& arg, const Comment& body){
    BODY_ARG_GBC(create_Async, Comment, arg, body);
}

void CommentsRoutes::create_Async(
    const gdrive::CreateCommentArg& arg,
    const Comment& body,
    std::function<void(std::unique_ptr<Comment>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        std::unique_ptr<Comment>,
        Comment::factory,
        Comment
        >
        (m_end_point->buildGdriveUrl("comments", arg),
        body,
        completed_callback,
        failed_callback);
}

void CommentsRoutes::deleteOperation(const gdrive::DeleteCommentArg& arg ){
    VOID_RESULT_GBC(deleteOperation_Async, arg);
}

void CommentsRoutes::deleteOperation_Async(
    const gdrive::DeleteCommentArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->deleteStyle
        (m_end_point->buildGdriveUrl("comments", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<Comment> CommentsRoutes::get(const gdrive::GetCommentArg& arg){
    GOOGLE_BLOCKING_CALL(get_Async, Comment, arg);
}

void CommentsRoutes::get_Async(
    const gdrive::GetCommentArg& arg,
    std::function<void(std::unique_ptr<Comment>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<Comment>,
        Comment::factory
        >
        (m_end_point->buildGdriveUrl("comments", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<CommentListResult> CommentsRoutes::list(const gdrive::CommentListArg& arg){
    GOOGLE_BLOCKING_CALL(list_Async, CommentListResult, arg);
}

void CommentsRoutes::list_Async(
    const gdrive::CommentListArg& arg,
    std::function<void(std::unique_ptr<CommentListResult>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<CommentListResult>,
        CommentListResult::factory
        >
        (m_end_point->buildGdriveUrl("comments", arg),
        completed_callback,
        failed_callback);
}

