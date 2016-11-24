#include "GdriveRoutes.h"

using namespace googleQt;

GdriveRoutes::GdriveRoutes(Endpoint* e):m_endpoint(e)
{
};

files::FilesRoutes* GdriveRoutes::getFiles()
{
  if(!m_Files){
    m_Files.reset(new files::FilesRoutes(m_endpoint));
  }
  return m_Files.get();
};

about::AboutRoutes* GdriveRoutes::getAbout()
{
  if(!m_About){
    m_About.reset(new about::AboutRoutes(m_endpoint));
  }
  return m_About.get();
};


permissions::PermissionsRoutes* GdriveRoutes::getPermissions() 
{
    if (!m_Permissions) {
        m_Permissions.reset(new permissions::PermissionsRoutes(m_endpoint));
    }
    return m_Permissions.get();
};

comments::CommentsRoutes* GdriveRoutes::getComments()
{
    if (!m_Comments) {
        m_Comments.reset(new comments::CommentsRoutes(m_endpoint));
    }
    return m_Comments.get();
};
