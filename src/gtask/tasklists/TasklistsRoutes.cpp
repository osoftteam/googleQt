/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasklists"
 www.prokarpaty.net
***********************************************************/

#include "gtask/tasklists/TasklistsRoutes.h"
#include "Endpoint.h"
#include "gtask/GtaskRoutes.h"
using namespace googleQt;
using namespace tasklists;

void TasklistsRoutes::deleteOperation(const gtask::TaskListContainerIdArg& arg ){
    VOID_RESULT_GBC(deleteOperation_Async, arg);
}

void TasklistsRoutes::deleteOperation_Async(
    const gtask::TaskListContainerIdArg& arg,
    std::function<void()> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->deleteStyle
        (m_end_point->buildGtasklistUrl("tasklists", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskListResource> TasklistsRoutes::get(const gtask::TaskListContainerIdArg& arg){
    GOOGLE_BLOCKING_CALL(get_Async, TaskListResource, arg);
}

void TasklistsRoutes::get_Async(
    const gtask::TaskListContainerIdArg& arg,
    std::function<void(std::unique_ptr<TaskListResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<TaskListResource>,
        TaskListResource::factory
        >
        (m_end_point->buildGtasklistUrl("tasklists", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskListResource> TasklistsRoutes::insert(const TaskListResource& body){
    BODY_NO_ARG_ARG_GBC(insert_Async, TaskListResource, body);
}

void TasklistsRoutes::insert_Async(
    const TaskListResource& body,
    std::function<void(std::unique_ptr<TaskListResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        std::unique_ptr<TaskListResource>,
        TaskListResource::factory,
        TaskListResource
        >
        (m_end_point->buildGtasklistUrl("tasklists", VoidType::instance()),
        body,
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskListCollectionRes> TasklistsRoutes::list(const gtask::TasklistsListArg& arg){
    GOOGLE_BLOCKING_CALL(list_Async, TaskListCollectionRes, arg);
}

void TasklistsRoutes::list_Async(
    const gtask::TasklistsListArg& arg,
    std::function<void(std::unique_ptr<TaskListCollectionRes>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<TaskListCollectionRes>,
        TaskListCollectionRes::factory
        >
        (m_end_point->buildGtasklistUrl("tasklists", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskListResource> TasklistsRoutes::update(const gtask::TaskListContainerIdArg& arg, const TaskListResource& body){
    BODY_ARG_GBC(update_Async, TaskListResource, arg, body);
}

void TasklistsRoutes::update_Async(
    const gtask::TaskListContainerIdArg& arg,
    const TaskListResource& body,
    std::function<void(std::unique_ptr<TaskListResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->putStyle
        <
        std::unique_ptr<TaskListResource>,
        TaskListResource::factory,
        TaskListResource
        >
        (m_end_point->buildGtasklistUrl("tasklists", arg),
        body,
        completed_callback,
        failed_callback);
}

