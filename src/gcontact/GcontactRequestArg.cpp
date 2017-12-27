#include <QUrlQuery>
#include "GcontactRequestArg.h"

using namespace googleQt;
using namespace gcontact;

/**
   ContactsListArg
*/
ContactsListArg::ContactsListArg()
{

};

void ContactsListArg::build(const QString& link_path, QUrl& url)const{
    UrlBuilder b(link_path, url);
    if(!m_alt.isEmpty()){
        b.add("alt", m_alt);
    }
    if(!m_q.isEmpty()){
        b.add("q", m_q);
    }
    if(m_max_results > 0){
        b.add("max-results", m_max_results);
    }
};

#ifdef API_QT_AUTOTEST
std::unique_ptr<ContactsListArg> ContactsListArg::EXAMPLE(int, int)
{
    std::unique_ptr<ContactsListArg> rv(new ContactsListArg);
    return rv;
};
#endif
