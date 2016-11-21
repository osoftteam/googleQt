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
        */

    public:
        MessagePartBody(){};

        MessagePartBody(const int& arg){ m_size = arg; };

    public:
            /**
                Total number of bytes in the body of the message part.
            */
        int size()const{return m_size;};
        MessagePartBody& setSize(const int& arg){m_size=arg;return *this;};

            /**
                The body data of a MIME message part. May be empty for MIME
                container types that have no message body or when the body data
                is sent as a separate attachment. An attachment ID is present if
                the body data is contained in a separate attachment.
            */
        QByteArray data()const{return m_data;};
        MessagePartBody& setData(const QByteArray& arg){m_data=arg;return *this;};

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
        static std::unique_ptr<MessagePartBody> EXAMPLE();
        #endif //API_QT_AUTOTEST


    protected:
            /**
                Total number of bytes in the body of the message part.
            */
        int m_size = {0};

            /**
                The body data of a MIME message part. May be empty for MIME
                container types that have no message body or when the body data
                is sent as a separate attachment. An attachment ID is present if
                the body data is contained in a separate attachment.
            */
        QByteArray m_data;

    };//MessagePartBody

}//messages
}//googleQt
