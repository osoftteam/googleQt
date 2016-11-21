/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "users"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"

namespace googleQt{
namespace users{
    class GetProfileResponse{
        /**
            field: email_address: The user's email address.
            field: messages_total: The total number of messages in the mailbox.
            field: threads_total: The total number of threads in the mailbox.
            field: history_id: The ID of the mailbox's current history record.
        */

    public:
        GetProfileResponse(){};

        GetProfileResponse(const QString& arg){ m_emailAddress = arg; };

    public:
            /**
                The user's email address.
            */
        QString emailaddress()const{return m_emailAddress;};
        GetProfileResponse& setEmailaddress(const QString& arg){m_emailAddress=arg;return *this;};

            /**
                The total number of messages in the mailbox.
            */
        int messagestotal()const{return m_messagesTotal;};
        GetProfileResponse& setMessagestotal(const int& arg){m_messagesTotal=arg;return *this;};

            /**
                The total number of threads in the mailbox.
            */
        int threadstotal()const{return m_threadsTotal;};
        GetProfileResponse& setThreadstotal(const int& arg){m_threadsTotal=arg;return *this;};

            /**
                The ID of the mailbox's current history record.
            */
        int historyid()const{return m_historyId;};
        GetProfileResponse& setHistoryid(const int& arg){m_historyId=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<GetProfileResponse>  create(const QByteArray& data);
            static std::unique_ptr<GetProfileResponse>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<GetProfileResponse> EXAMPLE();
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The user's email address.
            */
        QString m_emailAddress;

            /**
                The total number of messages in the mailbox.
            */
        int m_messagesTotal = {0};

            /**
                The total number of threads in the mailbox.
            */
        int m_threadsTotal = {0};

            /**
                The ID of the mailbox's current history record.
            */
        int m_historyId = {0};

    };//GetProfileResponse

}//users
}//googleQt
