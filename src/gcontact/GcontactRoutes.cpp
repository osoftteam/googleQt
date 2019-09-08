#include "GcontactRoutes.h"
using namespace googleQt;

GcontactRoutes::GcontactRoutes(Endpoint& e/*, gcontact::GContactCacheBase* cb*/)
    :m_endpoint(e)//, m_contacts_cache(cb)
{
    
};

contacts::ContactsRoutes* GcontactRoutes::getContacts()
{
  if(!m_contacts){
    m_contacts.reset(new contacts::ContactsRoutes(&m_endpoint));
  }
  return m_contacts.get();
};

contact_group::ContactGroupRoutes* GcontactRoutes::getContactGroup()
{
    if (!m_contact_group) {
        m_contact_group.reset(new contact_group::ContactGroupRoutes(&m_endpoint));
    }
    return m_contact_group.get();
};


googleQt::gcontact::GcontactCacheRoutes* GcontactRoutes::cacheRoutes() 
{
    if(!m_CacheRoutes){
        m_CacheRoutes.reset(new googleQt::gcontact::GcontactCacheRoutes(m_endpoint, *this));
    }
    
    return m_CacheRoutes.get();
};

QString GcontactRoutes::encodeGroupUri(QString userId, QString groupId)
{
    QString usr = QUrl::toPercentEncoding(userId);
    QString rv = QString("http://www.google.com/m8/feeds/groups/%2/base/%3")
        .arg(usr)
        .arg(groupId);
    return rv;
};

#ifdef API_QT_AUTOTEST
void GcontactRoutes::autotest() 
{
    cacheRoutes()->runAutotest();
};
#endif
