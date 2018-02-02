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
    class MessagePayloadHeader{
        /**
            field: name: The name of the header before the : separator. For
                example, To.
            field: value: The value of the header after the : separator. For
                example, someuser@example.com.
        */

    public:
        MessagePayloadHeader(){};

        MessagePayloadHeader(const QString& arg){ m_name = arg; };
        virtual ~MessagePayloadHeader(){};

    public:
            /**
                The name of the header before the : separator. For example, To.
            */
        QString name()const{return m_name;};
        MessagePayloadHeader& setName(const QString& arg){m_name=arg;return *this;};

            /**
                The value of the header after the : separator. For example,
                someuser@example.com.
            */
        QString value()const{return m_value;};
        MessagePayloadHeader& setValue(const QString& arg){m_value=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<MessagePayloadHeader>  create(const QByteArray& data);
            static std::unique_ptr<MessagePayloadHeader>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<MessagePayloadHeader> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The name of the header before the : separator. For example, To.
            */
        QString m_name;

            /**
                The value of the header after the : separator. For example,
                someuser@example.com.
            */
        QString m_value;

    };//MessagePayloadHeader

}//messages
}//googleQt
