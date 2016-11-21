/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/messages/MessagesMessageResource.h"

namespace googleQt{
namespace messages{
    class MessageListRes{
        /**
            field: messages: List of messages.
            field: next_page_token: Token to retrieve the next page of results
                in the list.
            field: result_size_estimate: Estimated total number of results.
        */

    public:
        MessageListRes(){};

        MessageListRes(const std::list <MessageResource>& arg){ m_messages = arg; };

    public:
            /**
                List of messages.
            */
        const std::list <MessageResource>& messages()const{return m_messages;};
        MessageListRes& setMessages(const std::list <MessageResource>& arg){m_messages=arg;return *this;};

            /**
                Token to retrieve the next page of results in the list.
            */
        QString nextpagetoken()const{return m_nextPageToken;};
        MessageListRes& setNextpagetoken(const QString& arg){m_nextPageToken=arg;return *this;};

            /**
                Estimated total number of results.
            */
        int resultsizeestimate()const{return m_resultSizeEstimate;};
        MessageListRes& setResultsizeestimate(const int& arg){m_resultSizeEstimate=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<MessageListRes>  create(const QByteArray& data);
            static std::unique_ptr<MessageListRes>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<MessageListRes> EXAMPLE();
        #endif //API_QT_AUTOTEST


    protected:
            /**
                List of messages.
            */
        std::list <MessageResource> m_messages;

            /**
                Token to retrieve the next page of results in the list.
            */
        QString m_nextPageToken;

            /**
                Estimated total number of results.
            */
        int m_resultSizeEstimate = {0};

    };//MessageListRes

}//messages
}//googleQt
