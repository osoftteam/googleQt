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
    BODY_ARG_GBC(copy_Async, FileResource, arg, body);
}

void FilesRoutes::copy_Async(
    const gdrive::CopyFileArg& arg,
    const FileResource& body,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        std::unique_ptr<FileResource>,
        FileResource::factory,
        FileResource
        >
        (m_end_point->buildGdriveUrl("files", arg),
        body,
        completed_callback,
        failed_callback);
}

std::unique_ptr<FileResource> FilesRoutes::create(const gdrive::CreateFileArg& arg, QIODevice* data){
    DATA_GBC(create_Async, FileResource, arg, data);
}

void FilesRoutes::create_Async(
    const gdrive::CreateFileArg& arg,
    QIODevice* data,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->uploadStyle
        <
        std::unique_ptr<FileResource>,
        FileResource::factory
        >
        (m_end_point->buildGdriveUploadUrl("files", arg),
        data,
        completed_callback,
        failed_callback);
}

std::unique_ptr<FileResource> FilesRoutes::createFolder(const gdrive::CreateFileArg& arg, const FileResource& body){
    BODY_ARG_GBC(createFolder_Async, FileResource, arg, body);
}

void FilesRoutes::createFolder_Async(
    const gdrive::CreateFileArg& arg,
    const FileResource& body,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        std::unique_ptr<FileResource>,
        FileResource::factory,
        FileResource
        >
        (m_end_point->buildGdriveUrl("files", arg),
        body,
        completed_callback,
        failed_callback);
}

void FilesRoutes::deleteOperation(const gdrive::DeleteFileArg& arg ){
    VOID_RESULT_GBC(deleteOperation_Async, arg);
}

void FilesRoutes::deleteOperation_Async(
    const gdrive::DeleteFileArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->deleteStyle
        (m_end_point->buildGdriveUrl("files", arg),
        completed_callback,
        failed_callback);
}

void FilesRoutes::emptyTrash(const gdrive::EmptyTrashArg& arg ){
    VOID_RESULT_GBC(emptyTrash_Async, arg);
}

void FilesRoutes::emptyTrash_Async(
    const gdrive::EmptyTrashArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->deleteStyle
        (m_end_point->buildGdriveUrl("files", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<FileResource> FilesRoutes::get(const gdrive::GetFileArg& arg){
    GOOGLE_BLOCKING_CALL(get_Async, FileResource, arg);
}

void FilesRoutes::get_Async(
    const gdrive::GetFileArg& arg,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<FileResource>,
        FileResource::factory
        >
        (m_end_point->buildGdriveUrl("files", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<FileResourcesCollection> FilesRoutes::list(const gdrive::FileListArg& arg){
    GOOGLE_BLOCKING_CALL(list_Async, FileResourcesCollection, arg);
}

void FilesRoutes::list_Async(
    const gdrive::FileListArg& arg,
    std::function<void(std::unique_ptr<FileResourcesCollection>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<FileResourcesCollection>,
        FileResourcesCollection::factory
        >
        (m_end_point->buildGdriveUrl("files", arg),
        completed_callback,
        failed_callback);
}

