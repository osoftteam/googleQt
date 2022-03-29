#include <iostream>
#include <functional>
#include <QBuffer>
#include <QFile>
#include <QFileInfo>
#include <QNetworkProxy>
#include <QCryptographicHash>
#include <iomanip>
#include <QDir>
#include "GdriveCommands.h"
#include "google/demo/ApiTerminal.h"
#include "gdrive/GdriveRoutes.h"
#include "gdrive/files/FilesCreateFileDetails.h"
#include "gdrive/files/FilesUpdateFileDetails.h"

using namespace googleQt;
using namespace gdrive;

GdriveCommands::GdriveCommands(GoogleClient& c):m_c(c)
{
    m_gd = m_c.gdrive();
};

void GdriveCommands::about(QString)
{    
    try
        {
            AboutArg arg;
            arg.setFields("user(displayName,emailAddress,permissionId), storageQuota(limit,usage)");
            auto a = m_gd->getAbout()->get(arg);
            const about::UserInfo& u = a->user();
            const about::StorageQuota& q = a->storagequota();
            std::cout << "permissionID=" << u.permissionid() << std::endl;
            std::cout << "name=" << u.displayname()
                      << " email=" << u.emailaddress()
                      << std::endl;            
            
            std::cout << "used=" << size_human(q.usage())
                      << " limit=" << size_human(q.limit())
                      << std::endl;        
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
            auto& files = lst->files();
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
            if(!lst->nextpagetoken().isEmpty())
                {
                    std::cout << "next token: " << lst->nextpagetoken() << std::endl;
                }
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }    
};

void GdriveCommands::ls_folders(QString nextToken)
{
    std::string name_filter;
    std::cout << "filter name? [string]" << std::endl;
    getline(std::cin, name_filter);
    
    try
        {
            FileListArg arg(nextToken);
            arg.setQ("mimeType = 'application/vnd.google-apps.folder'");
            auto lst = m_gd->getFiles()->list(arg);
            auto& files = lst->files();
            int idx = 1;
            for(const files::FileResource& f : files){
                if(!name_filter.empty()){
                    auto p = f.name().indexOf(name_filter.c_str());
                    if(p == -1)
                        continue;
                }
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
                          << f.name();
                std::cout << std::endl;
            }
            std::cout << "next token: " << lst->nextpagetoken() << std::endl;
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }    
};

void GdriveCommands::ls_dir_content(QString folderId)
{
    if(folderId.isEmpty())
        {
            std::cout << "folderId required" << std::endl;
            return;
        }
    
    try
        {
            FileListArg arg;
            arg.setQ(QString("'%1' in parents").arg(folderId));
            auto lst = m_gd->getFiles()->list(arg);
            auto& files = lst->files();
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
            if(!lst->nextpagetoken().isEmpty())
                {
                    std::cout << "next token: " << lst->nextpagetoken() << std::endl;
                }
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }    
};

void GdriveCommands::clean_dir_content(QString folderId)
{
    if(folderId.isEmpty())
        {
            std::cout << "folderId required" << std::endl;
            return;
        }
    
    try
        {
            bool deleteAll = false;
        
            FileListArg arg;
            arg.setQ(QString("'%1' in parents").arg(folderId));
            auto lst = m_gd->getFiles()->list(arg);
            auto& files = lst->files();
            int idx = 1;
            for(const files::FileResource& f : files){
                std::string tmp;
                if(!deleteAll)
                    {
            
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
                        std::cout << "delete? [Y/n/REMOVE-ALL/END]" << std::endl;;
                        getline(std::cin, tmp);
                    }
                if(deleteAll || tmp.compare("Y") == 0)
                    {
                        DeleteFileArg arg(f.id());
                        m_gd->getFiles()->deleteOperation(arg);
                        std::cout << "deleted: " << f.id() << std::endl;
                    }
                else if(tmp.compare("Y") == 0)
                    {
                        //skip
                    }
                else if(tmp.compare("END") == 0)
                    {
                        return;
                    }
                else if(tmp.compare("REMOVE-ALL") == 0)
                    {
                        DeleteFileArg arg(f.id());
                        m_gd->getFiles()->deleteOperation(arg);
                        std::cout << "deleted: " << f.id() << std::endl;
                        deleteAll = true;
                    }
            
            }
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }    
};

