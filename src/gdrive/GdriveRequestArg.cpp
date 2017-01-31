#include <QUrlQuery>
#include "GdriveRequestArg.h"
#include "gdrive/files/FilesCreateFileDetails.h"
#include "gdrive/files/FilesUpdateFileDetails.h"

using namespace googleQt;
using namespace gdrive;

/**
   AboutArg
*/
AboutArg::AboutArg()
{

};


void AboutArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/%1").arg(path_about::path()), url);
    if(!m_Fields.isEmpty()){
        b.add("fields", m_Fields);
    }
};


/**
    FileListArg
*/
FileListArg::FileListArg(QString pageToken):
    m_corpus("user"),
    m_pageSize(100),
    m_pageToken(pageToken)
{

};

void FileListArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + "/files", url);
    b.add("corpus", m_corpus)
        .add("pageSize", m_pageSize)
        .add("orderBy", m_orderBy)
        .add("pageToken", m_pageToken)
        .add("q", m_q)
        .add("spaces", m_spaces);
}

/**
    FileIdArg
*/
GetFileArg::GetFileArg(QString fileId)
    :m_fileId(fileId), m_acknowledgeAbuse(false)
{

};

void GetFileArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/files/%2").arg(m_fileId), url);
    b.add("acknowledgeAbuse", m_acknowledgeAbuse);
    ResponseFields2Builder(b);
}

/**
    MultipartUploadFileArg
*/
MultipartUploadFileArg::MultipartUploadFileArg(QString name /*= ""*/) 
    :m_name(name)
{

};

void MultipartUploadFileArg::build(const QString&, QUrl& )const 
{
    ///we don't really build anything, the mpart link should be good enough
};

void MultipartUploadFileArg::toJson(QJsonObject& js)const
{
    if (!m_name.isEmpty())
        js["name"] = QString(m_name);
};


/**
    DownloadFileArg
*/
DownloadFileArg::DownloadFileArg(QString fileId)
    :m_fileId(fileId)
{

};

void DownloadFileArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/files/%2").arg(m_fileId), url);
    b.add("alt", "media");
    ResponseFields2Builder(b);
}


/**
    CopyFileArg
*/
CopyFileArg::CopyFileArg()
    :m_ignoreDefaultVisibility(false),
    m_keepRevisionForever(false)
{

};

void CopyFileArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + "/files", url);
    b.add("fileId", m_fileId)
        .add("ignoreDefaultVisibility", m_ignoreDefaultVisibility)
        .add("keepRevisionForever", m_keepRevisionForever)
        .add("ocrLanguage", m_ocrLanguage);
}

DeleteFileArg::DeleteFileArg(QString fileId)
    :m_fileId(fileId)
{

}

void DeleteFileArg::build(const QString& link_path, QUrl& url)const
{
    url = link_path + QString("/files/%1").arg(m_fileId);
}

/**
    CreateFileArg
*/
CreateFileArg::CreateFileArg(QString name /*= ""*/)
{
    m_ignoreDefaultVisibility = false;
    m_keepRevisionForever = false;
    m_useContentAsIndexableText = false;
    m_create_file.reset(new files::CreateFileDetails());
    if (!name.isEmpty())
    {
        m_create_file->setName(name);
    }
};

CreateFileArg::~CreateFileArg() 
{

};

void CreateFileArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + "/files", url);
    b.add("ignoreDefaultVisibility", m_ignoreDefaultVisibility)
        .add("keepRevisionForever", m_keepRevisionForever)
        .add("ocrLanguage", m_ocrLanguage)
        .add("useContentAsIndexableText", m_useContentAsIndexableText);
    ResponseFields2Builder(b);
}

void CreateFileArg::toJson(QJsonObject& js)const
{
    m_create_file->toJson(js);
};

files::CreateFileDetails& CreateFileArg::fileDetailes()const 
{
    return *(m_create_file.get());
};

/**
   UpdateFileArg
*/
UpdateFileArg::UpdateFileArg(QString name /*= ""*/)
{
    m_update_file.reset(new files::UpdateFileDetails());
    if (!name.isEmpty())
    {
        m_update_file->setName(name);
    }
};

UpdateFileArg::~UpdateFileArg() 
{

};

void UpdateFileArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + "/files", url);
    b.add("ocrLanguage", m_ocrLanguage);
    QString parents2remove = slist2commalist(m_removeParents);
    if(!parents2remove.isEmpty())
        b.add("removeParents", parents2remove);
    ResponseFields2Builder(b);
}

void UpdateFileArg::toJson(QJsonObject& js)const
{
    m_update_file->toJson(js);
};

files::UpdateFileDetails& UpdateFileArg::fileDetailes()const 
{
    return *(m_update_file.get());
};


/**
   CreateFolderArg
*/
CreateFolderArg::CreateFolderArg(QString name):m_name(name)
{

};

