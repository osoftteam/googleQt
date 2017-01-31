/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "gdrive/files/FilesContentHints.h"

namespace googleQt{
namespace files{
    class UpdateFileDetails{
        /**
            field: name: The name of the file. This is not necessarily unique
                within a folder.
            field: mime_type: The MIME type of the file. Drive will attempt to
                automatically detect an appropriate value from uploaded content
                if no value is provided. The value cannot be changed unless a
                new revision is uploaded.
            field: description: A short description of the file.
            field: content_hints: Additional information about the content of
                the file.
            field: original_filename: The original filename of the uploaded
                content if available, or else the original value of the name
                field. This is only available for files with binary content in
                Drive.
        */

    public:
        UpdateFileDetails(){};

        UpdateFileDetails(const QString& arg){ m_name = arg; };

    public:
            /**
                The name of the file. This is not necessarily unique within a
                folder.
            */
        QString name()const{return m_name;};
        UpdateFileDetails& setName(const QString& arg){m_name=arg;return *this;};

            /**
                The MIME type of the file. Drive will attempt to automatically
                detect an appropriate value from uploaded content if no value is
                provided. The value cannot be changed unless a new revision is
                uploaded.
            */
        QString mimetype()const{return m_mimeType;};
        UpdateFileDetails& setMimetype(const QString& arg){m_mimeType=arg;return *this;};

            /**
                A short description of the file.
            */
        QString description()const{return m_description;};
        UpdateFileDetails& setDescription(const QString& arg){m_description=arg;return *this;};

            /**
                Additional information about the content of the file.
            */
        const ContentHints& contenthints()const{return m_contentHints;};
        UpdateFileDetails& setContenthints(const ContentHints& arg){m_contentHints=arg;return *this;};

            /**
                The original filename of the uploaded content if available, or
                else the original value of the name field. This is only
                available for files with binary content in Drive.
            */
        QString originalfilename()const{return m_originalFilename;};
        UpdateFileDetails& setOriginalfilename(const QString& arg){m_originalFilename=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<UpdateFileDetails>  create(const QByteArray& data);
            static std::unique_ptr<UpdateFileDetails>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<UpdateFileDetails> EXAMPLE(int context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The name of the file. This is not necessarily unique within a
                folder.
            */
        QString m_name;

            /**
                The MIME type of the file. Drive will attempt to automatically
                detect an appropriate value from uploaded content if no value is
                provided. The value cannot be changed unless a new revision is
                uploaded.
            */
        QString m_mimeType;

            /**
                A short description of the file.
            */
        QString m_description;

            /**
                Additional information about the content of the file.
            */
        ContentHints m_contentHints;

            /**
                The original filename of the uploaded content if available, or
                else the original value of the name field. This is only
                available for files with binary content in Drive.
            */
        QString m_originalFilename;

    };//UpdateFileDetails

}//files
}//googleQt