void GdriveCommands::ls_space_content(QString spaceName)
{
    try
        {
            if(spaceName.isEmpty())
                {
                    spaceName = "appDataFolder";
                }
            
            FileListArg arg;
            arg.setSpaces(spaceName);
            auto lst = m_gd->getFiles()->list(arg);
            auto& files = lst->files();
            if(files.size() == 0)
                {
                    std::cout << "Empty space " << spaceName << std::endl;
                    return;
                }
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
            if(!lst->nextpagetoken().isEmpty())
                {
                    std::cout << "next token: " << lst->nextpagetoken() << std::endl;
                }
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }    
};

void GdriveCommands::clean_space_content(QString spaceName)
{
    try
        {
            bool deleteAll = false;
        
            FileListArg arg;
            arg.setSpaces(spaceName);
            auto lst = m_gd->getFiles()->list(arg);
            auto& files = lst->files();
            int idx = 1;
            for(const files::FileResource& f : files)
                {
                    std::string tmp;
                    if(!deleteAll)
                        {
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
                            std::cout << "delete? [Y/n/REMOVE-ALL/END]" << std::endl;;
                            getline(std::cin, tmp);
                        }
                    if(deleteAll || tmp.compare("Y") == 0)
                        {
                            DeleteFileArg arg(f.id());
                            m_gd->getFiles()->deleteOperation(arg);
                            std::cout << "deleted: " << f.id() << std::endl;
                        }
                    else if(tmp.compare("Y") == 0)
                        {
                            //skip
                        }
                    else if(tmp.compare("END") == 0)
                        {
                            return;
                        }
                    else if(tmp.compare("REMOVE-ALL") == 0)
                        {
                            DeleteFileArg arg(f.id());
                            m_gd->getFiles()->deleteOperation(arg);
                            std::cout << "deleted: " << f.id() << std::endl;
                            deleteAll = true;
                        }
                }
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }    
};

void GdriveCommands::rm_appdata_files(QString name_exceptId)
{
    QStringList arg_list = name_exceptId.split(" ",
                                               Qt::SkipEmptyParts);
    if(arg_list.size() == 0)
        {
            std::cout << "Invalid parameters, expected <fileName> <fileId> (to skip)" << std::endl;
            return;
        }
    
    QString name;
    QString fileId;

    if(arg_list.size() > 0)
        name = arg_list[0];
    if(arg_list.size() > 1)
        fileId = arg_list[1];

    
    int n = m_gd->cleanUpAppDataFolder(name, fileId);
    if(n == 0)
        {
            std::cout << "no files deleted" << std::endl;
        }
    else if(n > 0)
        {
            std::cout << "files deleted: " << n << std::endl;
        }
    else if(n < 0)
        {
            std::cout << "error cleaning up folder" << std::endl;
        }
};


