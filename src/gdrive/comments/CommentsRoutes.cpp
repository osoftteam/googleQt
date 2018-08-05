/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/comments/CommentsRoutes.h"
#include "Endpoint.h"
#include "gdrive/GdriveRoutes.h"
using namespace googleQt;
using namespace comments;

std::unique_ptr<Comment> CommentsRoutes::create(const gdrive::CreateCommentArg& arg, const Comment& body){
    return create_Async(arg, body)->waitForResultAndRelease();
}

GoogleTask<Comment>* CommentsRoutes::create_Async(const gdrive::CreateCommentArg& arg, const Comment& body)
{
    GoogleTask<Comment>* t = m_end_point->produceTask<Comment>();
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
    deleteOperation_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* CommentsRoutes::deleteOperation_Async(const gdrive::DeleteCommentArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
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
    return get_Async(arg)->waitForResultAndRelease();
}

GoogleTask<Comment>* CommentsRoutes::get_Async(const gdrive::GetCommentArg& arg)
{
    GoogleTask<Comment>* t = m_end_point->produceTask<Comment>();
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
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<CommentListResult>* CommentsRoutes::list_Async(const gdrive::CommentListArg& arg)
{
    GoogleTask<CommentListResult>* t = m_end_point->produceTask<CommentListResult>();
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

