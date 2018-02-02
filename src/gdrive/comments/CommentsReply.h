/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "gdrive/comments/CommentsUser.h"

namespace googleQt{
namespace comments{
    class Reply{
        /**
            field: id: The ID of the reply.
            field: kind: This is always drive#reply
            field: created_time: The time at which the reply was created (RFC
                3339 date-time).
            field: modified_time: The last time the reply was modified (RFC 3339
                date-time).
            field: author: The user who created the reply.
            field: html_content: The content of the reply with HTML formatting.
            field: content: The plain text content of the reply. This field is
                used for setting the content, while htmlContent should be
                displayed. This is required on creates if no action is
                specified.
            field: deleted: Whether the reply has been deleted. A deleted reply
                has no content.
            field: action: The action the reply performed to the parent comment.
                Valid values are: resolve reopen
        */

    public:
        Reply():
        m_kind("drive#reply")
        {};

        Reply(const QString& arg):
        m_kind("drive#reply")
        { m_id = arg; };
        virtual ~Reply(){};

    public:
            /**
                The ID of the reply.
            */
        QString id()const{return m_id;};
        Reply& setId(const QString& arg){m_id=arg;return *this;};

            /**
                This is always drive#reply
            */
        QString kind()const{return m_kind;};
        Reply& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                The time at which the reply was created (RFC 3339 date-time).
            */
        QDateTime createdtime()const{return m_createdTime;};
        Reply& setCreatedtime(const QDateTime& arg){m_createdTime=arg;return *this;};

            /**
                The last time the reply was modified (RFC 3339 date-time).
            */
        QDateTime modifiedtime()const{return m_modifiedTime;};
        Reply& setModifiedtime(const QDateTime& arg){m_modifiedTime=arg;return *this;};

            /**
                The user who created the reply.
            */
        const User& author()const{return m_author;};
        Reply& setAuthor(const User& arg){m_author=arg;return *this;};

            /**
                The content of the reply with HTML formatting.
            */
        QString htmlcontent()const{return m_htmlContent;};
        Reply& setHtmlcontent(const QString& arg){m_htmlContent=arg;return *this;};

            /**
                The plain text content of the reply. This field is used for
                setting the content, while htmlContent should be displayed. This
                is required on creates if no action is specified.
            */
        QString content()const{return m_content;};
        Reply& setContent(const QString& arg){m_content=arg;return *this;};

            /**
                Whether the reply has been deleted. A deleted reply has no
                content.
            */
        bool deleted()const{return m_deleted;};
        Reply& setDeleted(const bool& arg){m_deleted=arg;return *this;};

            /**
                The action the reply performed to the parent comment. Valid
                values are: resolve reopen
            */
        QString action()const{return m_action;};
        Reply& setAction(const QString& arg){m_action=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<Reply>  create(const QByteArray& data);
            static std::unique_ptr<Reply>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<Reply> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The ID of the reply.
            */
        QString m_id;

            /**
                This is always drive#reply
            */
        QString m_kind;

            /**
                The time at which the reply was created (RFC 3339 date-time).
            */
        QDateTime m_createdTime;

            /**
                The last time the reply was modified (RFC 3339 date-time).
            */
        QDateTime m_modifiedTime;

            /**
                The user who created the reply.
            */
        User m_author;

            /**
                The content of the reply with HTML formatting.
            */
        QString m_htmlContent;

            /**
                The plain text content of the reply. This field is used for
                setting the content, while htmlContent should be displayed. This
                is required on creates if no action is specified.
            */
        QString m_content;

            /**
                Whether the reply has been deleted. A deleted reply has no
                content.
            */
        bool m_deleted;

            /**
                The action the reply performed to the parent comment. Valid
                values are: resolve reopen
            */
        QString m_action;

    };//Reply

}//comments
}//googleQt
