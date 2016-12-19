#include <iostream>
#include <functional>
#include <QBuffer>
#include <QFile>
#include <QFileInfo>
#include <iomanip>
#include "GdriveCommands.h"
#include "google/demo/ApiTerminal.h"
#include "gdrive/GdriveRoutes.h"


using namespace googleQt;
using namespace gdrive;

GdriveCommands::GdriveCommands(GoogleClient& c):m_c(c)
{
    m_gd = m_c.gdrive();
};

static QString size_human(qreal num)
{
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while(num >= 1024.0 && i.hasNext())
        {
            unit = i.next();
            num /= 1024.0;
        }
    return QString().setNum(num, 'f', 2) + " " + unit;
}

void GdriveCommands::about(QString)
{    
    try
    {
        AboutArg arg;
        arg.setFields("user(displayName,emailAddress), storageQuota(limit,usage)");
        auto a = m_gd->getAbout()->get(arg);
        const about::UserInfo& u = a->user();
        const about::StorageQuota& q = a->storagequota();
        std::cout << "name=" << u.displayname()
                  << " email=" << u.emailaddress()
                  << std::endl;
        
        std::cout << "used=" << size_human(q.usage())
                  << " limit=" << size_human(q.limit())
                  << std::endl;
        
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GdriveCommands::ls(QString nextToken)
{
    try
    {
        FileListArg arg(nextToken);
        auto lst = m_gd->getFiles()->list(arg);
        const std::list <files::FileResource>& files = lst->files();
        int idx = 1;
        for(const files::FileResource& f : files){
            QString mimeType = f.mimetype();
            QString ftype = "[f]";
            if(mimeType == "application/vnd.google-apps.folder"){
                ftype = "[d]";
            }else if(mimeType == "image/jpeg"){
                ftype = "[i]";
            }
            std::cout << QString("%1").arg(idx++).leftJustified(3, ' ')
                      << ftype << " "
                      << f.id() << " "
                      << f.name()<< " "
                      << mimeType << std::endl;
        }
        std::cout << "next token: " << lst->nextpagetoken() << std::endl;
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }    
};

void GdriveCommands::get(QString fileId)
{
    if(fileId.isEmpty())
        {
            std::cout << "fileId required" << std::endl;
            return;
        }
    
    try
    {
        GetFileArg arg(fileId);
        arg.setFields("id,name,size,mimeType,webContentLink");
        auto f = m_gd->getFiles()->get(arg);
        std::cout << "id= " << f->id() << std::endl
                  << "name= " << f->name() << std::endl
                  << "type= " << f->mimetype() << std::endl
                  << "size= " << f->size() << std::endl
                  << "webLink= " << f->webcontentlink() << std::endl;
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GdriveCommands::rename(QString fileId_space_new_title) 
{
    QStringList arg_list = fileId_space_new_title.split(" ",
        QString::SkipEmptyParts);
    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <fileID> <fileName>" << std::endl;
        return;
    }

    QString fileId = arg_list[0];
    QString fileName = arg_list[1];

    try
    {    
        RenameFileArg arg(fileId);
        arg.setName(fileName);
        arg.setFields("id,name,size,mimeType,webContentLink");
        auto f = m_gd->getFiles()->rename(arg);
        std::cout << "id= " << f->id() << std::endl
                  << "name= " << f->name() << std::endl
                  << "type= " << f->mimetype() << std::endl
                  << "size= " << f->size() << std::endl
                  << "webLink= " << f->webcontentlink() << std::endl;
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }    
}


void GdriveCommands::download(QString fileId_space_localFileName) 
{
    QStringList arg_list = fileId_space_localFileName.split(" ",
        QString::SkipEmptyParts);
    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <fileID> <fileName>" << std::endl;
        return;
    }

    QString fileId = arg_list[0];
    QString fileName = arg_list[1];

    QFile out(fileName);
    if (!out.open(QFile::WriteOnly | QIODevice::Truncate)) {
        std::cout << "Error opening file: " << fileName.toStdString() << std::endl;
        return;
    }

    try
    {
        DownloadFileArg arg(fileId);
        m_gd->getFiles()->downloadFile(arg, &out);
        std::cout << "file downloaded" << std::endl;
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    out.close();
};

void GdriveCommands::cat(QString fileId) 
{
    if (fileId.isEmpty())
    {
        std::cout << "fileId required" << std::endl;
        return;
    }

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);

    try
    {
        DownloadFileArg arg(fileId);
        m_gd->getFiles()->downloadFile(arg, &buffer);
        std::cout << byteArray.constData();
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    buffer.close();
};

void GdriveCommands::put(QString fileName) 
{
    if (fileName.isEmpty()) {
        std::cout << "ERROR argument reguired" << std::endl;
        return;
    }

    QFile file_in(fileName);
    if (!file_in.open(QFile::ReadOnly)) {
        std::cout << "Error opening file: " << fileName.toStdString() << std::endl;
        return;
    }

    QFileInfo fi(fileName);
    QString doxFileName = fi.fileName();

    try
    {
        files::FileResource fres;
        fres.setName(doxFileName);
        auto f = m_gd->getFiles()->uploadFile(fres, &file_in);
        std::cout << "file uploaded" << std::endl;
        std::cout << "id= " << f->id() << std::endl
            << "name= " << f->name() << std::endl
            << "type= " << f->mimetype() << std::endl
            << "size= " << f->size() << std::endl
            << "webLink= " << f->webcontentlink() << std::endl;
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    file_in.close();
};

void GdriveCommands::put_simple(QString fileName) 
{
    if (fileName.isEmpty()) {
        std::cout << "ERROR argument reguired" << std::endl;
        return;
    }

    QFile file_in(fileName);
    if (!file_in.open(QFile::ReadOnly)) {
        std::cout << "Error opening file: " << fileName.toStdString() << std::endl;
        return;
    }

    try
    {
        auto f = m_gd->getFiles()->uploadFileSimple(&file_in);
        std::cout << "file uploaded" << std::endl;
        std::cout << "id= " << f->id() << std::endl
            << "name= " << f->name() << std::endl
            << "type= " << f->mimetype() << std::endl
            << "size= " << f->size() << std::endl
            << "webLink= " << f->webcontentlink() << std::endl;
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    file_in.close();
};

void GdriveCommands::rm(QString fileId) 
{
    if (fileId.isEmpty())
    {
        std::cout << "fileId required" << std::endl;
        return;
    }

    try
    {
        DeleteFileArg arg(fileId);
        m_gd->getFiles()->deleteOperation(arg);
        std::cout << "deleted" << std::endl;
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GdriveCommands::mkdir(QString title_Space_parentFolderId)
{
    QStringList arg_list = title_Space_parentFolderId.split(" ",
                                                        QString::SkipEmptyParts);
    if(arg_list.size() < 1)
        {
            std::cout << "Invalid parameters, expected <title> <parent_id>" << std::endl;
            return;
        }
    
    QString title = arg_list[0];
    QString parentId = "";
    if(arg_list.size() > 1){
        parentId = arg_list[1];
    }

    try
        {
            
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GdriveCommands::ls_comments(QString fileId)
{
    try
        {
            CommentListArg arg(fileId);
            arg.setFields("comments(content,id)");
            auto col = m_gd->getComments()->list(arg);
            std::list <comments::Comment> com_list = col->comments();
            int idx = 1;
            for(const comments::Comment& c : com_list){
                std::cout << idx++ << ". " << c.id() << " " << c.content() << std::endl;
            }
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GdriveCommands::rm_comment(QString fileId_Space_commentId)
{
    QStringList arg_list = fileId_Space_commentId.split(" ",
                                                        QString::SkipEmptyParts);
    if(arg_list.size() < 2)
        {
            std::cout << "Invalid parameters, expected <file_id> <comment_id>" << std::endl;
            return;
        }
    
    QString fileId = arg_list[0];
    QString commentId = arg_list[1];
    try
        {
            DeleteCommentArg arg(fileId, commentId);
            m_gd->getComments()->deleteOperation(arg);
            std::cout << "comment deleted" << std::endl;
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GdriveCommands::get_comment(QString fileId_Space_commentId)
{
    QStringList arg_list = fileId_Space_commentId.split(" ",
                                                        QString::SkipEmptyParts);
    if(arg_list.size() < 2)
        {
            std::cout << "Invalid parameters, expected <file_id> <comment_id>" << std::endl;
            return;
        }
    
    QString fileId = arg_list[0];
    QString commentId = arg_list[1];    
    try
    {
        GetCommentArg arg(fileId, commentId);
        arg.setFields("content,id");
        auto c = m_gd->getComments()->get(arg);
        std::cout << c->id() << " " << c->content() << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GdriveCommands::new_comment(QString fileId_Space_content)
{
    QStringList arg_list = fileId_Space_content.split(" ",
                                                        QString::SkipEmptyParts);
    if(arg_list.size() < 2)
        {
            std::cout << "Invalid parameters, expected <file_id> <comment_id>" << std::endl;
            return;
        }
    QString fileId = arg_list[0];
    QString content = arg_list[1];
    try
    {
        CreateCommentArg arg(fileId);
        arg.setFields("content,id");
        comments::Comment cmt;
        cmt.setContent(content);
        auto c = m_gd->getComments()->create(arg, cmt);
        std::cout << c->id() << " " << c->content() << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GdriveCommands::ls_permissions(QString fileId)
{
    try
        {
            PermissionListArg arg(fileId);
            auto col = m_gd->getPermissions()->list(arg);
            std::list <permissions::ResourcePermission> p_list = col->permissions();
            int idx = 1;
            for(const permissions::ResourcePermission& p : p_list){
                std::cout << idx++ << ". " << p.id() << " " << p.role() << " " << p.type() << std::endl;
            }
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GdriveCommands::get_permission(QString fileId_Space_permissionId)
{
    QStringList arg_list = fileId_Space_permissionId.split(" ",
                                                        QString::SkipEmptyParts);
    if(arg_list.size() < 2)
        {
            std::cout << "Invalid parameters, expected <file_id> <comment_id>" << std::endl;
            return;
        }
    
    QString fileId = arg_list[0];
    QString permissionId = arg_list[1];    
    try
    {
        PermissionArg arg(fileId, permissionId);
        auto p = m_gd->getPermissions()->get(arg);
        std::cout << p->id() << " " << p->role() << " " << p->type() << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GdriveCommands::print_last_result(QString )
{
    m_c.printLastResponse();
};
