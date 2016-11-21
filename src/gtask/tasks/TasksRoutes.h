/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasks"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gtask/GtaskRequestArg.h"
#include "GoogleRouteBase.h"
#include "gtask/tasks/TasksTaskCollectionRes.h"
#include "gtask/tasks/TasksTaskResource.h"

namespace googleQt{
namespace tasks{


    class TasksRoutes: public GoogleRouteBase{
    public:
        TasksRoutes(Endpoint* ep):GoogleRouteBase(ep){};
            /**
            ApiRoute('clear')


            Deletes the specified task.

            */
        void clear(const gtask::TaskListClearArg& );

            /**
            ApiRoute('delete')


            Deletes the specified task.

            */
        void deleteOperation(const gtask::TaskIdArg& );

            /**
            ApiRoute('get')


            Returns the specified task.

            */
        std::unique_ptr<TaskResource> get(const gtask::TaskIdArg& arg);

            /**
            ApiRoute('insert')


            Returns the specified task.

            */
        std::unique_ptr<TaskResource> insert(const gtask::TaskListIdArg& arg, const TaskResource& body);

            /**
            ApiRoute('list')


            Returns all tasks in the specified task list

            */
        std::unique_ptr<TaskCollectionRes> list(const gtask::TaskListArg& arg);

            /**
            ApiRoute('move')


            Updates the specified task.

            */
        std::unique_ptr<TaskResource> move(const gtask::TaskMoveArg& arg);

            /**
            ApiRoute('update')


            Updates the specified task.

            */
        std::unique_ptr<TaskResource> update(const gtask::TaskIdArg& arg, const TaskResource& body);

    protected:
    };//TasksRoutes

}//tasks
}//googleQt
