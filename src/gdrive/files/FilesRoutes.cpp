/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/files/FilesRoutes.h"
#include "Endpoint.h"
#include "gdrive/GdriveRoutes.h"
using namespace googleQt;
using namespace files;

std::unique_ptr<FileResource> FilesRoutes::copy(const gdrive::CopyFileArg& arg, const FileResource& body){
    return m_end_point->postStyle<std::unique_ptr<FileResource>, FileResource::factory, FileResource>(m_end_point->buildGdriveUrl("files", arg), body);
}

std::unique_ptr<FileResource> FilesRoutes::create(const gdrive::CreateFileArg& arg, QIODevice* readFrom){
    return m_end_point->uploadStyle<std::unique_ptr<FileResource>, FileResource::factory, NotAnException>(m_end_point->buildGdriveUploadUrl("files", arg), readFrom);
}

std::unique_ptr<FileResource> FilesRoutes::createFolder(const gdrive::CreateFileArg& arg, const FileResource& body){
    return m_end_point->postStyle<std::unique_ptr<FileResource>, FileResource::factory, FileResource>(m_end_point->buildGdriveUrl("files", arg), body);
}

void FilesRoutes::deleteOperation(const gdrive::DeleteFileArg& arg ){
    m_end_point->deleteStyle<std::unique_ptr<VoidType>, VoidType, NotAnException>(m_end_point->buildGdriveUrl("files", arg) );
}

void FilesRoutes::emptyTrash(const gdrive::EmptyTrashArg& arg ){
    m_end_point->deleteStyle<std::unique_ptr<VoidType>, VoidType, NotAnException>(m_end_point->buildGdriveUrl("files", arg) );
}

std::unique_ptr<FileResource> FilesRoutes::get(const gdrive::GetFileArg& arg){
    return m_end_point->getStyle<std::unique_ptr<FileResource>, FileResource::factory, NotAnException>(m_end_point->buildGdriveUrl("files", arg));
}

std::unique_ptr<FileResourcesCollection> FilesRoutes::list(const gdrive::FileListArg& arg){
    return m_end_point->getStyle<std::unique_ptr<FileResourcesCollection>, FileResourcesCollection::factory, NotAnException>(m_end_point->buildGdriveUrl("files", arg));
}

