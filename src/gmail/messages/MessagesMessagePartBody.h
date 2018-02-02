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
    class MessagePartBody{
        /**
            field: size: Total number of bytes in the body of the message part.
            field: data: The body data of a MIME message part. May be empty for
                MIME container types that have no message body or when the body
                data is sent as a separate attachment. An attachment ID is
                present if the body data is contained in a separate attachment.
            field: attachment_id: When present, contains the ID of an external
                attachment that can be retrieved in a separate
                messages.attachments.get request. When not present, the entire
                content of the message part body is contained in the data field.
        */

    public:
        MessagePartBody(){};

        MessagePartBody(const quint64& arg){ m_size = arg; };
        virtual ~MessagePartBody(){};

    public:
            /**
                Total number of bytes in the body of the message part.
            */
        quint64 size()const{return m_size;};
        MessagePartBody& setSize(const quint64& arg){m_size=arg;return *this;};

            /**
                The body data of a MIME message part. May be empty for MIME
                container types that have no message body or when the body data
                is sent as a separate attachment. An attachment ID is present if
                the body data is contained in a separate attachment.
            */
        QByteArray data()const{return m_data;};
        MessagePartBody& setData(const QByteArray& arg){m_data=arg;return *this;};

            /**
                When present, contains the ID of an external attachment that can
                be retrieved in a separate messages.attachments.get request.
                When not present, the entire content of the message part body is
                contained in the data field.
            */
        QString attachmentid()const{return m_attachmentId;};
        MessagePartBody& setAttachmentid(const QString& arg){m_attachmentId=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<MessagePartBody>  create(const QByteArray& data);
            static std::unique_ptr<MessagePartBody>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<MessagePartBody> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                Total number of bytes in the body of the message part.
            */
        quint64 m_size = {0};

            /**
                The body data of a MIME message part. May be empty for MIME
                container types that have no message body or when the body data
                is sent as a separate attachment. An attachment ID is present if
                the body data is contained in a separate attachment.
            */
        QByteArray m_data;

            /**
                When present, contains the ID of an external attachment that can
                be retrieved in a separate messages.attachments.get request.
                When not present, the entire content of the message part body is
                contained in the data field.
            */
        QString m_attachmentId;

    };//MessagePartBody

}//messages
}//googleQt
