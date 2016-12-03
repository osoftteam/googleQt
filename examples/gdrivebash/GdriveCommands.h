#pragma once

#include "GoogleClient.h"

using namespace googleQt;

class GdriveCommands
{
public:
    GdriveCommands(GoogleClient& c);
    ///get account information
    void about(QString);
    ///list folder content
    void ls(QString);
    ///get file or folder meta information
    void get(QString fileId);
    ///download file media data to local folder,
    ///required fileID and local file name separated
    ///by space
    void download(QString fileId_space_localFileName);
    ///download and print file content on the screen, good for text files
    void cat(QString fileId);
    ///upload file, fileName - local filename to upload
    void put(QString fileName);
    ///delete file or folder on gdrive
    void rm(QString fileId);
    ///create folder
    void mkdir(QString title_Space_parentFolderId);

    void ls_comments(QString fileId);
    void rm_comment(QString fileId_Space_commentId);
    void get_comment(QString fileId_Space_commentId);
    void new_comment(QString fileId_Space_content);

    void ls_permissions(QString fileId);
    void get_permission(QString fileId_Space_permissionId);
protected:
    GoogleClient& m_c;
    GdriveRoutes*  m_gd;
};
