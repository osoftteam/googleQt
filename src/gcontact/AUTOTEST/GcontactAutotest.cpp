/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification 
 www.prokarpaty.net
***********************************************************/

#include "GcontactAutotest.h"
#ifdef API_QT_AUTOTEST
#include <QBuffer>
#include <QByteArray>
using namespace googleQt;

static GcontactRoutes* cl;


static void call_create_from_Contacts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Contacts").arg("create");
    std::unique_ptr<gcontact::CreateContactArg> arg = gcontact::CreateContactArg::EXAMPLE(0, 0);
    auto res = cl->getContacts()->create(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_Contacts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Contacts").arg("list");
    std::unique_ptr<gcontact::ContactsListArg> arg = gcontact::ContactsListArg::EXAMPLE(0, 0);
    auto res = cl->getContacts()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}


static void test_call_ContactsRoutes(){
    call_create_from_Contacts();
    call_list_from_Contacts();
}


void GcontactAutotest::generateCalls(){
    ApiAutotest::INSTANCE() << "";
    ApiAutotest::INSTANCE() << "============ autotest for module: gcontact ============";
    cl->autotest();
    test_call_ContactsRoutes();
}

GcontactAutotest::GcontactAutotest(GoogleClient& c)
{
    cl = c.gcontact();
}
#endif //API_QT_AUTOTEST

