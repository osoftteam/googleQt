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
#include "gdrive/comments/CommentsUser.h"
#include "gdrive/comments/CommentsQuotedFileContent.h"
#include "gdrive/comments/CommentsReply.h"

namespace googleQt{
namespace comments{
    class Comment{
        /**
            field: id: The ID of the comment.
            field: kind: This is always drive#comment.
            field: created_time: The time at which the comment was created (RFC
                3339 date-time).
            field: modified_time: The last time the comment or any of its
                replies was modified (RFC 3339 date-time).
            field: author: The user who created the comment.
            field: html_content: The content of the comment with HTML
                formatting.
            field: content: The plain text content of the comment. This field is
                used for setting the content, while htmlContent should be
                displayed.
            field: deleted: Whether the comment has been deleted. A deleted
                comment has no content.
            field: resolved: Whether the comment has been resolved by one of its
                replies.
            field: quoted_file_content: The file content to which the comment
                refers, typically within the anchor region. For a text file, for
                example, this would be the text at the location of the comment.
            field: anchor: A region of the document represented as a JSON
                string. See anchor documentation for details on how to define
                and interpret anchor properties.
            field: replies: The full list of replies to the comment in
                chronological order.
        */

    public:
        Comment():
        m_kind("drive#comment")
        ,m_deleted(false)
        ,m_resolved(false)
        {};

        Comment(const QString& arg):
        m_kind("drive#comment")
        ,m_deleted(false)
        ,m_resolved(false)
        { m_id = arg; };
        virtual ~Comment(){};

    public:
            /**
                The ID of the comment.
            */
        QString id()const{return m_id;};
        Comment& setId(const QString& arg){m_id=arg;return *this;};

            /**
                This is always drive#comment.
            */
        QString kind()const{return m_kind;};
        Comment& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                The time at which the comment was created (RFC 3339 date-time).
            */
        QDateTime createdtime()const{return m_createdTime;};
        Comment& setCreatedtime(const QDateTime& arg){m_createdTime=arg;return *this;};

            /**
                The last time the comment or any of its replies was modified
                (RFC 3339 date-time).
            */
        QDateTime modifiedtime()const{return m_modifiedTime;};
        Comment& setModifiedtime(const QDateTime& arg){m_modifiedTime=arg;return *this;};

            /**
                The user who created the comment.
            */
        const User& author()const{return m_author;};
        Comment& setAuthor(const User& arg){m_author=arg;return *this;};

            /**
                The content of the comment with HTML formatting.
            */
        QString htmlcontent()const{return m_htmlContent;};
        Comment& setHtmlcontent(const QString& arg){m_htmlContent=arg;return *this;};

            /**
                The plain text content of the comment. This field is used for
                setting the content, while htmlContent should be displayed.
            */
        QString content()const{return m_content;};
        Comment& setContent(const QString& arg){m_content=arg;return *this;};

            /**
                Whether the comment has been deleted. A deleted comment has no
                content.
            */
        bool deleted()const{return m_deleted;};
        Comment& setDeleted(const bool& arg){m_deleted=arg;return *this;};

            /**
                Whether the comment has been resolved by one of its replies.
            */
        bool resolved()const{return m_resolved;};
        Comment& setResolved(const bool& arg){m_resolved=arg;return *this;};

            /**
                The file content to which the comment refers, typically within
                the anchor region. For a text file, for example, this would be
                the text at the location of the comment.
            */
        const QuotedFileContent& quotedfilecontent()const{return m_quotedFileContent;};
        Comment& setQuotedfilecontent(const QuotedFileContent& arg){m_quotedFileContent=arg;return *this;};

            /**
                A region of the document represented as a JSON string. See
                anchor documentation for details on how to define and interpret
                anchor properties.
            */
        QString anchor()const{return m_anchor;};
        Comment& setAnchor(const QString& arg){m_anchor=arg;return *this;};

            /**
                The full list of replies to the comment in chronological order.
            */
        const std::list <Reply>& replies()const{return m_replies;};
        Comment& setReplies(const std::list <Reply>& arg){m_replies=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<Comment>  create(const QByteArray& data);
            static std::unique_ptr<Comment>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<Comment> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The ID of the comment.
            */
        QString m_id;

            /**
                This is always drive#comment.
            */
        QString m_kind;

            /**
                The time at which the comment was created (RFC 3339 date-time).
            */
        QDateTime m_createdTime;

            /**
                The last time the comment or any of its replies was modified
                (RFC 3339 date-time).
            */
        QDateTime m_modifiedTime;

            /**
                The user who created the comment.
            */
        User m_author;

            /**
                The content of the comment with HTML formatting.
            */
        QString m_htmlContent;

            /**
                The plain text content of the comment. This field is used for
                setting the content, while htmlContent should be displayed.
            */
        QString m_content;

            /**
                Whether the comment has been deleted. A deleted comment has no
                content.
            */
        bool m_deleted;

            /**
                Whether the comment has been resolved by one of its replies.
            */
        bool m_resolved;

            /**
                The file content to which the comment refers, typically within
                the anchor region. For a text file, for example, this would be
                the text at the location of the comment.
            */
        QuotedFileContent m_quotedFileContent;

            /**
                A region of the document represented as a JSON string. See
                anchor documentation for details on how to define and interpret
                anchor properties.
            */
        QString m_anchor;

            /**
                The full list of replies to the comment in chronological order.
            */
        std::list <Reply> m_replies;

    };//Comment

}//comments
}//googleQt