void GdriveCommands::find_by_name(QString name_space_parentId)
{
    QStringList arg_list = name_space_parentId.split(" ",
                                                     Qt::SkipEmptyParts);
    QString name;
    QString parentId;

    if(arg_list.size() > 0)
        name = arg_list[0];
    if(arg_list.size() > 1)
        parentId = arg_list[1];

    if(arg_list.size() == 0)
        {
            std::cout << "Invalid parameters, expected <fileName> " << std::endl;
            return;
        }
    
    
    try
        {
            FileListArg arg;
            QString q = QString("name contains '%1'").arg(name);
            if (!parentId.isEmpty()) 
                {
                    q += QString(" and '%1' in parents").arg(parentId);
                }            
            arg.setQ(q);
            auto lst = m_gd->getFiles()->list(arg);
            auto& files = lst->files();
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
            if(!lst->nextpagetoken().isEmpty())
                {
                    std::cout << "next token: " << lst->nextpagetoken() << std::endl;
                }

            //            print_last_result("");
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
            arg.setFields("id,name,size,mimeType,webContentLink,webViewLink,parents,spaces,modifiedByMeTime");
            auto f = m_gd->getFiles()->get(arg);
            std::cout << "id= " << f->id() << std::endl
                      << "name= " << f->name() << std::endl
                      << "type= " << f->mimetype() << std::endl
                      << "size= " << f->size() << std::endl
                      << "modified= " << f->modifiedbymetime().toString() << std::endl;
            if(!f->webcontentlink().isEmpty())
                {
                    std::cout << "webContentLink= " << f->webcontentlink() << std::endl;
                }
            if(!f->webviewlink().isEmpty())
                {
                    std::cout << "webViewLink= " << f->webviewlink() << std::endl;
                }            
            if(f->parents().size() != 0)
                {
                    std::cout << "=== parents ===" << std::endl;
                    for(auto j = f->parents().begin(); j != f->parents().end(); j++)
                        {
                            std::cout << *j << std::endl;
                        }
                }
            if(f->spaces().size() != 0)
                {
                    std::cout << "=== spaces ===" << std::endl;
                    for(auto j = f->spaces().begin(); j != f->spaces().end(); j++)
                        {
                            std::cout << *j << std::endl;
                        }
                }        
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GdriveCommands::rename(QString fileId_space_new_title) 
{
    QStringList arg_list = fileId_space_new_title.split(" ",
                                                        Qt::SkipEmptyParts);
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
            print_status(f.get(), "renamed");
        }    
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }    
}

