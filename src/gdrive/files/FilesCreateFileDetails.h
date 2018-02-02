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
    class CreateFileDetails{
        /**
            field: id: The ID of the file.
            field: mime_type: The MIME type of the file. Drive will attempt to
                automatically detect an appropriate value from uploaded content
                if no value is provided. The value cannot be changed unless a
                new revision is uploaded.
            field: description: A short description of the file.
            field: content_hints: Additional information about the content of
                the file.
            field: name: The name of the file. This is not necessarily unique
                within a folder.
            field: original_filename: The original filename of the uploaded
                content if available, or else the original value of the name
                field. This is only available for files with binary content in
                Drive.
            field: parents: The IDs of the parent folders which contain the
                file. If not specified as part of a create request, the file
                will be placed directly in the My Drive folder. Update requests
                must use the addParents and removeParents parameters to modify
                the values.
            field: starred: Whether the user has starred the file.
        */

    public:
        CreateFileDetails():
        m_starred(false)
        {};

        CreateFileDetails(const QString& arg):
        m_starred(false)
        { m_id = arg; };
        virtual ~CreateFileDetails(){};

    public:
            /**
                The ID of the file.
            */
        QString id()const{return m_id;};
        CreateFileDetails& setId(const QString& arg){m_id=arg;return *this;};

            /**
                The MIME type of the file. Drive will attempt to automatically
                detect an appropriate value from uploaded content if no value is
                provided. The value cannot be changed unless a new revision is
                uploaded.
            */
        QString mimetype()const{return m_mimeType;};
        CreateFileDetails& setMimetype(const QString& arg){m_mimeType=arg;return *this;};

            /**
                A short description of the file.
            */
        QString description()const{return m_description;};
        CreateFileDetails& setDescription(const QString& arg){m_description=arg;return *this;};

            /**
                Additional information about the content of the file.
            */
        const ContentHints& contenthints()const{return m_contentHints;};
        CreateFileDetails& setContenthints(const ContentHints& arg){m_contentHints=arg;return *this;};

            /**
                The name of the file. This is not necessarily unique within a
                folder.
            */
        QString name()const{return m_name;};
        CreateFileDetails& setName(const QString& arg){m_name=arg;return *this;};

            /**
                The original filename of the uploaded content if available, or
                else the original value of the name field. This is only
                available for files with binary content in Drive.
            */
        QString originalfilename()const{return m_originalFilename;};
        CreateFileDetails& setOriginalfilename(const QString& arg){m_originalFilename=arg;return *this;};

            /**
                The IDs of the parent folders which contain the file. If not
                specified as part of a create request, the file will be placed
                directly in the My Drive folder. Update requests must use the
                addParents and removeParents parameters to modify the values.
            */
        const std::list <QString>& parents()const{return m_parents;};
        CreateFileDetails& setParents(const std::list <QString>& arg){m_parents=arg;return *this;};

            /**
                Whether the user has starred the file.
            */
        bool starred()const{return m_starred;};
        CreateFileDetails& setStarred(const bool& arg){m_starred=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<CreateFileDetails>  create(const QByteArray& data);
            static std::unique_ptr<CreateFileDetails>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<CreateFileDetails> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The ID of the file.
            */
        QString m_id;

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
                The name of the file. This is not necessarily unique within a
                folder.
            */
        QString m_name;

            /**
                The original filename of the uploaded content if available, or
                else the original value of the name field. This is only
                available for files with binary content in Drive.
            */
        QString m_originalFilename;

            /**
                The IDs of the parent folders which contain the file. If not
                specified as part of a create request, the file will be placed
                directly in the My Drive folder. Update requests must use the
                addParents and removeParents parameters to modify the values.
            */
        std::list <QString> m_parents;

            /**
                Whether the user has starred the file.
            */
        bool m_starred;

    };//CreateFileDetails

}//files
}//googleQt
