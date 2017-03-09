/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "gdrive/comments/CommentsComment.h"

namespace googleQt{
namespace comments{
    class CommentListResult{
        /**
            field: next_page_token: The page token for the next page of
                comments. This will be absent if the end of the comments list
                has been reached.
            field: kind: This is always drive#commentList.
        */

    public:
        CommentListResult():
        m_kind("drive#commentList")
        {};

        CommentListResult(const QString& arg):
        m_kind("drive#commentList")
        { m_nextPageToken = arg; };

    public:
            /**
                The page token for the next page of comments. This will be
                absent if the end of the comments list has been reached.
            */
        QString nextpagetoken()const{return m_nextPageToken;};

            /**
                This is always drive#commentList.
            */
        QString kind()const{return m_kind;};

        const std::list <Comment>& comments()const{return m_comments;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<CommentListResult>  create(const QByteArray& data);
            static std::unique_ptr<CommentListResult>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<CommentListResult> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The page token for the next page of comments. This will be
                absent if the end of the comments list has been reached.
            */
        QString m_nextPageToken;

            /**
                This is always drive#commentList.
            */
        QString m_kind;

        std::list <Comment> m_comments;

    };//CommentListResult

}//comments
}//googleQt
