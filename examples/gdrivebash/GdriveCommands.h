#pragma once

#include "GoogleClient.h"

using namespace googleQt;

class GdriveCommands
{
public:
    GdriveCommands(GoogleClient& c);

    void about(QString);
    void ls(QString);
    void get(QString fileId);
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
