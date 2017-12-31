#include "GcontactRoutes.h"
using namespace googleQt;

GcontactRoutes::GcontactRoutes(Endpoint* e):m_endpoint(e)
{

};

contacts::ContactsRoutes* GcontactRoutes::getContacts()
{
  if(!m_Contacts){
    m_Contacts.reset(new contacts::ContactsRoutes(m_endpoint));
  }
  return m_Contacts.get();
};
