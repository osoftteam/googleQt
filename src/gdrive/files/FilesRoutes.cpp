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
    BODY_ARG_GBC(copy_AsyncCB, FileResource, arg, body);
}

GoogleTask<FileResource>* FilesRoutes::copy_Async(const gdrive::CopyFileArg& arg, const FileResource& body)
{
    GoogleTask<FileResource>* t = new GoogleTask<FileResource>();
    m_end_point->postStyle<
        FileResource,
        FileResource::factory,
        FileResource>
        (m_end_point->buildGdriveUrl("files", arg),
        body,
        t);
    return t;
}

void FilesRoutes::copy_AsyncCB(
    const gdrive::CopyFileArg& arg,
    const FileResource& body,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        FileResource,
        FileResource::factory,
        FileResource
        >
        (m_end_point->buildGdriveUrl("files", arg),
        body,
        completed_callback,
        failed_callback);
}

std::unique_ptr<FileResource> FilesRoutes::create(const gdrive::CreateFileArg& arg, const FileResource& body){
    BODY_ARG_GBC(create_AsyncCB, FileResource, arg, body);
}

GoogleTask<FileResource>* FilesRoutes::create_Async(const gdrive::CreateFileArg& arg, const FileResource& body)
{
    GoogleTask<FileResource>* t = new GoogleTask<FileResource>();
    m_end_point->postStyle<
        FileResource,
        FileResource::factory,
        FileResource>
        (m_end_point->buildGdriveUrl("files", arg),
        body,
        t);
    return t;
}

void FilesRoutes::create_AsyncCB(
    const gdrive::CreateFileArg& arg,
    const FileResource& body,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        FileResource,
        FileResource::factory,
        FileResource
        >
        (m_end_point->buildGdriveUrl("files", arg),
        body,
        completed_callback,
        failed_callback);
}

void FilesRoutes::deleteOperation(const gdrive::DeleteFileArg& arg ){
    VOID_RESULT_GBC(deleteOperation_AsyncCB, arg);
}

GoogleVoidTask* FilesRoutes::deleteOperation_Async(const gdrive::DeleteFileArg& arg)
{
    GoogleVoidTask* t = new GoogleVoidTask();
    m_end_point->deleteStyle
        (m_end_point->buildGdriveUrl("files", arg),
        t);
    return t;
}

void FilesRoutes::deleteOperation_AsyncCB(
    const gdrive::DeleteFileArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->deleteStyle
        (m_end_point->buildGdriveUrl("files", arg),
        completed_callback,
        failed_callback);
}

void FilesRoutes::downloadFile(const gdrive::DownloadFileArg& arg , QIODevice* data){
    VOID_RESULT_ARG_WITH_DATA_GBC(downloadFile_AsyncCB, arg, data);
}

GoogleVoidTask* FilesRoutes::downloadFile_Async(const gdrive::DownloadFileArg& arg, QIODevice* data)
{
    GoogleVoidTask* t = new GoogleVoidTask();
    m_end_point->downloadStyle
        (m_end_point->buildGdriveUrl("files", arg),
        data,
        t);
    return t;
}

void FilesRoutes::downloadFile_AsyncCB(
    const gdrive::DownloadFileArg& arg,
    QIODevice* data,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->downloadStyle
        (m_end_point->buildGdriveUrl("files", arg),
        data,
        completed_callback,
        failed_callback);
}

void FilesRoutes::emptyTrash(const gdrive::EmptyTrashArg& arg ){
    VOID_RESULT_GBC(emptyTrash_AsyncCB, arg);
}

GoogleVoidTask* FilesRoutes::emptyTrash_Async(const gdrive::EmptyTrashArg& arg)
{
    GoogleVoidTask* t = new GoogleVoidTask();
    m_end_point->deleteStyle
        (m_end_point->buildGdriveUrl("files", arg),
        t);
    return t;
}

void FilesRoutes::emptyTrash_AsyncCB(
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
    GOOGLE_BLOCKING_CALL(get_AsyncCB, FileResource, arg);
}

GoogleTask<FileResource>* FilesRoutes::get_Async(const gdrive::GetFileArg& arg)
{
    GoogleTask<FileResource>* t = new GoogleTask<FileResource>();
    m_end_point->getStyle<
        FileResource,
        FileResource::factory
        >
        (m_end_point->buildGdriveUrl("files", arg),
        t);
    return t;
}

void FilesRoutes::get_AsyncCB(
    const gdrive::GetFileArg& arg,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        FileResource,
        FileResource::factory
        >
        (m_end_point->buildGdriveUrl("files", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<FileResourcesCollection> FilesRoutes::list(const gdrive::FileListArg& arg){
    GOOGLE_BLOCKING_CALL(list_AsyncCB, FileResourcesCollection, arg);
}

GoogleTask<FileResourcesCollection>* FilesRoutes::list_Async(const gdrive::FileListArg& arg)
{
    GoogleTask<FileResourcesCollection>* t = new GoogleTask<FileResourcesCollection>();
    m_end_point->getStyle<
        FileResourcesCollection,
        FileResourcesCollection::factory
        >
        (m_end_point->buildGdriveUrl("files", arg),
        t);
    return t;
}

void FilesRoutes::list_AsyncCB(
    const gdrive::FileListArg& arg,
    std::function<void(std::unique_ptr<FileResourcesCollection>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        FileResourcesCollection,
        FileResourcesCollection::factory
        >
        (m_end_point->buildGdriveUrl("files", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<FileResource> FilesRoutes::uploadFile(const FileResource& body, QIODevice* data){
    DATA_GBC(uploadFile_AsyncCB, FileResource, body, data);
}

GoogleTask<FileResource>* FilesRoutes::uploadFile_Async(const FileResource& body, QIODevice* data)
{
    GoogleTask<FileResource>* t = new GoogleTask<FileResource>();
    m_end_point->mpartUploadStyle<
        FileResource,
        FileResource::factory,
        FileResource>
        (m_end_point->buildGdriveMPartUploadUrl("files", VoidType::instance()),
        body,
        data,
        t);
    return t;
}

void FilesRoutes::uploadFile_AsyncCB(
    const FileResource& body,
    QIODevice* data,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->mpartUploadStyle
        <
        FileResource,
        FileResource::factory,
        FileResource
        >
        (m_end_point->buildGdriveMPartUploadUrl("files", VoidType::instance()),
        body,
        data,
        completed_callback,
        failed_callback);
}

