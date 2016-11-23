/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasks"
 www.prokarpaty.net
***********************************************************/

#include "gtask/tasks/TasksRoutes.h"
#include "Endpoint.h"
#include "gtask/GtaskRoutes.h"
using namespace googleQt;
using namespace tasks;

void TasksRoutes::clear(const gtask::TaskListClearArg& arg ){
    VOID_RESULT_GBC(clear_Async, arg);
}

void TasksRoutes::clear_Async(
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
    VOID_RESULT_GBC(deleteOperation_Async, arg);
}

void TasksRoutes::deleteOperation_Async(
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
    GOOGLE_BLOCKING_CALL(get_Async, TaskResource, arg);
}

void TasksRoutes::get_Async(
    const gtask::TaskIdArg& arg,
    std::function<void(std::unique_ptr<TaskResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<TaskResource>,
        TaskResource::factory
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskResource> TasksRoutes::insert(const gtask::TaskListIdArg& arg, const TaskResource& body){
    BODY_ARG_GBC(insert_Async, TaskResource, arg, body);
}

void TasksRoutes::insert_Async(
    const gtask::TaskListIdArg& arg,
    const TaskResource& body,
    std::function<void(std::unique_ptr<TaskResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        std::unique_ptr<TaskResource>,
        TaskResource::factory,
        TaskResource
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        body,
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskCollectionRes> TasksRoutes::list(const gtask::TaskListArg& arg){
    GOOGLE_BLOCKING_CALL(list_Async, TaskCollectionRes, arg);
}

void TasksRoutes::list_Async(
    const gtask::TaskListArg& arg,
    std::function<void(std::unique_ptr<TaskCollectionRes>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<TaskCollectionRes>,
        TaskCollectionRes::factory
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskResource> TasksRoutes::move(const gtask::TaskMoveArg& arg){
    GOOGLE_BLOCKING_CALL(move_Async, TaskResource, arg);
}

void TasksRoutes::move_Async(
    const gtask::TaskMoveArg& arg,
    std::function<void(std::unique_ptr<TaskResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        std::unique_ptr<TaskResource>,
        TaskResource::factory
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskResource> TasksRoutes::update(const gtask::TaskIdArg& arg, const TaskResource& body){
    BODY_ARG_GBC(update_Async, TaskResource, arg, body);
}

void TasksRoutes::update_Async(
    const gtask::TaskIdArg& arg,
    const TaskResource& body,
    std::function<void(std::unique_ptr<TaskResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->putStyle
        <
        std::unique_ptr<TaskResource>,
        TaskResource::factory,
        TaskResource
        >
        (m_end_point->buildGtaskUrl("tasks", arg),
        body,
        completed_callback,
        failed_callback);
}

