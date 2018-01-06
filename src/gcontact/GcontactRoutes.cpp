#include "GcontactRoutes.h"
using namespace googleQt;

GcontactRoutes::GcontactRoutes(Endpoint* e):m_endpoint(e)
{
    m_CacheRoutes.reset(new googleQt::gcontact::GcontactCacheRoutes(*e, *this));
};

contacts::ContactsRoutes* GcontactRoutes::getContacts()
{
  if(!m_Contacts){
    m_Contacts.reset(new contacts::ContactsRoutes(m_endpoint));
  }
  return m_Contacts.get();
};

googleQt::gcontact::GcontactCacheRoutes* GcontactRoutes::cacheRoutes() 
{
    return m_CacheRoutes.get();
};

#ifdef API_QT_AUTOTEST
void GcontactRoutes::autotest() 
{
    m_CacheRoutes->runAutotest();
};
#endif
