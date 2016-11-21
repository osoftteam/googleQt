#pragma once
#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"

#include "gdrive/about/AboutRoutes.h"
#include "gdrive/files/FilesRoutes.h"
#include "gdrive/permissions/PermissionsRoutes.h"
#include "gdrive/comments/CommentsRoutes.h"

namespace googleQt
{
	class Endpoint;
    
    class GdriveRoutes{	
    public:

        GdriveRoutes(Endpoint*);
        Endpoint* endpoint() { return m_endpoint; }

        files::FilesRoutes* getFiles();
        about::AboutRoutes* getAbout();
		permissions::PermissionsRoutes* getPermissions();
		comments::CommentsRoutes* getComments();
    protected:
        std::unique_ptr<files::FilesRoutes> m_Files;
        std::unique_ptr<about::AboutRoutes> m_About;
		std::unique_ptr<permissions::PermissionsRoutes> m_Permissions;
		std::unique_ptr<comments::CommentsRoutes> m_Comments;
        Endpoint*  m_endpoint;
    };
};//googleQt
