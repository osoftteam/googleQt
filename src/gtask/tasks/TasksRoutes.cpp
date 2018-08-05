/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasks"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gtask/tasks/TasksRoutes.h"
#include "Endpoint.h"
#include "gtask/GtaskRoutes.h"
using namespace googleQt;
using namespace tasks;

void TasksRoutes::clear(const gtask::TaskListClearArg& arg ){
    clear_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* TasksRoutes::clear_Async(const gtask::TaskListClearArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->postStyle
        (m_end_point->buildGtaskUrl("tasks", arg),
        t);
    return t;
}

void TasksRoutes::clear_AsyncCB(
    const gtask::TaskListClearArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        (m_end_point->buildGtaskUrl("tasks", arg),
        completed_callback,
        failed_callback);
}

void TasksRoutes::deleteOperation(const gtask::TaskIdArg& arg ){
    deleteOperation_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* TasksRoutes::deleteOperation_Async(const gtask::TaskIdArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->deleteStyle
        (m_end_point->buildGtaskUrl("tasks", arg),
        t);
    return t;
}

void TasksRoutes::deleteOperation_AsyncCB(
    const gtask::TaskIdArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->deleteStyle
        (m_end_point->buildGtaskUrl("tasks", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskResource> TasksRoutes::get(const gtask::TaskIdArg& arg){
    return get_Async(arg)->waitForResultAndRelease();
}

GoogleTask<TaskResource>* TasksRoutes::get_Async(const gtask::TaskIdArg& arg)
{
    GoogleTask<TaskResource>* t = m_end_point->produceTask<TaskResource>();
    m_end_point->getStyle<
        TaskResource,
        TaskResource::factory
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        t);
    return t;
}

void TasksRoutes::get_AsyncCB(
    const gtask::TaskIdArg& arg,
    std::function<void(std::unique_ptr<TaskResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        TaskResource,
        TaskResource::factory
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskResource> TasksRoutes::insert(const gtask::TaskListIdArg& arg, const TaskResource& body){
    return insert_Async(arg, body)->waitForResultAndRelease();
}

GoogleTask<TaskResource>* TasksRoutes::insert_Async(const gtask::TaskListIdArg& arg, const TaskResource& body)
{
    GoogleTask<TaskResource>* t = m_end_point->produceTask<TaskResource>();
    m_end_point->postStyle<
        TaskResource,
        TaskResource::factory,
        TaskResource>
        (m_end_point->buildGtaskUrl("tasks", arg),
        body,
        t);
    return t;
}

void TasksRoutes::insert_AsyncCB(
    const gtask::TaskListIdArg& arg,
    const TaskResource& body,
    std::function<void(std::unique_ptr<TaskResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        TaskResource,
        TaskResource::factory,
        TaskResource
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        body,
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskCollectionRes> TasksRoutes::list(const gtask::TaskListArg& arg){
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<TaskCollectionRes>* TasksRoutes::list_Async(const gtask::TaskListArg& arg)
{
    GoogleTask<TaskCollectionRes>* t = m_end_point->produceTask<TaskCollectionRes>();
    m_end_point->getStyle<
        TaskCollectionRes,
        TaskCollectionRes::factory
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        t);
    return t;
}

void TasksRoutes::list_AsyncCB(
    const gtask::TaskListArg& arg,
    std::function<void(std::unique_ptr<TaskCollectionRes>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        TaskCollectionRes,
        TaskCollectionRes::factory
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskResource> TasksRoutes::move(const gtask::TaskMoveArg& arg){
    return move_Async(arg)->waitForResultAndRelease();
}

GoogleTask<TaskResource>* TasksRoutes::move_Async(const gtask::TaskMoveArg& arg)
{
    GoogleTask<TaskResource>* t = m_end_point->produceTask<TaskResource>();
    m_end_point->postStyle<
        TaskResource,
        TaskResource::factory
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        t);
    return t;
}

void TasksRoutes::move_AsyncCB(
    const gtask::TaskMoveArg& arg,
    std::function<void(std::unique_ptr<TaskResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        TaskResource,
        TaskResource::factory
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskResource> TasksRoutes::update(const gtask::TaskIdArg& arg, const TaskResource& body){
    return update_Async(arg, body)->waitForResultAndRelease();
}

GoogleTask<TaskResource>* TasksRoutes::update_Async(const gtask::TaskIdArg& arg, const TaskResource& body)
{
    GoogleTask<TaskResource>* t = m_end_point->produceTask<TaskResource>();
    m_end_point->putStyle<
        TaskResource,
        TaskResource::factory,
        TaskResource>
        (m_end_point->buildGtaskUrl("tasks", arg),
        body,
        t);
    return t;
}

void TasksRoutes::update_AsyncCB(
    const gtask::TaskIdArg& arg,
    const TaskResource& body,
    std::function<void(std::unique_ptr<TaskResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->putStyle
        <
        TaskResource,
        TaskResource::factory,
        TaskResource
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        body,
        completed_callback,
        failed_callback);
}