void GdriveCommands::change_mime(QString fileId_space_new_mimeType) 
{
    std::cout << "Google doesn't do it (Jan/2017) - you have to upload file again" << std::endl;
    
    QStringList arg_list = fileId_space_new_mimeType.split(" ",
                                                           Qt::SkipEmptyParts);
    if (arg_list.size() < 2)
        {
            std::cout << "Invalid parameters, expected <fileID> <mimeType>" << std::endl;
            std::cout << "png = > 'image/png'" << std::endl
                      << "gif = >'image/gif'" << std::endl
                      << "bmp = >'image/bmp'" << std::endl
                      << "txt = >'text/plain'" << std::endl
                      << "doc = >'application/msword'" << std::endl
                      << "js = >'text/js'" << std::endl
                      << "swf = >'application/x-shockwave-flash'" << std::endl
                      << "mp3 = >'audio/mpeg'" << std::endl
                      << "zip = >'application/zip'" << std::endl
                      << "rar = >'application/rar'" << std::endl
                      << "tar = >'application/tar'" << std::endl;
        
            return;
        }

    QString fileId = arg_list[0];
    QString mimeType = arg_list[1];

    try
        {
            UpdateFileArg arg(fileId);
            files::UpdateFileDetails& file_details = arg.fileDetailes();
            file_details.setMimetype(mimeType);
            arg.setFields("id,name,size,mimeType,webContentLink");
            auto f = m_gd->getFiles()->updateFileMeta(arg);
            print_status(f.get(), "mime updated");
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GdriveCommands::move_file(QString fileId)
{
    if(fileId.isEmpty())
        {
            std::cout << "fileId required" << std::endl;
            return;
        }
    
    std::string tmp;
    std::cout << "Enter parents ID(space separated) from which object will be removed (or SPACE to none)" << std::endl;
    std::cout << "from>";
    getline(std::cin, tmp);
    STRING_LIST remove_list = split_string(QString(tmp.c_str()));

    std::cout << "Enter parents ID(space separated) of destination folders (or SPACE to none)" << std::endl;
    std::cout << "to>";
    getline(std::cin, tmp);
    STRING_LIST add_list = split_string(QString(tmp.c_str()));
    
    try
        {
            MoveFileArg arg(fileId);
            arg.setFields("id,name,size,mimeType,webContentLink");
            arg.setRemoveParents(remove_list);
            arg.setAddParents(add_list);
            auto f = m_gd->getFiles()->moveFile(arg);
            print_status(f.get(), "moved");
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GdriveCommands::download(QString fileId) 
{
    if(fileId.isEmpty())
        {
            std::cout << "fileId required" << std::endl;
            return;
        }
    
    QFile out;
    try
        {
            GetFileArg arg1(fileId);
            arg1.setFields("name");
            auto f = m_gd->getFiles()->get(arg1);
            std::cout << "loading file: " << f->name() << std::endl;

            QDir d;
            if(!d.mkpath("downloads")){
                std::cout << "Failed to create 'downloads' directory." << std::endl;
                return;
            }
            
            out.setFileName("downloads/" + f->name());
            if (!out.open(QFile::WriteOnly | QIODevice::Truncate)) {
                std::cout << "Error opening file: " << out.fileName() << std::endl;
                return;
            }            
            
            DownloadFileArg arg(fileId);
            m_gd->getFiles()->downloadFile(arg, &out);
            out.flush();
            std::cout << "=== file downloaded ===" << std::endl;
            std::cout << size_human(out.size()) << std::endl;
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
            std::cout << "=== file content " << size_human(buffer.size()) << " ===" << std::endl;
            std::cout << byteArray.constData() << std::endl;
            QString hash_s = QCryptographicHash::hash((byteArray.constData()),
                                                      QCryptographicHash::Md5).toHex();
            std::cout << "hash: " << hash_s.toStdString() << std::endl;
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    buffer.close();
};

void GdriveCommands::hash_file(QString fileId) 
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
            std::cout << "size: " << size_human(buffer.size()) << std::endl;
            QString hash_s = QCryptographicHash::hash((byteArray.constData()),
                                                      QCryptographicHash::Md5).toHex();
            std::cout << "hash: " << hash_s.toStdString() << std::endl;
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }

    buffer.close();
};

void GdriveCommands::create(QString fileName) 
{
    if (fileName.isEmpty()) {
        std::cout << "ERROR <fileName> reguired" << std::endl;
        return;
    }

    QFile file_in(fileName);
    if (!file_in.open(QFile::ReadOnly)) {
        std::cout << "Error opening file: " << fileName.toStdString() << std::endl;
        return;
    }

    QFileInfo fi(fileName);

    try
        {
            gdrive::CreateFileArg arg(fi.fileName());
            files::CreateFileDetails& file_details = arg.fileDetailes();
            file_details.setDescription("file created from googleQt API");
            auto f = m_gd->getFiles()->create(arg, &file_in);
            print_status(f.get(), QString("uploaded %1").arg(file_in.size()));
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    file_in.close();
};

void GdriveCommands::create_in_appdata(QString fileName) 
{
    if (fileName.isEmpty()) {
        std::cout << "ERROR <fileName> reguired" << std::endl;
        return;
    }

    QFile file_in(fileName);
    if (!file_in.open(QFile::ReadOnly)) {
        std::cout << "Error opening file: " << fileName.toStdString() << std::endl;
        return;
    }

    QFileInfo fi(fileName);

    try
        {
            gdrive::CreateFileArg arg(fi.fileName());
            files::CreateFileDetails& file_details = arg.fileDetailes();
            std::vector<QString> parent_folders;
            parent_folders.push_back("appDataFolder");
            file_details.setParents(parent_folders);
            file_details.setDescription("file created from googleQt API");
            auto f = m_gd->getFiles()->create(arg, &file_in);
            print_status(f.get(), QString("uploaded %1").arg(file_in.size()));
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    file_in.close();
};

void GdriveCommands::upload_simple(QString fileName) 
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
            print_status(f.get(), QString("uploaded %1").arg(file_in.size()));
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
    file_in.close();
};

void GdriveCommands::upgrade_file(QString localFile_parentFolderId)
{
    QStringList arg_list = localFile_parentFolderId.split(" ",
                                                          Qt::SkipEmptyParts);
    if (arg_list.isEmpty())
        {
            std::cout << "<fileName> required(local-path), optional <parentId>" << std::endl;
            return;
        }

    
    QString parentId;

    QString filePath = arg_list[0];
    if(arg_list.size() > 1)
        parentId = arg_list[1];

    QFileInfo fi(filePath);
    QString fileName = fi.fileName();
    
    QString fId = m_gd->upgradeFile(filePath, fileName, parentId);
    if(fId.isEmpty())
        {
            std::cout << "error upgrading file" << std::endl;
        }
    else
        {
            std::cout << "upgraded: " << fId << std::endl;
        }
};

void GdriveCommands::create_using_id(QString fileName_space_fileId)
{
    QStringList arg_list = fileName_space_fileId.split(" ",
        Qt::SkipEmptyParts);
    if (arg_list.size() != 2)
    {
        std::cout << "<fileName> required(local-path) <fileId>" << std::endl;
        return;
    }

    QString filePath = arg_list[0];
    QString fileId = arg_list[1];

    QFileInfo fi(filePath);
    QString fileName = fi.fileName();

    auto res = m_gd->uploadFileUsingId(filePath, fileName, fileId);
    if (!res.first)
    {
        std::cout << "error creating file: " << res.second << std::endl;
    }
    else
    {
        std::cout << "created: " << fileId << std::endl;
    }
};

void GdriveCommands::rm(QString arg) 
{
    if (arg.isEmpty())
        {
            std::cout << "fileId required" << std::endl;
            return;
        }

    QStringList arg_list = arg.split(" ",
                                     Qt::SkipEmptyParts);
    
    try
        {
            for(auto i = arg_list.begin(); i != arg_list.end(); i++)
                {
                    QString fileId = *i;
                    DeleteFileArg arg(fileId);
                    m_gd->getFiles()->deleteOperation(arg);
                    std::cout << "deleted: " << fileId << std::endl;
                }
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GdriveCommands::mkdir(QString title_Space_parentFolderId)
{
    QStringList arg_list = title_Space_parentFolderId.split(" ",
                                                            Qt::SkipEmptyParts);
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
            CreateFolderArg arg(title);
            arg.setFields("id,name,mimeType");
            auto f = m_gd->getFiles()->createFolder(arg);
            print_status(f.get(), "folder created");
        }
    catch (GoogleException& e)
        {
            std::cout << "Exception: " << e.what() << std::endl;
        }
};

void GdriveCommands::generate_ids(QString) 
{
    try
    {
        GenerateIdArg arg("drive");
        auto f = m_gd->getFiles()->generateIds(arg);
        auto lst = f->ids();
        for (QString& id : lst) {
            std::cout << id << std::endl;
        }
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GdriveCommands::ls_comments(QString fileId)
{
    if (fileId.isEmpty())
        {
            std::cout << "fileId required" << std::endl;
            return;
        }
    
    try
        {
            CommentListArg arg(fileId);
            arg.setFields("comments(content,id)");
            auto col = m_gd->getComments()->list(arg);
            std::vector<comments::Comment> com_list = col->comments();
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
                                                        Qt::SkipEmptyParts);
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
                                                        Qt::SkipEmptyParts);
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
    int spaceIdx = fileId_Space_content.indexOf(" ");
    if(spaceIdx == -1)
        {
            std::cout << "Invalid parameters, expected <file_id> <comment text>" << std::endl;
            return;
        }
    
    QString fileId = fileId_Space_content.left(spaceIdx).trimmed();
    QString content = fileId_Space_content.mid(spaceIdx).trimmed();
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
    if (fileId.isEmpty())
        {
            std::cout << "fileId required" << std::endl;
            return;
        }
    
    try
        {
            PermissionListArg arg(fileId);
            auto col = m_gd->getPermissions()->list(arg);
            std::vector<permissions::ResourcePermission> p_list = col->permissions();
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
                                                           Qt::SkipEmptyParts);
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

void GdriveCommands::create_permissions(QString fileId) 
{
    if (fileId.isEmpty())
    {
        std::cout << "fileId required" << std::endl;
        return;
    }

    try
    {
        permissions::ResourcePermission p_new;
        p_new.setRole("reader");
        p_new.setType("anyone");

        CreatePermissionArg arg;
        arg.setFileId(fileId);
        auto p = m_gd->getPermissions()->create(arg, p_new);
        std::cout << "permission created " << p->id() << " " << p->role() << " " << p->type() << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GdriveCommands::delete_permissions(QString fileId_Space_permissionId)
{
    QStringList arg_list = fileId_Space_permissionId.split(" ",
        Qt::SkipEmptyParts);
    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <file_id> <permission_id>" << std::endl;
        return;
    }

    QString fileId = arg_list[0];
    QString permissionId = arg_list[1];
    try
    {
        PermissionArg arg(fileId, permissionId);
        m_gd->getPermissions()->deleteOperation(arg);
        std::cout << "permission deleted" << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GdriveCommands::ls_revisions(QString fileId)
{
    if (fileId.isEmpty())
    {
        std::cout << "fileId required" << std::endl;
        return;
    }

    try
    {
        ListRevisionArg arg(fileId);
        auto col = m_gd->getRevisions()->list(arg);
        std::vector<revisions::RevisionResource> r_list = col->files();
        int idx = 1;
        for (const revisions::RevisionResource& r : r_list) {
            std::cout << idx++ << ". " << r.id() << 
                " " << r.modifiedtime().toString() << 
                " " << r.mimetype() <<
                " " << r.md5Checksum() << std::endl;
        }
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GdriveCommands::get_revision(QString fileId_Space_revisionId)
{
    QStringList arg_list = fileId_Space_revisionId.split(" ",
        Qt::SkipEmptyParts);
    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <file_id> <revision_id>" << std::endl;
        return;
    }

    QString fileId = arg_list[0];
    QString revisionId = arg_list[1];
    try
    {
        GetRevisionArg arg(fileId, revisionId);
        auto r = m_gd->getRevisions()->get(arg);
        std::cout << r->id() <<
            " modifiedtime=" << r->modifiedtime().toString() <<
            " mimetype=" << r->mimetype() <<
            " md5=" << r->md5Checksum() <<
            " keepforever=" << r->keepforever() <<
            " published=" << r->published() <<
            " publishauto=" << r->publishauto() << std::endl;
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

void GdriveCommands::set_local_proxy(QString port)
{
    int proxyPort = 8888;
    if(!port.isEmpty()){
        proxyPort = port.toInt();
    }
    
    QNetworkProxy proxy(QNetworkProxy::Socks5Proxy, "localhost", proxyPort);
    std::cout << "Proxy:" << proxy.hostName().toStdString() << ":" << proxy.port() << std::endl;
    m_c.setNetworkProxy(proxy);
};

void GdriveCommands::print_status(files::FileResource* f, QString status)
{
    print_status(f, status.toStdString().c_str());
};

void GdriveCommands::print_status(files::FileResource* f, const char* status)
{
    std::cout << "=== " << status << " ===" << std::endl;
    std::cout << "id= " << f->id() << std::endl
              << "name= " << f->name() << std::endl
              << "type= " << f->mimetype() << std::endl
              << "size= " << f->size() << " ("<< size_human(f->size()) << ")" << std::endl;
    if(!f->webcontentlink().isEmpty())
        {
            std::cout << "webLink= " << f->webcontentlink() << std::endl;
        }
};
