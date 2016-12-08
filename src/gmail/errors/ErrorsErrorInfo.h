/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "errors"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/errors/ErrorsErrorPart.h"

namespace googleQt{
namespace errors{
    class ErrorInfo{
        /**
            field: errors: groups of error details
            field: code: HTTP response code
            field: message: Details of the error.
        */

    public:
        ErrorInfo(){};

        ErrorInfo(const std::list <ErrorPart>& arg){ m_errors = arg; };

    public:
            /**
                groups of error details
            */
        const std::list <ErrorPart>& errors()const{return m_errors;};
        ErrorInfo& setErrors(const std::list <ErrorPart>& arg){m_errors=arg;return *this;};

            /**
                HTTP response code
            */
        int code()const{return m_code;};
        ErrorInfo& setCode(const int& arg){m_code=arg;return *this;};

            /**
                Details of the error.
            */
        QString message()const{return m_message;};
        ErrorInfo& setMessage(const QString& arg){m_message=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<ErrorInfo>  create(const QByteArray& data);
            static std::unique_ptr<ErrorInfo>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<ErrorInfo> EXAMPLE();
        #endif //API_QT_AUTOTEST


    protected:
            /**
                groups of error details
            */
        std::list <ErrorPart> m_errors;

            /**
                HTTP response code
            */
        int m_code = {0};

            /**
                Details of the error.
            */
        QString m_message;

    };//ErrorInfo

}//errors
}//googleQt
