/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification 
 www.prokarpaty.net
***********************************************************/

#include "GmailAutotest.h"
#ifdef API_QT_AUTOTEST
#include <QBuffer>
#include <QByteArray>
using namespace googleQt;
using namespace googleQt;

static GmailRoutes* cl;


static void call_get_from_Drafts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Drafts").arg("get");
    std::unique_ptr<gmail::IdArg> arg = gmail::IdArg::EXAMPLE(0, 0);
    auto res = cl->getDrafts()->get(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_Drafts(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Drafts").arg("list");
    std::unique_ptr<gmail::DraftListArg> arg = gmail::DraftListArg::EXAMPLE(0, 0);
    auto res = cl->getDrafts()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_History(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("History").arg("list");
    std::unique_ptr<gmail::HistoryListArg> arg = gmail::HistoryListArg::EXAMPLE(0, 0);
    auto res = cl->getHistory()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_create_from_Labels(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Labels").arg("create");
    std::unique_ptr<labels::LabelResource> arg = labels::LabelResource::EXAMPLE(0, 0);
    auto res = cl->getLabels()->create(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_deleteOperation_from_Labels(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Labels").arg("deleteOperation");
    std::unique_ptr<gmail::IdArg> arg = gmail::IdArg::EXAMPLE(0, 0);
    cl->getLabels()->deleteOperation(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_get_from_Labels(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Labels").arg("get");
    std::unique_ptr<gmail::IdArg> arg = gmail::IdArg::EXAMPLE(0, 0);
    auto res = cl->getLabels()->get(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_Labels(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Labels").arg("list");
    cl->getLabels()->list();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_update_from_Labels(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Labels").arg("update");
    std::unique_ptr<gmail::IdArg> arg = gmail::IdArg::EXAMPLE(0, 0);
    std::unique_ptr<labels::LabelResource> arg2 = labels::LabelResource::EXAMPLE(0, 0);
    auto res = cl->getLabels()->update(*(arg.get()) , *(arg2.get()));
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_deleteOperation_from_Messages(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Messages").arg("deleteOperation");
    std::unique_ptr<gmail::IdArg> arg = gmail::IdArg::EXAMPLE(0, 0);
    cl->getMessages()->deleteOperation(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_get_from_Messages(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Messages").arg("get");
    std::unique_ptr<gmail::IdArg> arg = gmail::IdArg::EXAMPLE(0, 0);
    auto res = cl->getMessages()->get(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_importMessage_from_Messages(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Messages").arg("importMessage");
    std::unique_ptr<gmail::ImportMessageArg> arg = gmail::ImportMessageArg::EXAMPLE(0, 0);
    std::unique_ptr<messages::MessageResource> arg2 = messages::MessageResource::EXAMPLE(0, 0);
    auto res = cl->getMessages()->importMessage(*(arg.get()) , *(arg2.get()));
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_insert_from_Messages(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Messages").arg("insert");
    std::unique_ptr<gmail::InsertMessageArg> arg = gmail::InsertMessageArg::EXAMPLE(0, 0);
    std::unique_ptr<messages::MessageResource> arg2 = messages::MessageResource::EXAMPLE(0, 0);
    auto res = cl->getMessages()->insert(*(arg.get()) , *(arg2.get()));
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_Messages(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Messages").arg("list");
    std::unique_ptr<gmail::ListArg> arg = gmail::ListArg::EXAMPLE(0, 0);
    auto res = cl->getMessages()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_send_from_Messages(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Messages").arg("send");
    std::unique_ptr<gmail::SendMessageArg> arg = gmail::SendMessageArg::EXAMPLE(0, 0);
    std::unique_ptr<messages::MessageResource> arg2 = messages::MessageResource::EXAMPLE(0, 0);
    auto res = cl->getMessages()->send(*(arg.get()) , *(arg2.get()));
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_trash_from_Messages(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Messages").arg("trash");
    std::unique_ptr<gmail::TrashMessageArg> arg = gmail::TrashMessageArg::EXAMPLE(0, 0);
    cl->getMessages()->trash(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_untrash_from_Messages(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Messages").arg("untrash");
    std::unique_ptr<gmail::UntrashMessageArg> arg = gmail::UntrashMessageArg::EXAMPLE(0, 0);
    cl->getMessages()->untrash(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_get_from_Threads(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Threads").arg("get");
    std::unique_ptr<gmail::IdArg> arg = gmail::IdArg::EXAMPLE(0, 0);
    auto res = cl->getThreads()->get(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_Threads(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Threads").arg("list");
    std::unique_ptr<gmail::ListArg> arg = gmail::ListArg::EXAMPLE(0, 0);
    auto res = cl->getThreads()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_profile_from_Users(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Users").arg("profile");
    cl->getUsers()->profile();
    ApiAutotest::INSTANCE() << "--------------------------";
}


static void test_call_DraftsRoutes(){
    call_get_from_Drafts();
    call_list_from_Drafts();
}

static void test_call_HistoryRoutes(){
    call_list_from_History();
}

static void test_call_LabelsRoutes(){
    call_create_from_Labels();
    call_deleteOperation_from_Labels();
    call_get_from_Labels();
    call_list_from_Labels();
    call_update_from_Labels();
}

static void test_call_MessagesRoutes(){
    call_deleteOperation_from_Messages();
    call_get_from_Messages();
    call_importMessage_from_Messages();
    call_insert_from_Messages();
    call_list_from_Messages();
    call_send_from_Messages();
    call_trash_from_Messages();
    call_untrash_from_Messages();
}

static void test_call_ThreadsRoutes(){
    call_get_from_Threads();
    call_list_from_Threads();
}

static void test_call_UsersRoutes(){
    call_profile_from_Users();
}


void GmailAutotest::generateCalls(){
    ApiAutotest::INSTANCE() << "";
    ApiAutotest::INSTANCE() << "============ autotest for module: gmail ============";
    cl->autotest();
    test_call_DraftsRoutes();
    test_call_HistoryRoutes();
    test_call_LabelsRoutes();
    test_call_MessagesRoutes();
    test_call_ThreadsRoutes();
    test_call_UsersRoutes();
}

GmailAutotest::GmailAutotest(GoogleClient& c)
{
    cl = c.gmail();
}
#endif //API_QT_AUTOTEST

