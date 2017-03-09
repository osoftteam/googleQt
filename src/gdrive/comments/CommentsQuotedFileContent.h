/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"

namespace googleQt{
namespace comments{
    class QuotedFileContent{
        /**
            field: mime_type: The MIME type of the quoted content.
            field: value: The quoted content itself. This is interpreted as
                plain text if set through the API.
        */

    public:
        QuotedFileContent(){};

        QuotedFileContent(const QString& arg){ m_mimeType = arg; };

    public:
            /**
                The MIME type of the quoted content.
            */
        QString mimetype()const{return m_mimeType;};
        QuotedFileContent& setMimetype(const QString& arg){m_mimeType=arg;return *this;};

            /**
                The quoted content itself. This is interpreted as plain text if
                set through the API.
            */
        QString value()const{return m_value;};
        QuotedFileContent& setValue(const QString& arg){m_value=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<QuotedFileContent>  create(const QByteArray& data);
            static std::unique_ptr<QuotedFileContent>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<QuotedFileContent> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The MIME type of the quoted content.
            */
        QString m_mimeType;

            /**
                The quoted content itself. This is interpreted as plain text if
                set through the API.
            */
        QString m_value;

    };//QuotedFileContent

}//comments
}//googleQt
