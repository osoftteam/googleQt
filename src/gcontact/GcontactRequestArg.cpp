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
    b.add("alt", "json");
};

#ifdef API_QT_AUTOTEST
std::unique_ptr<ContactsListArg> ContactsListArg::EXAMPLE(int, int)
{
    std::unique_ptr<ContactsListArg> rv(new ContactsListArg);
    return rv;
};
#endif