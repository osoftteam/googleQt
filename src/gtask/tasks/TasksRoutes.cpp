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
    m_end_point->postStyle<std::unique_ptr<VoidType>, VoidType, NotAnException>(m_end_point->buildGtaskUrl("tasks", arg) );
}

void TasksRoutes::deleteOperation(const gtask::TaskIdArg& arg ){
    m_end_point->deleteStyle<std::unique_ptr<VoidType>, VoidType, NotAnException>(m_end_point->buildGtaskUrl("tasks", arg) );
}

std::unique_ptr<TaskResource> TasksRoutes::get(const gtask::TaskIdArg& arg){
    return m_end_point->getStyle<std::unique_ptr<TaskResource>, TaskResource::factory, NotAnException>(m_end_point->buildGtaskUrl("tasks", arg));
}

std::unique_ptr<TaskResource> TasksRoutes::insert(const gtask::TaskListIdArg& arg, const TaskResource& body){
    return m_end_point->postStyle<std::unique_ptr<TaskResource>, TaskResource::factory, TaskResource>(m_end_point->buildGtaskUrl("tasks", arg), body);
}

std::unique_ptr<TaskCollectionRes> TasksRoutes::list(const gtask::TaskListArg& arg){
    return m_end_point->getStyle<std::unique_ptr<TaskCollectionRes>, TaskCollectionRes::factory, NotAnException>(m_end_point->buildGtaskUrl("tasks", arg));
}

std::unique_ptr<TaskResource> TasksRoutes::move(const gtask::TaskMoveArg& arg){
    return m_end_point->postStyle<std::unique_ptr<TaskResource>, TaskResource::factory, NotAnException>(m_end_point->buildGtaskUrl("tasks", arg));
}

std::unique_ptr<TaskResource> TasksRoutes::update(const gtask::TaskIdArg& arg, const TaskResource& body){
    return m_end_point->putStyle<std::unique_ptr<TaskResource>, TaskResource::factory, TaskResource>(m_end_point->buildGtaskUrl("tasks", arg), body);
}

