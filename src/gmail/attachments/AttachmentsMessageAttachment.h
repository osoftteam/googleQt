/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "attachments"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"

namespace googleQt{
namespace attachments{
    class MessageAttachment{
        /**
            field: attachment_id: When present, contains the ID of an external
                attachment that can be retrieved in a separate
                messages.attachments.get request. When not present, the entire
                content of the message part body is contained in the data field.
            field: data: The body data of a MIME message part as a base64url
                encoded string. May be empty for MIME container types that have
                no message body or when the body data is sent as a separate
                attachment. An attachment ID is present if the body data is
                contained in a separate attachment.
            field: size: Number of bytes for the message part data (encoding
                notwithstanding).
        */

    public:
        MessageAttachment(){};

        MessageAttachment(const QString& arg){ m_attachmentId = arg; };

    public:
            /**
                When present, contains the ID of an external attachment that can
                be retrieved in a separate messages.attachments.get request.
                When not present, the entire content of the message part body is
                contained in the data field.
            */
        QString attachmentid()const{return m_attachmentId;};
        MessageAttachment& setAttachmentid(const QString& arg){m_attachmentId=arg;return *this;};

            /**
                The body data of a MIME message part as a base64url encoded
                string. May be empty for MIME container types that have no
                message body or when the body data is sent as a separate
                attachment. An attachment ID is present if the body data is
                contained in a separate attachment.
            */
        QByteArray data()const{return m_data;};
        MessageAttachment& setData(const QByteArray& arg){m_data=arg;return *this;};

            /**
                Number of bytes for the message part data (encoding
                notwithstanding).
            */
        quint64 size()const{return m_size;};
        MessageAttachment& setSize(const quint64& arg){m_size=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<MessageAttachment>  create(const QByteArray& data);
            static std::unique_ptr<MessageAttachment>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<MessageAttachment> EXAMPLE(int context_index, int parent_context_index);
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
                The body data of a MIME message part as a base64url encoded
                string. May be empty for MIME container types that have no
                message body or when the body data is sent as a separate
                attachment. An attachment ID is present if the body data is
                contained in a separate attachment.
            */
        QByteArray m_data;

            /**
                Number of bytes for the message part data (encoding
                notwithstanding).
            */
        quint64 m_size = {0};

    };//MessageAttachment

}//attachments
}//googleQt
