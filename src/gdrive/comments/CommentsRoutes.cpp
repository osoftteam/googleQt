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
    return m_end_point->postStyle<std::unique_ptr<Comment>, Comment::factory, Comment>(m_end_point->buildGdriveUrl("comments", arg), body);
}

void CommentsRoutes::deleteOperation(const gdrive::DeleteCommentArg& arg ){
    m_end_point->deleteStyle<std::unique_ptr<VoidType>, VoidType, NotAnException>(m_end_point->buildGdriveUrl("comments", arg) );
}

std::unique_ptr<Comment> CommentsRoutes::get(const gdrive::GetCommentArg& arg){
    return m_end_point->getStyle<std::unique_ptr<Comment>, Comment::factory, NotAnException>(m_end_point->buildGdriveUrl("comments", arg));
}

std::unique_ptr<CommentListResult> CommentsRoutes::list(const gdrive::CommentListArg& arg){
    return m_end_point->getStyle<std::unique_ptr<CommentListResult>, CommentListResult::factory, NotAnException>(m_end_point->buildGdriveUrl("comments", arg));
}

