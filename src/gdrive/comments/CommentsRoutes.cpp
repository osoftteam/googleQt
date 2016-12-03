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
    BODY_ARG_GBC(create_AsyncCB, Comment, arg, body);
}

GoogleTask<Comment>* CommentsRoutes::create_Async(const gdrive::CreateCommentArg& arg, const Comment& body)
{
    GoogleTask<Comment>* t = new GoogleTask<Comment>();
    m_end_point->postStyle<
        Comment,
        Comment::factory,
        Comment>
        (m_end_point->buildGdriveUrl("comments", arg),
        body,
        t);
    return t;
}

void CommentsRoutes::create_AsyncCB(
    const gdrive::CreateCommentArg& arg,
    const Comment& body,
    std::function<void(std::unique_ptr<Comment>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        Comment,
        Comment::factory,
        Comment
        >
        (m_end_point->buildGdriveUrl("comments", arg),
        body,
        completed_callback,
        failed_callback);
}

void CommentsRoutes::deleteOperation(const gdrive::DeleteCommentArg& arg ){
    VOID_RESULT_GBC(deleteOperation_AsyncCB, arg);
}

GoogleVoidTask* CommentsRoutes::deleteOperation_Async(const gdrive::DeleteCommentArg& arg)
{
    GoogleVoidTask* t = new GoogleVoidTask();
    m_end_point->deleteStyle
        (m_end_point->buildGdriveUrl("comments", arg),
        t);
    return t;
}

void CommentsRoutes::deleteOperation_AsyncCB(
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
    GOOGLE_BLOCKING_CALL(get_AsyncCB, Comment, arg);
}

GoogleTask<Comment>* CommentsRoutes::get_Async(const gdrive::GetCommentArg& arg)
{
    GoogleTask<Comment>* t = new GoogleTask<Comment>();
    m_end_point->getStyle<
        Comment,
        Comment::factory
        >
        (m_end_point->buildGdriveUrl("comments", arg),
        t);
    return t;
}

void CommentsRoutes::get_AsyncCB(
    const gdrive::GetCommentArg& arg,
    std::function<void(std::unique_ptr<Comment>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        Comment,
        Comment::factory
        >
        (m_end_point->buildGdriveUrl("comments", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<CommentListResult> CommentsRoutes::list(const gdrive::CommentListArg& arg){
    GOOGLE_BLOCKING_CALL(list_AsyncCB, CommentListResult, arg);
}

GoogleTask<CommentListResult>* CommentsRoutes::list_Async(const gdrive::CommentListArg& arg)
{
    GoogleTask<CommentListResult>* t = new GoogleTask<CommentListResult>();
    m_end_point->getStyle<
        CommentListResult,
        CommentListResult::factory
        >
        (m_end_point->buildGdriveUrl("comments", arg),
        t);
    return t;
}

void CommentsRoutes::list_AsyncCB(
    const gdrive::CommentListArg& arg,
    std::function<void(std::unique_ptr<CommentListResult>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        CommentListResult,
        CommentListResult::factory
        >
        (m_end_point->buildGdriveUrl("comments", arg),
        completed_callback,
        failed_callback);
}

