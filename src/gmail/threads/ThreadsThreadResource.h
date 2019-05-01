/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "threads"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/messages/MessagesMessageResource.h"

namespace googleQt{
namespace threads{
    class ThreadResource{
        /**
            field: id: The unique ID of the thread.
            field: snipped: A short part of the message text.
            field: history_id: The ID of the last history record that modified
                this thread.
        */

    public:
        ThreadResource(){};

        ThreadResource(const QString& arg){ m_id = arg; };
        virtual ~ThreadResource(){};

    public:
            /**
                The unique ID of the thread.
            */
        QString id()const{return m_id;};
        ThreadResource& setId(const QString& arg){m_id=arg;return *this;};

            /**
                A short part of the message text.
            */
        QString snipped()const{return m_snipped;};
        ThreadResource& setSnipped(const QString& arg){m_snipped=arg;return *this;};

            /**
                The ID of the last history record that modified this thread.
            */
        quint64 historyid()const{return m_historyId;};
        ThreadResource& setHistoryid(const quint64& arg){m_historyId=arg;return *this;};

        const std::vector<messages::MessageResource>& messages()const{return m_messages;};
        ThreadResource& setMessages(const std::vector<messages::MessageResource>& arg){m_messages=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<ThreadResource>  create(const QByteArray& data);
            static std::unique_ptr<ThreadResource>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<ThreadResource> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The unique ID of the thread.
            */
        QString m_id;

            /**
                A short part of the message text.
            */
        QString m_snipped;

            /**
                The ID of the last history record that modified this thread.
            */
        quint64 m_historyId = {0};

        std::vector<messages::MessageResource> m_messages;

    };//ThreadResource

}//threads
}//googleQt
