/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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

std::unique_ptr<FileResource> FilesRoutes::create(const gdrive::CreateFileArg& arg, QIODevice* data){
    return create_Async(arg, data)->waitForResultAndRelease();
}

GoogleTask<FileResource>* FilesRoutes::create_Async(const gdrive::CreateFileArg& arg, QIODevice* data)
{
    GoogleTask<FileResource>* t = m_end_point->produceTask<FileResource>();
    m_end_point->mpartUploadStyle<
        FileResource,
        FileResource::factory
        ,gdrive::CreateFileArg>
        (m_end_point->buildGdriveMPartUploadUrl("files", arg),
        arg,
        data,
        t);
    return t;
}

void FilesRoutes::create_AsyncCB(
    const gdrive::CreateFileArg& arg,
    QIODevice* data,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->mpartUploadStyle
        <
        FileResource,
        FileResource::factory
        , gdrive::CreateFileArg
        >
        (m_end_point->buildGdriveMPartUploadUrl("files", arg),
        arg,
        data,
        completed_callback,
        failed_callback);
}

std::unique_ptr<FileResource> FilesRoutes::createEmptyFile(const gdrive::CreateFileArg& arg){
    return createEmptyFile_Async(arg)->waitForResultAndRelease();
}

GoogleTask<FileResource>* FilesRoutes::createEmptyFile_Async(const gdrive::CreateFileArg& arg)
{
    GoogleTask<FileResource>* t = m_end_point->produceTask<FileResource>();
    m_end_point->postStyleB<
        FileResource,
        FileResource::factory
        ,gdrive::CreateFileArg>
        (m_end_point->buildGdriveUrl("files", arg),
        arg,
        t);
    return t;
}

void FilesRoutes::createEmptyFile_AsyncCB(
    const gdrive::CreateFileArg& arg,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyleB
        <
        FileResource,
        FileResource::factory
        , gdrive::CreateFileArg
        >
        (m_end_point->buildGdriveUrl("files", arg),
        arg,
        completed_callback,
        failed_callback);
}

std::unique_ptr<FileResource> FilesRoutes::createFolder(const gdrive::CreateFolderArg& arg){
    return createFolder_Async(arg)->waitForResultAndRelease();
}

GoogleTask<FileResource>* FilesRoutes::createFolder_Async(const gdrive::CreateFolderArg& arg)
{
    GoogleTask<FileResource>* t = m_end_point->produceTask<FileResource>();
    m_end_point->postStyleB<
        FileResource,
        FileResource::factory
        ,gdrive::CreateFolderArg>
        (m_end_point->buildGdriveUrl("files", arg),
        arg,
        t);
    return t;
}

void FilesRoutes::createFolder_AsyncCB(
    const gdrive::CreateFolderArg& arg,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyleB
        <
        FileResource,
        FileResource::factory
        , gdrive::CreateFolderArg
        >
        (m_end_point->buildGdriveUrl("files", arg),
        arg,
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

std::unique_ptr<GeneratedIdsCollection> FilesRoutes::generateIds(const gdrive::GenerateIdArg& arg){
    return generateIds_Async(arg)->waitForResultAndRelease();
}

GoogleTask<GeneratedIdsCollection>* FilesRoutes::generateIds_Async(const gdrive::GenerateIdArg& arg)
{
    GoogleTask<GeneratedIdsCollection>* t = m_end_point->produceTask<GeneratedIdsCollection>();
    m_end_point->getStyle<
        GeneratedIdsCollection,
        GeneratedIdsCollection::factory
        >
        (m_end_point->buildGdriveUrl("files", arg),
        t);
    return t;
}

void FilesRoutes::generateIds_AsyncCB(
    const gdrive::GenerateIdArg& arg,
    std::function<void(std::unique_ptr<GeneratedIdsCollection>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        GeneratedIdsCollection,
        GeneratedIdsCollection::factory
        >
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

std::unique_ptr<FileResource> FilesRoutes::moveFile(const gdrive::MoveFileArg& arg){
    return moveFile_Async(arg)->waitForResultAndRelease();
}

GoogleTask<FileResource>* FilesRoutes::moveFile_Async(const gdrive::MoveFileArg& arg)
{
    GoogleTask<FileResource>* t = m_end_point->produceTask<FileResource>();
    m_end_point->updateStyle<
        FileResource,
        FileResource::factory
        ,gdrive::MoveFileArg>
        (m_end_point->buildGdriveUrl("files", arg),
        arg,
        t);
    return t;
}

void FilesRoutes::moveFile_AsyncCB(
    const gdrive::MoveFileArg& arg,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->updateStyle
        <
        FileResource,
        FileResource::factory
        , gdrive::MoveFileArg
        >
        (m_end_point->buildGdriveUrl("files", arg),
        arg,
        completed_callback,
        failed_callback);
}

std::unique_ptr<FileResource> FilesRoutes::rename(const gdrive::RenameFileArg& arg){
    return rename_Async(arg)->waitForResultAndRelease();
}

GoogleTask<FileResource>* FilesRoutes::rename_Async(const gdrive::RenameFileArg& arg)
{
    GoogleTask<FileResource>* t = m_end_point->produceTask<FileResource>();
    m_end_point->updateStyle<
        FileResource,
        FileResource::factory
        ,gdrive::RenameFileArg>
        (m_end_point->buildGdriveUrl("files", arg),
        arg,
        t);
    return t;
}

void FilesRoutes::rename_AsyncCB(
    const gdrive::RenameFileArg& arg,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->updateStyle
        <
        FileResource,
        FileResource::factory
        , gdrive::RenameFileArg
        >
        (m_end_point->buildGdriveUrl("files", arg),
        arg,
        completed_callback,
        failed_callback);
}

std::unique_ptr<FileResource> FilesRoutes::updateFileMeta(const gdrive::UpdateFileArg& arg){
    return updateFileMeta_Async(arg)->waitForResultAndRelease();
}

GoogleTask<FileResource>* FilesRoutes::updateFileMeta_Async(const gdrive::UpdateFileArg& arg)
{
    GoogleTask<FileResource>* t = m_end_point->produceTask<FileResource>();
    m_end_point->updateStyle<
        FileResource,
        FileResource::factory
        ,gdrive::UpdateFileArg>
        (m_end_point->buildGdriveUrl("files", arg),
        arg,
        t);
    return t;
}

void FilesRoutes::updateFileMeta_AsyncCB(
    const gdrive::UpdateFileArg& arg,
    std::function<void(std::unique_ptr<FileResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->updateStyle
        <
        FileResource,
        FileResource::factory
        , gdrive::UpdateFileArg
        >
        (m_end_point->buildGdriveUrl("files", arg),
        arg,
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
        (m_end_point->buildGdriveSimpleMediaUploadUrl("files", VoidType::instance()),
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
        (m_end_point->buildGdriveSimpleMediaUploadUrl("files", VoidType::instance()),
        data,
        completed_callback,
        failed_callback);
}

