/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "labels"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"

namespace googleQt{
namespace labels{
    class LabelResource{
        /**
            field: id: The immutable ID of the label.
            field: name: The display name of the label.
            field: message_list_visibility: The visibility of messages with this
                label in the message list in the Gmail web interface. Acceptable
                values: 'hide' 'show'
            field: label_list_visibility: The visibility of the label in the
                label list in the Gmail web interface. Acceptable values:
                'labelHide' 'labelShow' 'labelShowIfUnread'
            field: type: The owner type for the label. Acceptable values:
                'system' 'user'
            field: messages_total: The total number of messages with the label.
            field: messages_unread: The number of unread messages with the
                label.
            field: threads_total: The number of unread threads with the label.
        */

    public:
        LabelResource(){};

        LabelResource(const QString& arg){ m_id = arg; };

    public:
            /**
                The immutable ID of the label.
            */
        QString id()const{return m_id;};
        LabelResource& setId(const QString& arg){m_id=arg;return *this;};

            /**
                The display name of the label.
            */
        QString name()const{return m_name;};
        LabelResource& setName(const QString& arg){m_name=arg;return *this;};

            /**
                The visibility of messages with this label in the message list
                in the Gmail web interface. Acceptable values: 'hide' 'show'
            */
        QString messagelistvisibility()const{return m_messageListVisibility;};
        LabelResource& setMessagelistvisibility(const QString& arg){m_messageListVisibility=arg;return *this;};

            /**
                The visibility of the label in the label list in the Gmail web
                interface. Acceptable values: 'labelHide' 'labelShow'
                'labelShowIfUnread'
            */
        QString labellistvisibility()const{return m_labelListVisibility;};
        LabelResource& setLabellistvisibility(const QString& arg){m_labelListVisibility=arg;return *this;};

            /**
                The owner type for the label. Acceptable values: 'system' 'user'
            */
        QString type()const{return m_type;};
        LabelResource& setType(const QString& arg){m_type=arg;return *this;};

            /**
                The total number of messages with the label.
            */
        int64_t messagestotal()const{return m_messagesTotal;};
        LabelResource& setMessagestotal(const int64_t& arg){m_messagesTotal=arg;return *this;};

            /**
                The number of unread messages with the label.
            */
        int64_t messagesunread()const{return m_messagesUnread;};
        LabelResource& setMessagesunread(const int64_t& arg){m_messagesUnread=arg;return *this;};

            /**
                The number of unread threads with the label.
            */
        int64_t threadstotal()const{return m_threadsTotal;};
        LabelResource& setThreadstotal(const int64_t& arg){m_threadsTotal=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<LabelResource>  create(const QByteArray& data);
            static std::unique_ptr<LabelResource>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<LabelResource> EXAMPLE();
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The immutable ID of the label.
            */
        QString m_id;

            /**
                The display name of the label.
            */
        QString m_name;

            /**
                The visibility of messages with this label in the message list
                in the Gmail web interface. Acceptable values: 'hide' 'show'
            */
        QString m_messageListVisibility;

            /**
                The visibility of the label in the label list in the Gmail web
                interface. Acceptable values: 'labelHide' 'labelShow'
                'labelShowIfUnread'
            */
        QString m_labelListVisibility;

            /**
                The owner type for the label. Acceptable values: 'system' 'user'
            */
        QString m_type;

            /**
                The total number of messages with the label.
            */
        int64_t m_messagesTotal = {0};

            /**
                The number of unread messages with the label.
            */
        int64_t m_messagesUnread = {0};

            /**
                The number of unread threads with the label.
            */
        int64_t m_threadsTotal = {0};

    };//LabelResource

}//labels
}//googleQt
