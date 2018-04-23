#include "GcontactRoutes.h"
using namespace googleQt;

GcontactRoutes::GcontactRoutes(Endpoint* e):m_endpoint(e)
{
    m_CacheRoutes.reset(new googleQt::gcontact::GcontactCacheRoutes(*e, *this));
};

contacts::ContactsRoutes* GcontactRoutes::getContacts()
{
  if(!m_contacts){
    m_contacts.reset(new contacts::ContactsRoutes(m_endpoint));
  }
  return m_contacts.get();
};

contact_group::ContactGroupRoutes* GcontactRoutes::getContactGroup()
{
    if (!m_contact_group) {
        m_contact_group.reset(new contact_group::ContactGroupRoutes(m_endpoint));
    }
    return m_contact_group.get();
};


googleQt::gcontact::GcontactCacheRoutes* GcontactRoutes::cacheRoutes() 
{
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
    m_CacheRoutes->runAutotest();
};
#endif
