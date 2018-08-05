/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification 
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "GtaskAutotest.h"
#ifdef API_QT_AUTOTEST
#include <QBuffer>
#include <QByteArray>
using namespace googleQt;

static GtaskRoutes* cl;


static void call_deleteOperation_from_Tasklists(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasklists").arg("deleteOperation");
    std::unique_ptr<gtask::TaskListContainerIdArg> arg = gtask::TaskListContainerIdArg::EXAMPLE(0, 0);
    cl->getTasklists()->deleteOperation(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_get_from_Tasklists(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasklists").arg("get");
    std::unique_ptr<gtask::TaskListContainerIdArg> arg = gtask::TaskListContainerIdArg::EXAMPLE(0, 0);
    auto res = cl->getTasklists()->get(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_insert_from_Tasklists(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasklists").arg("insert");
    std::unique_ptr<tasklists::TaskListResource> arg = tasklists::TaskListResource::EXAMPLE(0, 0);
    auto res = cl->getTasklists()->insert(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_Tasklists(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasklists").arg("list");
    std::unique_ptr<gtask::TasklistsListArg> arg = gtask::TasklistsListArg::EXAMPLE(0, 0);
    auto res = cl->getTasklists()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_update_from_Tasklists(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasklists").arg("update");
    std::unique_ptr<gtask::TaskListContainerIdArg> arg = gtask::TaskListContainerIdArg::EXAMPLE(0, 0);
    std::unique_ptr<tasklists::TaskListResource> arg2 = tasklists::TaskListResource::EXAMPLE(0, 0);
    auto res = cl->getTasklists()->update(*(arg.get()) , *(arg2.get()));
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_clear_from_Tasks(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasks").arg("clear");
    std::unique_ptr<gtask::TaskListClearArg> arg = gtask::TaskListClearArg::EXAMPLE(0, 0);
    cl->getTasks()->clear(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_deleteOperation_from_Tasks(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasks").arg("deleteOperation");
    std::unique_ptr<gtask::TaskIdArg> arg = gtask::TaskIdArg::EXAMPLE(0, 0);
    cl->getTasks()->deleteOperation(*(arg.get()) );
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_get_from_Tasks(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasks").arg("get");
    std::unique_ptr<gtask::TaskIdArg> arg = gtask::TaskIdArg::EXAMPLE(0, 0);
    auto res = cl->getTasks()->get(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_insert_from_Tasks(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasks").arg("insert");
    std::unique_ptr<gtask::TaskListIdArg> arg = gtask::TaskListIdArg::EXAMPLE(0, 0);
    std::unique_ptr<tasks::TaskResource> arg2 = tasks::TaskResource::EXAMPLE(0, 0);
    auto res = cl->getTasks()->insert(*(arg.get()) , *(arg2.get()));
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_list_from_Tasks(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasks").arg("list");
    std::unique_ptr<gtask::TaskListArg> arg = gtask::TaskListArg::EXAMPLE(0, 0);
    auto res = cl->getTasks()->list(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_move_from_Tasks(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasks").arg("move");
    std::unique_ptr<gtask::TaskMoveArg> arg = gtask::TaskMoveArg::EXAMPLE(0, 0);
    auto res = cl->getTasks()->move(*(arg.get()) );
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}

static void call_update_from_Tasks(){
    ApiAutotest::INSTANCE() << QString("%1/%2").arg("Tasks").arg("update");
    std::unique_ptr<gtask::TaskIdArg> arg = gtask::TaskIdArg::EXAMPLE(0, 0);
    std::unique_ptr<tasks::TaskResource> arg2 = tasks::TaskResource::EXAMPLE(0, 0);
    auto res = cl->getTasks()->update(*(arg.get()) , *(arg2.get()));
    ApiAutotest::INSTANCE() << "------ RESULT ------------------";
    ApiAutotest::INSTANCE() << res->toString();
    ApiAutotest::INSTANCE() << "--------------------------";
}


static void test_call_TasklistsRoutes(){
    call_deleteOperation_from_Tasklists();
    call_get_from_Tasklists();
    call_insert_from_Tasklists();
    call_list_from_Tasklists();
    call_update_from_Tasklists();
}

static void test_call_TasksRoutes(){
    call_clear_from_Tasks();
    call_deleteOperation_from_Tasks();
    call_get_from_Tasks();
    call_insert_from_Tasks();
    call_list_from_Tasks();
    call_move_from_Tasks();
    call_update_from_Tasks();
}


void GtaskAutotest::generateCalls(){
    ApiAutotest::INSTANCE() << "";
    ApiAutotest::INSTANCE() << "============ autotest for module: gtask ============";
    cl->autotest();
    test_call_TasklistsRoutes();
    test_call_TasksRoutes();
}

GtaskAutotest::GtaskAutotest(GoogleClient& c)
{
    cl = c.gtask();
}
#endif //API_QT_AUTOTEST

