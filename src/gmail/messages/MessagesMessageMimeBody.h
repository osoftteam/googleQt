/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"

namespace googleQt{
namespace messages{
    class MessageMimeBody{
        /**
            field: attachment_id: When present, contains the ID of an external
                attachment that can be retrieved in a separate
                messages.attachments.get request. When not present, the entire
                content of the message part body is contained in the data field.
            field: size: Total number of bytes in the body of the message part.
            field: data: The body data of a MIME message part. May be empty for
                MIME container types that have no message body or when the body
                data is sent as a separate attachment. An attachment ID is
                present if the body data is contained in a separate attachment.
        */

    public:
        MessageMimeBody(){};

        MessageMimeBody(const QString& arg){ m_attachmentId = arg; };

    public:
            /**
                When present, contains the ID of an external attachment that can
                be retrieved in a separate messages.attachments.get request.
                When not present, the entire content of the message part body is
                contained in the data field.
            */
        QString attachmentid()const{return m_attachmentId;};
        MessageMimeBody& setAttachmentid(const QString& arg){m_attachmentId=arg;return *this;};

            /**
                Total number of bytes in the body of the message part.
            */
        int64_t size()const{return m_size;};
        MessageMimeBody& setSize(const int64_t& arg){m_size=arg;return *this;};

            /**
                The body data of a MIME message part. May be empty for MIME
                container types that have no message body or when the body data
                is sent as a separate attachment. An attachment ID is present if
                the body data is contained in a separate attachment.
            */
        QByteArray data()const{return m_data;};
        MessageMimeBody& setData(const QByteArray& arg){m_data=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<MessageMimeBody>  create(const QByteArray& data);
            static std::unique_ptr<MessageMimeBody>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<MessageMimeBody> EXAMPLE();
        #endif //API_QT_AUTOTEST


    protected:
            /**
                When present, contains the ID of an external attachment that can
                be retrieved in a separate messages.attachments.get request.
                When not present, the entire content of the message part body is
                contained in the data field.
            */
        QString m_attachmentId;

            /**
                Total number of bytes in the body of the message part.
            */
        int64_t m_size = {0};

            /**
                The body data of a MIME message part. May be empty for MIME
                container types that have no message body or when the body data
                is sent as a separate attachment. An attachment ID is present if
                the body data is contained in a separate attachment.
            */
        QByteArray m_data;

    };//MessageMimeBody

}//messages
}//googleQt
