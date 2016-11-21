/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "history"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/messages/MessagesMessageResource.h"
#include "gmail/history/HistoryLabelHistoryRecord.h"

namespace googleQt{
namespace history{
    class HistoryRecord{
        /**
            field: id: The mailbox sequence ID.
            field: messages: List of messages changed in this history record.
                The fields for specific change types, such as messagesAdded may
                duplicate messages in this field. We recommend using the
                specific change-type fields instead of this.
            field: messages_added: Messages added to the mailbox in this history
                record.
            field: messages_deleted: Messages deleted (not Trashed) from the
                mailbox in this history record.
            field: labels_added: Labels added to messages in this history
                record.
            field: labels_removed: Labels removed from messages in this history
                record.
        */

    public:
        HistoryRecord(){};

        HistoryRecord(const QString& arg){ m_id = arg; };

    public:
            /**
                The mailbox sequence ID.
            */
        QString id()const{return m_id;};
        HistoryRecord& setId(const QString& arg){m_id=arg;return *this;};

            /**
                List of messages changed in this history record. The fields for
                specific change types, such as messagesAdded may duplicate
                messages in this field. We recommend using the specific
                change-type fields instead of this.
            */
        const std::list <messages::MessageResource>& messages()const{return m_messages;};
        HistoryRecord& setMessages(const std::list <messages::MessageResource>& arg){m_messages=arg;return *this;};

            /**
                Messages added to the mailbox in this history record.
            */
        const std::list <messages::MessageResource>& messagesadded()const{return m_messagesAdded;};
        HistoryRecord& setMessagesadded(const std::list <messages::MessageResource>& arg){m_messagesAdded=arg;return *this;};

            /**
                Messages deleted (not Trashed) from the mailbox in this history
                record.
            */
        const std::list <messages::MessageResource>& messagesdeleted()const{return m_messagesDeleted;};
        HistoryRecord& setMessagesdeleted(const std::list <messages::MessageResource>& arg){m_messagesDeleted=arg;return *this;};

            /**
                Labels added to messages in this history record.
            */
        const std::list <LabelHistoryRecord>& labelsadded()const{return m_labelsAdded;};
        HistoryRecord& setLabelsadded(const std::list <LabelHistoryRecord>& arg){m_labelsAdded=arg;return *this;};

            /**
                Labels removed from messages in this history record.
            */
        const std::list <LabelHistoryRecord>& labelsremoved()const{return m_labelsRemoved;};
        HistoryRecord& setLabelsremoved(const std::list <LabelHistoryRecord>& arg){m_labelsRemoved=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<HistoryRecord>  create(const QByteArray& data);
            static std::unique_ptr<HistoryRecord>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<HistoryRecord> EXAMPLE();
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The mailbox sequence ID.
            */
        QString m_id;

            /**
                List of messages changed in this history record. The fields for
                specific change types, such as messagesAdded may duplicate
                messages in this field. We recommend using the specific
                change-type fields instead of this.
            */
        std::list <messages::MessageResource> m_messages;

            /**
                Messages added to the mailbox in this history record.
            */
        std::list <messages::MessageResource> m_messagesAdded;

            /**
                Messages deleted (not Trashed) from the mailbox in this history
                record.
            */
        std::list <messages::MessageResource> m_messagesDeleted;

            /**
                Labels added to messages in this history record.
            */
        std::list <LabelHistoryRecord> m_labelsAdded;

            /**
                Labels removed from messages in this history record.
            */
        std::list <LabelHistoryRecord> m_labelsRemoved;

    };//HistoryRecord

}//history
}//googleQt
