/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/messages/MessagesMessagePayload.h"

namespace googleQt{
namespace messages{
    class MessageResource{
        /**
            field: id: The immutable ID of the message.
            field: thread_id: The ID of the thread the message belongs to.
            field: label_ids: List of IDs of labels applied to this message.
            field: snippet: A short part of the message text.
            field: history_id: The ID of the last history record that modified
                this message.
            field: internal_date: The internal message creation timestamp (epoch
                ms), which determines ordering in the inbox. For normal
                SMTP-received email, this represents the time the message was
                originally accepted by Google, which is more reliable than the
                Date header. However, for API-migrated mail, it can be
                configured by client to be based on the Date header.
            field: payload: The parsed email structure in the message parts.
            field: size_estimate: Estimated size in bytes of the message.
            field: raw: The entire email message in an RFC 2822 formatted and
                base64url encoded string. Returned in messages.get and
                drafts.get responses when the format=RAW parameter is supplied.
        */

    public:
        MessageResource(){};

        MessageResource(const QString& arg){ m_id = arg; };

    public:
            /**
                The immutable ID of the message.
            */
        QString id()const{return m_id;};
        MessageResource& setId(const QString& arg){m_id=arg;return *this;};

            /**
                The ID of the thread the message belongs to.
            */
        QString threadid()const{return m_threadId;};
        MessageResource& setThreadid(const QString& arg){m_threadId=arg;return *this;};

            /**
                List of IDs of labels applied to this message.
            */
        const std::list <QString>& labelids()const{return m_labelIds;};
        MessageResource& setLabelids(const std::list <QString>& arg){m_labelIds=arg;return *this;};

            /**
                A short part of the message text.
            */
        QString snippet()const{return m_snippet;};
        MessageResource& setSnippet(const QString& arg){m_snippet=arg;return *this;};

            /**
                The ID of the last history record that modified this message.
            */
        qint64 historyid()const{return m_historyId;};
        MessageResource& setHistoryid(const qint64& arg){m_historyId=arg;return *this;};

            /**
                The internal message creation timestamp (epoch ms), which
                determines ordering in the inbox. For normal SMTP-received
                email, this represents the time the message was originally
                accepted by Google, which is more reliable than the Date header.
                However, for API-migrated mail, it can be configured by client
                to be based on the Date header.
            */
        qint64 internaldate()const{return m_internalDate;};
        MessageResource& setInternaldate(const qint64& arg){m_internalDate=arg;return *this;};

            /**
                The parsed email structure in the message parts.
            */
        const MessagePayload& payload()const{return m_payload;};
        MessageResource& setPayload(const MessagePayload& arg){m_payload=arg;return *this;};

            /**
                Estimated size in bytes of the message.
            */
        qint64 sizeestimate()const{return m_sizeEstimate;};
        MessageResource& setSizeestimate(const qint64& arg){m_sizeEstimate=arg;return *this;};

            /**
                The entire email message in an RFC 2822 formatted and base64url
                encoded string. Returned in messages.get and drafts.get
                responses when the format=RAW parameter is supplied.
            */
        QByteArray raw()const{return m_raw;};
        MessageResource& setRaw(const QByteArray& arg){m_raw=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<MessageResource>  create(const QByteArray& data);
            static std::unique_ptr<MessageResource>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<MessageResource> EXAMPLE(int context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The immutable ID of the message.
            */
        QString m_id;

            /**
                The ID of the thread the message belongs to.
            */
        QString m_threadId;

            /**
                List of IDs of labels applied to this message.
            */
        std::list <QString> m_labelIds;

            /**
                A short part of the message text.
            */
        QString m_snippet;

            /**
                The ID of the last history record that modified this message.
            */
        qint64 m_historyId = {0};

            /**
                The internal message creation timestamp (epoch ms), which
                determines ordering in the inbox. For normal SMTP-received
                email, this represents the time the message was originally
                accepted by Google, which is more reliable than the Date header.
                However, for API-migrated mail, it can be configured by client
                to be based on the Date header.
            */
        qint64 m_internalDate = {0};

            /**
                The parsed email structure in the message parts.
            */
        MessagePayload m_payload;

            /**
                Estimated size in bytes of the message.
            */
        qint64 m_sizeEstimate = {0};

            /**
                The entire email message in an RFC 2822 formatted and base64url
                encoded string. Returned in messages.get and drafts.get
                responses when the format=RAW parameter is supplied.
            */
        QByteArray m_raw;

    };//MessageResource

}//messages
}//googleQt
