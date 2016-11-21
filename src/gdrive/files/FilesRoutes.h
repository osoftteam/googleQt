/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "GoogleRouteBase.h"
#include "gdrive/files/FilesFileResource.h"
#include "gdrive/files/FilesFileResourcesCollection.h"

namespace googleQt{
namespace files{


    class FilesRoutes: public GoogleRouteBase{
    public:
        FilesRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('copy')


            Creates a copy of a file and applies any requested updates with
            patch semantics.

            */
        std::unique_ptr<FileResource> copy(const gdrive::CopyFileArg& arg, const FileResource& body);

            /**
            ApiRoute('create')


            Creates a new file.

            */
        std::unique_ptr<FileResource> create(const gdrive::CreateFileArg& arg, QIODevice* readFrom);

            /**
            ApiRoute('createFolder')


            Creates a new folder.

            */
        std::unique_ptr<FileResource> createFolder(const gdrive::CreateFileArg& arg, const FileResource& body);

            /**
            ApiRoute('delete')


            Permanently deletes a file owned by the user without moving it to
            the trash. If the target is a folder, all descendants owned by the
            user are also deleted.

            */
        void deleteOperation(const gdrive::DeleteFileArg& );

            /**
            ApiRoute('emptyTrash')


            Permanently deletes a file owned by the user without moving it to
            the trash. If the target is a folder, all descendants owned by the
            user are also deleted.

            */
        void emptyTrash(const gdrive::EmptyTrashArg& );

            /**
            ApiRoute('get')


            Get file metadata.

            */
        std::unique_ptr<FileResource> get(const gdrive::GetFileArg& arg);

            /**
            ApiRoute('list')


            Lists or searches files.

            */
        std::unique_ptr<FileResourcesCollection> list(const gdrive::FileListArg& arg);

    protected:
    };//FilesRoutes

}//files
}//googleQt
