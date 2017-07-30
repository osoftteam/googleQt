#pragma once

#include "GoogleClient.h"

using namespace googleQt;

namespace googleQt
{
    namespace files
    {
        class FileResource;
    };
}


class GdriveCommands
{
public:
    GdriveCommands(GoogleClient& c);
    ///get account information
    void about(QString);
    ///list root folder content
    void ls(QString);
    ///list folders in a directory content
    void ls_folders(QString);
    ///list folder content
    void ls_dir_content(QString);
    ///clean folder content
    void clean_dir_content(QString);    
    ///list space content
    void ls_space_content(QString);
    ///clean space content
    void clean_space_content(QString);
    ///get file or folder meta information
    void get(QString fileId);
    ///find file by name
    void find_by_name(QString name_space_parentId);
    ///rename file or folder
    void rename(QString fileId_space_new_title);
    ///change mime type of a file
    void change_mime(QString fileId_space_new_mimeType);
    ///move file or folder
    void move_file(QString fileId);
    ///download file media data to local folder,
    ///required fileID and local file name separated
    ///by space
    void download(QString fileId_space_localFileName);
    ///download and print file content on the screen, good for text files
    void cat(QString fileId);
    ///download, calculate and print hash calculated on file content
    void hash_file(QString fileId);    
    ///create new file by uploading local file
    void create(QString fileName);
    ///create new file in ApplicationDataFolder by uploading local file
    void create_in_appdata(QString fileName);
    ///delete files in appDataFolder by name except ID
    void rm_appdata_files(QString name_exceptId);
    ///upload file using 'simple' method without metadata, fileName - local filename to upload
    void upload_simple(QString fileName);
    /// replace remote file with local using 'upgrade' utility function
    void upgrade_file(QString localFile_parentFolderId);
    ///delete file or folder on gdrive
    void rm(QString fileId);
    ///create folder
    void mkdir(QString title_Space_parentFolderId);
    ///generate some ids
    void generate_ids(QString);
    
    void ls_comments(QString fileId);
    void rm_comment(QString fileId_Space_commentId);
    void get_comment(QString fileId_Space_commentId);
    void new_comment(QString fileId_Space_content);

    void ls_permissions(QString fileId);
    void get_permission(QString fileId_Space_permissionId);
    void print_last_result(QString );
    void set_local_proxy(QString port);
protected:
    void print_status(files::FileResource* f, const char* status);
    void print_status(files::FileResource* f, QString status);
protected:
    GoogleClient& m_c;
    GdriveRoutes*  m_gd;
};