void CreateFolderArg::build(const QString& link_path, QUrl& url)const
{
    url = link_path + QString("/files");
}

void CreateFolderArg::toJson(QJsonObject& js)const
{
    if(!m_name.isEmpty())
        js["name"] = QString(m_name);
    js["mimeType"] = QString("application/vnd.google-apps.folder");
    if (!m_description.isEmpty())
        js["description"] = QString(m_description);
    js["parents"] = ingrl_list2jsonarray(m_parents);
};


/**
    PermissionArg
*/
PermissionArg::PermissionArg()
{
};

PermissionArg::PermissionArg(QString fileId, QString permissionId)
    :m_fileId(fileId), m_permissionId(permissionId)
{

};

void PermissionArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/files/%1/permissions/%2").arg(m_fileId).arg(m_permissionId), url);
};

/**
    CreatePermissionArg
*/
CreatePermissionArg::CreatePermissionArg() 
{
    m_sendNotificationEmail = false;
    m_transferOwnership = false;
}

void CreatePermissionArg::build(const QString& link_path, QUrl& url)const 
{
    UrlBuilder b(link_path + QString("/files/%1/permissions").arg(m_fileId), url);
    b.add("emailMessage", m_emailMessage)
        .add("sendNotificationEmail", m_sendNotificationEmail)
        .add("transferOwnership", m_transferOwnership);
};

/**
    PermissionListArg
*/
PermissionListArg::PermissionListArg(QString fileId)
    :m_fileId(fileId)
{

};

void PermissionListArg::build(const QString& link_path, QUrl& url)const 
{
    UrlBuilder b(link_path + QString("/files/%1/permissions").arg(m_fileId), url);
};

/**
    CreateCommentArg
*/
CreateCommentArg::CreateCommentArg() 
{

};

CreateCommentArg::CreateCommentArg(QString fileId)
    :m_fileId(fileId)
{

};

void CreateCommentArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/files/%1/comments").arg(m_fileId), url);
    ResponseFields2Builder(b);
};

/**
    DeleteCommentArg
*/
DeleteCommentArg::DeleteCommentArg() 
{
    
};

DeleteCommentArg::DeleteCommentArg(QString fileId, QString commentId)
    :m_fileId(fileId), m_commentId(commentId)
{

};

void DeleteCommentArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/files/%1/comments/%2").arg(m_fileId).arg(m_commentId), url);
};


/**
    GetCommentArg
*/
GetCommentArg::GetCommentArg()
{

};

GetCommentArg::GetCommentArg(QString fileId, QString commentId)
    :m_fileId(fileId), m_commentId(commentId)
{
    m_includeDeleted = false;
};

void GetCommentArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/files/%1/comments/%2").arg(m_fileId).arg(m_commentId), url);
    b.add("includeDeleted", m_includeDeleted);
    ResponseFields2Builder(b);
};

/**
    RenameFileArg
*/
RenameFileArg::RenameFileArg(QString fileId)
    :m_fileId(fileId)
{

};

void RenameFileArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/files/%2").arg(m_fileId), url);
    ResponseFields2Builder(b);
}

void RenameFileArg::toJson(QJsonObject& js)const
{
    if(!m_name.isEmpty())
        js["name"] = QString(m_name);
};

/**
   MoveFileArg
*/
MoveFileArg::MoveFileArg(QString fileId)
    :m_fileId(fileId)
{

};

void MoveFileArg::build(const QString& link_path, QUrl& url)const
{
    QString parents2add = slist2commalist(m_addParents);
    QString parents2remove = slist2commalist(m_removeParents);
    
    UrlBuilder b(link_path + QString("/files/%2").arg(m_fileId), url);
    if(!parents2add.isEmpty())
        b.add("addParents", parents2add);
    if(!parents2remove.isEmpty())
        b.add("removeParents", parents2remove);
    ResponseFields2Builder(b);
}

void MoveFileArg::toJson(QJsonObject& js)const
{
    if(!m_name.isEmpty())
        js["name"] = QString(m_name);
};

void MoveFileArg::setAddParents(const std::list<QString>& newParents)
{
    m_addParents = newParents;
};

void MoveFileArg::setRemoveParents(const std::list<QString>& Parents2Remove)
{
    m_removeParents = Parents2Remove;
};

/**
    CommentListArg
*/
CommentListArg::CommentListArg(QString fileId):m_fileId(fileId)
{
    m_includeDeleted = false;
    m_pageSize = 20;
};

void CommentListArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/files/%1/comments").arg(m_fileId), url);
    b.add("includeDeleted", m_includeDeleted)
        .add("pageSize", m_pageSize)
        .add("pageToken", m_pageToken)
        .add("startModifiedTime", m_startModifiedTime);
    ResponseFields2Builder(b);
};


