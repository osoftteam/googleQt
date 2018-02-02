/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/messages/MessagesMessagePayloadHeader.h"
#include "gmail/messages/MessagesMessageMimeBody.h"
#include "gmail/messages/MessagesMessagePart.h"

namespace googleQt{
namespace messages{
    class MessagePayload{
        /**
            field: part_id: The immutable ID of the message part.
            field: mime_type: The MIME type of the message part.
            field: filename: The filename of the attachment. Only present if
                this message part represents an attachment.
            field: headers: List of headers on this message part. For the
                top-level message part, representing the entire message payload,
                it will contain the standard RFC 2822 email headers such as To,
                From, and Subject.
            field: body: The message part body for this part, which may be empty
                for container MIME message parts.
            field: parts: The list of parts in the message
        */

    public:
        MessagePayload(){};

        MessagePayload(const QString& arg){ m_partId = arg; };
        virtual ~MessagePayload(){};

    public:
            /**
                The immutable ID of the message part.
            */
        QString partid()const{return m_partId;};
        MessagePayload& setPartid(const QString& arg){m_partId=arg;return *this;};

            /**
                The MIME type of the message part.
            */
        QString mimetype()const{return m_mimeType;};
        MessagePayload& setMimetype(const QString& arg){m_mimeType=arg;return *this;};

            /**
                The filename of the attachment. Only present if this message
                part represents an attachment.
            */
        QString filename()const{return m_filename;};
        MessagePayload& setFilename(const QString& arg){m_filename=arg;return *this;};

            /**
                List of headers on this message part. For the top-level message
                part, representing the entire message payload, it will contain
                the standard RFC 2822 email headers such as To, From, and
                Subject.
            */
        const std::list <MessagePayloadHeader>& headers()const{return m_headers;};
        MessagePayload& setHeaders(const std::list <MessagePayloadHeader>& arg){m_headers=arg;return *this;};

            /**
                The message part body for this part, which may be empty for
                container MIME message parts.
            */
        const MessageMimeBody& body()const{return m_body;};
        MessagePayload& setBody(const MessageMimeBody& arg){m_body=arg;return *this;};

            /**
                The list of parts in the message
            */
        const std::list <MessagePart>& parts()const{return m_parts;};
        MessagePayload& setParts(const std::list <MessagePart>& arg){m_parts=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<MessagePayload>  create(const QByteArray& data);
            static std::unique_ptr<MessagePayload>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<MessagePayload> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The immutable ID of the message part.
            */
        QString m_partId;

            /**
                The MIME type of the message part.
            */
        QString m_mimeType;

            /**
                The filename of the attachment. Only present if this message
                part represents an attachment.
            */
        QString m_filename;

            /**
                List of headers on this message part. For the top-level message
                part, representing the entire message payload, it will contain
                the standard RFC 2822 email headers such as To, From, and
                Subject.
            */
        std::list <MessagePayloadHeader> m_headers;

            /**
                The message part body for this part, which may be empty for
                container MIME message parts.
            */
        MessageMimeBody m_body;

            /**
                The list of parts in the message
            */
        std::list <MessagePart> m_parts;

    };//MessagePayload

}//messages
}//googleQt
