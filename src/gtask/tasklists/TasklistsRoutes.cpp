/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasklists"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gtask/tasklists/TasklistsRoutes.h"
#include "Endpoint.h"
#include "gtask/GtaskRoutes.h"
using namespace googleQt;
using namespace tasklists;

void TasklistsRoutes::deleteOperation(const gtask::TaskListContainerIdArg& arg ){
    deleteOperation_Async(arg)->waitForResultAndRelease();
}

GoogleVoidTask* TasklistsRoutes::deleteOperation_Async(const gtask::TaskListContainerIdArg& arg)
{
    GoogleVoidTask* t = m_end_point->produceVoidTask();
    m_end_point->deleteStyle
        (m_end_point->buildGtasklistUrl("tasklists", arg),
        t);
    return t;
}

void TasklistsRoutes::deleteOperation_AsyncCB(
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
    return get_Async(arg)->waitForResultAndRelease();
}

GoogleTask<TaskListResource>* TasklistsRoutes::get_Async(const gtask::TaskListContainerIdArg& arg)
{
    GoogleTask<TaskListResource>* t = m_end_point->produceTask<TaskListResource>();
    m_end_point->getStyle<
        TaskListResource,
        TaskListResource::factory
        >
        (m_end_point->buildGtasklistUrl("tasklists", arg),
        t);
    return t;
}

void TasklistsRoutes::get_AsyncCB(
    const gtask::TaskListContainerIdArg& arg,
    std::function<void(std::unique_ptr<TaskListResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        TaskListResource,
        TaskListResource::factory
        >
        (m_end_point->buildGtasklistUrl("tasklists", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskListResource> TasklistsRoutes::insert(const TaskListResource& body){
    return insert_Async(body)->waitForResultAndRelease();
}

GoogleTask<TaskListResource>* TasklistsRoutes::insert_Async(const TaskListResource& body)
{
    GoogleTask<TaskListResource>* t = m_end_point->produceTask<TaskListResource>();
    m_end_point->postStyle<
        TaskListResource,
        TaskListResource::factory,
        TaskListResource>
        (m_end_point->buildGtasklistUrl("tasklists", VoidType::instance()),
        body,
        t);
    return t;
}

void TasklistsRoutes::insert_AsyncCB(
    const TaskListResource& body,
    std::function<void(std::unique_ptr<TaskListResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->postStyle
        <
        TaskListResource,
        TaskListResource::factory,
        TaskListResource
        >
        (m_end_point->buildGtasklistUrl("tasklists", VoidType::instance()),
        body,
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskListCollectionRes> TasklistsRoutes::list(const gtask::TasklistsListArg& arg){
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<TaskListCollectionRes>* TasklistsRoutes::list_Async(const gtask::TasklistsListArg& arg)
{
    GoogleTask<TaskListCollectionRes>* t = m_end_point->produceTask<TaskListCollectionRes>();
    m_end_point->getStyle<
        TaskListCollectionRes,
        TaskListCollectionRes::factory
        >
        (m_end_point->buildGtasklistUrl("tasklists", arg),
        t);
    return t;
}

void TasklistsRoutes::list_AsyncCB(
    const gtask::TasklistsListArg& arg,
    std::function<void(std::unique_ptr<TaskListCollectionRes>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        TaskListCollectionRes,
        TaskListCollectionRes::factory
        >
        (m_end_point->buildGtasklistUrl("tasklists", arg),
        completed_callback,
        failed_callback);
}

std::unique_ptr<TaskListResource> TasklistsRoutes::update(const gtask::TaskListContainerIdArg& arg, const TaskListResource& body){
    return update_Async(arg, body)->waitForResultAndRelease();
}

GoogleTask<TaskListResource>* TasklistsRoutes::update_Async(const gtask::TaskListContainerIdArg& arg, const TaskListResource& body)
{
    GoogleTask<TaskListResource>* t = m_end_point->produceTask<TaskListResource>();
    m_end_point->putStyle<
        TaskListResource,
        TaskListResource::factory,
        TaskListResource>
        (m_end_point->buildGtasklistUrl("tasklists", arg),
        body,
        t);
    return t;
}

void TasklistsRoutes::update_AsyncCB(
    const gtask::TaskListContainerIdArg& arg,
    const TaskListResource& body,
    std::function<void(std::unique_ptr<TaskListResource>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->putStyle
        <
        TaskListResource,
        TaskListResource::factory,
        TaskListResource
        >
        (m_end_point->buildGtasklistUrl("tasklists", arg),
        body,
        completed_callback,
        failed_callback);
}