#ifdef API_QT_AUTOTEST
std::unique_ptr<FileListArg> FileListArg::EXAMPLE(int)
{
    std::unique_ptr<FileListArg> rv(new FileListArg);
    rv->setOrderBy("modifiedTime");
    rv->setPageToken("next-page");
    rv->setQ("file=my");
    rv->setSpaces("drive,appDataFolder,photos");
    return rv;
};

std::unique_ptr<GetFileArg> GetFileArg::EXAMPLE(int)
{
    std::unique_ptr<GetFileArg> rv(new GetFileArg);
    rv->setFileId("file1");
    return rv;
};

std::unique_ptr<DownloadFileArg> DownloadFileArg::EXAMPLE(int)
{
    std::unique_ptr<DownloadFileArg> rv(new DownloadFileArg);
    rv->setFileId("file1");
    return rv;
};


std::unique_ptr<CopyFileArg> CopyFileArg::EXAMPLE(int)
{
    std::unique_ptr<CopyFileArg> rv(new CopyFileArg);
    rv->setFileId("file1");
    return rv;
};

std::unique_ptr<DeleteFileArg> DeleteFileArg::EXAMPLE(int)
{
    std::unique_ptr<DeleteFileArg> rv(new DeleteFileArg);
    rv->setFileId("file1");
    return rv;
};

std::unique_ptr<CreateFileArg> CreateFileArg::EXAMPLE(int)
{
    std::unique_ptr<CreateFileArg> rv(new CreateFileArg);  
    rv->m_create_file.reset(files::CreateFileDetails::EXAMPLE(1).release());
    return rv;
};

std::unique_ptr<PermissionArg> PermissionArg::EXAMPLE(int)
{
    std::unique_ptr<PermissionArg> rv(new PermissionArg);
    rv->setFileId("file1");
    rv->setPermissionId("permission1");
    return rv;
};

std::unique_ptr<CreatePermissionArg> CreatePermissionArg::EXAMPLE(int)
{
    std::unique_ptr<CreatePermissionArg> rv(new CreatePermissionArg);
    rv->setFileId("file1");
    return rv;
};

std::unique_ptr<PermissionListArg> PermissionListArg::EXAMPLE(int)
{
    std::unique_ptr<PermissionListArg> rv(new PermissionListArg);
    rv->setFileId("file1");
    return rv;
};

std::unique_ptr<CreateCommentArg> CreateCommentArg::EXAMPLE(int)
{
    std::unique_ptr<CreateCommentArg> rv(new CreateCommentArg);
    rv->setFileId("file1");
    return rv;
};

std::unique_ptr<DeleteCommentArg> DeleteCommentArg::EXAMPLE(int)
{
    std::unique_ptr<DeleteCommentArg> rv(new DeleteCommentArg);
    rv->setFileId("file1");
    rv->setCommentId("comment1");
    return rv;
};

std::unique_ptr<GetCommentArg> GetCommentArg::EXAMPLE(int)
{
    std::unique_ptr<GetCommentArg> rv(new GetCommentArg);
    rv->setFileId("file1");
    rv->setCommentId("comment1");
    return rv;
};

std::unique_ptr<CommentListArg> CommentListArg::EXAMPLE(int)
{
    std::unique_ptr<CommentListArg> rv(new CommentListArg);
    rv->setFileId("file1");
    rv->setStartModifiedTime(QDateTime::currentDateTime());
    rv->setPageToken("page-token");
    return rv;
};

std::unique_ptr<RenameFileArg> RenameFileArg::EXAMPLE(int)
{
    std::unique_ptr<RenameFileArg> rv(new RenameFileArg);
    rv->setFileId("file1");
    rv->setName("myFile");
    return rv;
};

std::unique_ptr<MoveFileArg> MoveFileArg::EXAMPLE(int)
{
    std::unique_ptr<MoveFileArg> rv(new MoveFileArg);
    rv->setFileId("file1");
    rv->setName("myFile");
    std::list<QString> lstFrom, lstTo;
    for(int i = 0; i < 5;i++)
        {
            lstFrom.push_back(QString("from%1").arg(i));
            lstTo.push_back(QString("to%1").arg(i));
        }
    rv->setRemoveParents(lstFrom);
    rv->setAddParents(lstTo);
    return rv;
};

std::unique_ptr<CreateFolderArg> CreateFolderArg::EXAMPLE(int)
{
    std::unique_ptr<CreateFolderArg> rv(new CreateFolderArg);
    rv->setName("myNewFolder");
    rv->setDescription("description of myNewFolder");
    std::list<QString> lstParents;
    for (int i = 0; i < 5; i++)
    {
        lstParents.push_back(QString("parent1").arg(i));
    }
    rv->setParents(lstParents);
    return rv;
};

std::unique_ptr<MultipartUploadFileArg> MultipartUploadFileArg::EXAMPLE(int)
{
    std::unique_ptr<MultipartUploadFileArg> rv(new MultipartUploadFileArg);
    rv->setName("myFile");
    return rv;
};


#endif //API_QT_AUTOTEST

