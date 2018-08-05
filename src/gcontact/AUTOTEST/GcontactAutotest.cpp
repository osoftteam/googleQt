/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification 
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "GcontactAutotest.h"
#ifdef API_QT_AUTOTEST
#include <QBuffer>
#include <QByteArray>
using namespace googleQt;

static GcontactRoutes* cl;


static void call_batch_from_ContactGroup(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("ContactGroup").arg("batch");
    std::unique_ptr<gcontact::BatchGroupArg> arg = gcontact::BatchGroupArg::EXAMPLE(0, 0);
    auto res = cl->getContactGroup()->batch(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_create_from_ContactGroup(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("ContactGroup").arg("create");
    std::unique_ptr<gcontact::CreateContactGroupArg> arg = gcontact::CreateContactGroupArg::EXAMPLE(0, 0);
    auto res = cl->getContactGroup()->create(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_deleteContactGroup_from_ContactGroup(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("ContactGroup").arg("deleteContactGroup");
    std::unique_ptr<gcontact::DeleteContactGroupArg> arg = gcontact::DeleteContactGroupArg::EXAMPLE(0, 0);
    cl->getContactGroup()->deleteContactGroup(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_ContactGroup(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("ContactGroup").arg("list");
    std::unique_ptr<gcontact::ContactGroupListArg> arg = gcontact::ContactGroupListArg::EXAMPLE(0, 0);
    auto res = cl->getContactGroup()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_update_from_ContactGroup(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("ContactGroup").arg("update");
    std::unique_ptr<gcontact::UpdateContactGroupArg> arg = gcontact::UpdateContactGroupArg::EXAMPLE(0, 0);
    auto res = cl->getContactGroup()->update(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_batch_from_Contacts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Contacts").arg("batch");
    std::unique_ptr<gcontact::BatchContactArg> arg = gcontact::BatchContactArg::EXAMPLE(0, 0);
    auto res = cl->getContacts()->batch(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_create_from_Contacts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Contacts").arg("create");
    std::unique_ptr<gcontact::CreateContactArg> arg = gcontact::CreateContactArg::EXAMPLE(0, 0);
    auto res = cl->getContacts()->create(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_deleteContact_from_Contacts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Contacts").arg("deleteContact");
    std::unique_ptr<gcontact::DeleteContactArg> arg = gcontact::DeleteContactArg::EXAMPLE(0, 0);
    cl->getContacts()->deleteContact(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_deleteContactPhoto_from_Contacts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Contacts").arg("deleteContactPhoto");
    std::unique_ptr<gcontact::DeletePhotoArg> arg = gcontact::DeletePhotoArg::EXAMPLE(0, 0);
    cl->getContacts()->deleteContactPhoto(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_getContactPhoto_from_Contacts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Contacts").arg("getContactPhoto");
    std::unique_ptr<gcontact::DownloadPhotoArg> arg = gcontact::DownloadPhotoArg::EXAMPLE(0, 0);
    QByteArray data("Hello World! 123454321 (.) :: (b -> c) -> (a -> b) -> (a -> c)");
    QBuffer io(&data);
    io.open(QIODevice::ReadOnly);
    cl->getContacts()->getContactPhoto(*(arg.get()) , &io);
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_Contacts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Contacts").arg("list");
    std::unique_ptr<gcontact::ContactListArg> arg = gcontact::ContactListArg::EXAMPLE(0, 0);
    auto res = cl->getContacts()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_update_from_Contacts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Contacts").arg("update");
    std::unique_ptr<gcontact::UpdateContactArg> arg = gcontact::UpdateContactArg::EXAMPLE(0, 0);
    auto res = cl->getContacts()->update(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_uploadContactPhoto_from_Contacts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Contacts").arg("uploadContactPhoto");
    std::unique_ptr<gcontact::UploadPhotoArg> arg = gcontact::UploadPhotoArg::EXAMPLE(0, 0);
    QByteArray data("Hello World! 123454321 (.) :: (b -> c) -> (a -> b) -> (a -> c)");
    QBuffer io(&data);
    io.open(QIODevice::ReadOnly);
    cl->getContacts()->uploadContactPhoto(*(arg.get()) , &io);
    ApiAutotest::INSTANCE() << "--------------------------";
}


static void test_call_ContactGroupRoutes(){
    call_batch_from_ContactGroup();
    call_create_from_ContactGroup();
    call_deleteContactGroup_from_ContactGroup();
    call_list_from_ContactGroup();
    call_update_from_ContactGroup();
}

static void test_call_ContactsRoutes(){
    call_batch_from_Contacts();
    call_create_from_Contacts();
    call_deleteContact_from_Contacts();
    call_deleteContactPhoto_from_Contacts();
    call_getContactPhoto_from_Contacts();
    call_list_from_Contacts();
    call_update_from_Contacts();
    call_uploadContactPhoto_from_Contacts();
}


void GcontactAutotest::generateCalls(){
    ApiAutotest::INSTANCE() << "";
    ApiAutotest::INSTANCE() << "============ autotest for module: gcontact ============";
    cl->autotest();
    test_call_ContactGroupRoutes();
    test_call_ContactsRoutes();
}

GcontactAutotest::GcontactAutotest(GoogleClient& c)
{
    cl = c.gcontact();
}
#endif //API_QT_AUTOTEST

