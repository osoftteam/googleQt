/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasklists"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gtask/GtaskRequestArg.h"
#include "GoogleRouteBase.h"
#include "gtask/tasklists/TasklistsTaskListCollectionRes.h"
#include "gtask/tasklists/TasklistsTaskListResource.h"

namespace googleQt{
namespace tasklists{


    class TasklistsRoutes: public GoogleRouteBase{
    public:
        TasklistsRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('delete')


            Updates the authenticated user's specified task list

            */
        void deleteOperation(const gtask::TaskListContainerIdArg& );

            /**
            ApiRoute('get')


            Returns the specified tasklist.

            */
        std::unique_ptr<TaskListResource> get(const gtask::TaskListContainerIdArg& arg);

            /**
            ApiRoute('insert')


            Creates a new task list and adds it to the authenticated user's task
            lists

            */
        std::unique_ptr<TaskListResource> insert(const TaskListResource& body);

            /**
            ApiRoute('list')


            Returns all the authenticated user's task lists.

            */
        std::unique_ptr<TaskListCollectionRes> list(const gtask::TasklistsListArg& arg);

            /**
            ApiRoute('update')


            Updates the authenticated user's specified task list

            */
        std::unique_ptr<TaskListResource> update(const gtask::TaskListContainerIdArg& arg, const TaskListResource& body);

    protected:
    };//TasklistsRoutes

}//tasklists
}//googleQt
