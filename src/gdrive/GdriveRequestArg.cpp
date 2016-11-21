#include <QUrlQuery>
#include "GdriveRequestArg.h"

using namespace googleQt;
using namespace gdrive;

/**
   AboutArg
*/
AboutArg::AboutArg()
{

};

void AboutArg::addResponseField(QString name)
{
    m_partResponseFields.insert(name);
};

void AboutArg::clearResponseFields()
{
    m_partResponseFields.clear();
};

void AboutArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/%1").arg(path_about::path()), url);
    if(!m_partResponseFields.empty()){
        QString fields = "";
        for(QString f : m_partResponseFields){
            fields += f;
            fields += ",";
        }
        fields = fields.left(fields.length() - 1);
        b.add("fields", fields);
    }
};


/**
	FileListArg
*/
FileListArg::FileListArg():
	m_corpus("user"),
	m_pageSize(100)
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

DeleteFileArg::DeleteFileArg()
{

}

void DeleteFileArg::build(const QString& link_path, QUrl& url)const
{
	UrlBuilder b(link_path + "/files", url);
	b.add("fileId", m_fileId);
}

/**
	CreateFileArg
*/
CreateFileArg::CreateFileArg()
{
	m_uploadType = "media";
	m_ignoreDefaultVisibility = false;
	m_keepRevisionForever = false;
	m_useContentAsIndexableText = false;
};

void CreateFileArg::build(const QString& link_path, QUrl& url)const
{
	UrlBuilder b(link_path + "/files", url);
	b.add("uploadType", m_uploadType)
		.add("ignoreDefaultVisibility", m_ignoreDefaultVisibility)
		.add("keepRevisionForever", m_keepRevisionForever)
		.add("ocrLanguage", m_ocrLanguage)
		.add("useContentAsIndexableText", m_useContentAsIndexableText);
}

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
std::unique_ptr<FileListArg> FileListArg::EXAMPLE()
{
	std::unique_ptr<FileListArg> rv(new FileListArg);
	rv->setOrderBy("modifiedTime");
	rv->setPageToken("next-page");
	rv->setQ("file=my");
	rv->setSpaces("drive,appDataFolder,photos");
	return rv;
};

std::unique_ptr<GetFileArg> GetFileArg::EXAMPLE()
{
	std::unique_ptr<GetFileArg> rv(new GetFileArg);
	rv->setFileId("file1");
	return rv;
};

std::unique_ptr<CopyFileArg> CopyFileArg::EXAMPLE()
{
	std::unique_ptr<CopyFileArg> rv(new CopyFileArg);
	rv->setFileId("file1");
	return rv;
};

std::unique_ptr<DeleteFileArg> DeleteFileArg::EXAMPLE() 
{
	std::unique_ptr<DeleteFileArg> rv(new DeleteFileArg);
	rv->setFileId("file1");
	return rv;
};

std::unique_ptr<CreateFileArg> CreateFileArg::EXAMPLE() 
{
	std::unique_ptr<CreateFileArg> rv(new CreateFileArg);	
	return rv;
};

std::unique_ptr<PermissionArg> PermissionArg::EXAMPLE()
{
	std::unique_ptr<PermissionArg> rv(new PermissionArg);
	rv->setFileId("file1");
	rv->setPermissionId("permission1");
	return rv;
};

std::unique_ptr<CreatePermissionArg> CreatePermissionArg::EXAMPLE()
{
	std::unique_ptr<CreatePermissionArg> rv(new CreatePermissionArg);
	rv->setFileId("file1");
	return rv;
};

std::unique_ptr<PermissionListArg> PermissionListArg::EXAMPLE()
{
	std::unique_ptr<PermissionListArg> rv(new PermissionListArg);
	rv->setFileId("file1");
	return rv;
};

std::unique_ptr<CreateCommentArg> CreateCommentArg::EXAMPLE()
{
	std::unique_ptr<CreateCommentArg> rv(new CreateCommentArg);
	rv->setFileId("file1");
	return rv;
};

std::unique_ptr<DeleteCommentArg> DeleteCommentArg::EXAMPLE()
{
	std::unique_ptr<DeleteCommentArg> rv(new DeleteCommentArg);
	rv->setFileId("file1");
	rv->setCommentId("comment1");
	return rv;
};

std::unique_ptr<GetCommentArg> GetCommentArg::EXAMPLE()
{
	std::unique_ptr<GetCommentArg> rv(new GetCommentArg);
	rv->setFileId("file1");
	rv->setCommentId("comment1");
	return rv;
};

std::unique_ptr<CommentListArg> CommentListArg::EXAMPLE() 
{
	std::unique_ptr<CommentListArg> rv(new CommentListArg);
	rv->setFileId("file1");
	rv->setStartModifiedTime(QDateTime::currentDateTime());
	rv->setPageToken("page-token");
	return rv;
};

#endif //API_QT_AUTOTEST

