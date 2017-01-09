/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "gdrive/files/FilesFileResource.h"

namespace googleQt{
namespace files{
    class FileResourcesCollection{
        /**
            field: next_page_token: The page token for the next page of files.
                This will be absent if the end of the files list has been
                reached.
            field: kind: This is always drive#fileList.
            field: files: The page of files.
        */

    public:
        FileResourcesCollection():
        m_kind("drive#fileList")
        {};

        FileResourcesCollection(const QString& arg):
        m_kind("drive#fileList")
        { m_nextPageToken = arg; };

    public:
            /**
                The page token for the next page of files. This will be absent
                if the end of the files list has been reached.
            */
        QString nextpagetoken()const{return m_nextPageToken;};
        FileResourcesCollection& setNextpagetoken(const QString& arg){m_nextPageToken=arg;return *this;};

            /**
                This is always drive#fileList.
            */
        QString kind()const{return m_kind;};
        FileResourcesCollection& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                The page of files.
            */
        const std::list <FileResource>& files()const{return m_files;};
        FileResourcesCollection& setFiles(const std::list <FileResource>& arg){m_files=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<FileResourcesCollection>  create(const QByteArray& data);
            static std::unique_ptr<FileResourcesCollection>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<FileResourcesCollection> EXAMPLE(int context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The page token for the next page of files. This will be absent
                if the end of the files list has been reached.
            */
        QString m_nextPageToken;

            /**
                This is always drive#fileList.
            */
        QString m_kind;

            /**
                The page of files.
            */
        std::list <FileResource> m_files;

    };//FileResourcesCollection

}//files
}//googleQt
