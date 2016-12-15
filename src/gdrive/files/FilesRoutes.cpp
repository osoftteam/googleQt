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
    return copy_Async(arg, body)->waitForResultAndRelease();
}

GoogleTask<FileResource>* FilesRoutes::copy_Async(const gdrive::CopyFileArg& arg, const FileResource& body)
{
    GoogleTask<FileResource>* t = m_end_point->produceTask<FileResource>();
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
    return create_Async(arg, body)->waitForResultAndRelease();
}

GoogleTask<FileResource>* FilesRoutes::create_Async(const gdrive::CreateFileArg& arg, const FileResource& body)
{
    GoogleTask<FileResource>* t = m_end_point->produceTask<FileResource>();
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
    deleteOperation_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* FilesRoutes::deleteOperation_Async(const gdrive::DeleteFileArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
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
    downloadFile_Async(arg, data)->waitForResultAndRelease();
}

GoogleVoidTask* FilesRoutes::downloadFile_Async(const gdrive::DownloadFileArg& arg, QIODevice* data)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
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
    emptyTrash_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* FilesRoutes::emptyTrash_Async(const gdrive::EmptyTrashArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
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
    return get_Async(arg)->waitForResultAndRelease();
}

GoogleTask<FileResource>* FilesRoutes::get_Async(const gdrive::GetFileArg& arg)
{
    GoogleTask<FileResource>* t = m_end_point->produceTask<FileResource>();
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
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<FileResourcesCollection>* FilesRoutes::list_Async(const gdrive::FileListArg& arg)
{
    GoogleTask<FileResourcesCollection>* t = m_end_point->produceTask<FileResourcesCollection>();
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
    return uploadFile_Async(body, data)->waitForResultAndRelease();
}

GoogleTask<FileResource>* FilesRoutes::uploadFile_Async(const FileResource& body, QIODevice* data)
{
    GoogleTask<FileResource>* t = m_end_point->produceTask<FileResource>();
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

std::unique_ptr<FileResource> FilesRoutes::uploadFileSimple(QIODevice* data){
    return uploadFileSimple_Async(data)->waitForResultAndRelease();
}

GoogleTask<FileResource>* FilesRoutes::uploadFileSimple_Async(QIODevice* data)
{
    GoogleTask<FileResource>* t = m_end_point->produceTask<FileResource>();
    m_end_point->simpleUploadStyle<
        FileResource,
        FileResource::factory
        >
        (m_end_point->buildGdriveMPartUploadUrl("files", VoidType::instance()),
        data,
        t);
    return t;
}

void FilesRoutes::uploadFileSimple_AsyncCB(
    QIODevice* data,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->simpleUploadStyle
        <
        FileResource,
        FileResource::factory
        >
        (m_end_point->buildGdriveMPartUploadUrl("files", VoidType::instance()),
        data,
        completed_callback,
        failed_callback);
}

