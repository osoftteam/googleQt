/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "errors"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"

namespace googleQt{
namespace errors{
    class ErrorPart{
        /**
            field: domain: domain - global
            field: reason: reason - badRequest, authError etc.
            field: message: message details
        */

    public:
        ErrorPart(){};

        ErrorPart(const QString& arg){ m_domain = arg; };

    public:
            /**
                domain - global
            */
        QString domain()const{return m_domain;};
        ErrorPart& setDomain(const QString& arg){m_domain=arg;return *this;};

            /**
                reason - badRequest, authError etc.
            */
        QString reason()const{return m_reason;};
        ErrorPart& setReason(const QString& arg){m_reason=arg;return *this;};

            /**
                message details
            */
        QString message()const{return m_message;};
        ErrorPart& setMessage(const QString& arg){m_message=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<ErrorPart>  create(const QByteArray& data);
            static std::unique_ptr<ErrorPart>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<ErrorPart> EXAMPLE(int context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                domain - global
            */
        QString m_domain;

            /**
                reason - badRequest, authError etc.
            */
        QString m_reason;

            /**
                message details
            */
        QString m_message;

    };//ErrorPart

}//errors
}//googleQt
