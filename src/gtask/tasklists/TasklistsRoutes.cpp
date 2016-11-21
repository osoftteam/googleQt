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
    m_end_point->deleteStyle<std::unique_ptr<VoidType>, VoidType, NotAnException>(m_end_point->buildGtasklistUrl("tasklists", arg) );
}

std::unique_ptr<TaskListResource> TasklistsRoutes::get(const gtask::TaskListContainerIdArg& arg){
    return m_end_point->getStyle<std::unique_ptr<TaskListResource>, TaskListResource::factory, NotAnException>(m_end_point->buildGtasklistUrl("tasklists", arg));
}

std::unique_ptr<TaskListResource> TasklistsRoutes::insert(const TaskListResource& body){
    return m_end_point->postStyle<std::unique_ptr<TaskListResource>, TaskListResource::factory, TaskListResource >(m_end_point->buildGtasklistUrl("tasklists", VoidType::instance()), body);
}

std::unique_ptr<TaskListCollectionRes> TasklistsRoutes::list(const gtask::TasklistsListArg& arg){
    return m_end_point->getStyle<std::unique_ptr<TaskListCollectionRes>, TaskListCollectionRes::factory, NotAnException>(m_end_point->buildGtasklistUrl("tasklists", arg));
}

std::unique_ptr<TaskListResource> TasklistsRoutes::update(const gtask::TaskListContainerIdArg& arg, const TaskListResource& body){
    return m_end_point->putStyle<std::unique_ptr<TaskListResource>, TaskListResource::factory, TaskListResource>(m_end_point->buildGtasklistUrl("tasklists", arg), body);
}

