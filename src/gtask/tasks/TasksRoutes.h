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
        void clear_Async(
            const gtask::TaskListClearArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('delete')


            Deletes the specified task.

            */
        void deleteOperation(const gtask::TaskIdArg& );
        void deleteOperation_Async(
            const gtask::TaskIdArg&,
            std::function<void()> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('get')


            Returns the specified task.

            */
        std::unique_ptr<TaskResource> get(const gtask::TaskIdArg& arg);
        void get_Async(
            const gtask::TaskIdArg&,
            std::function<void(std::unique_ptr<TaskResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('insert')


            Returns the specified task.

            */
        std::unique_ptr<TaskResource> insert(const gtask::TaskListIdArg& arg, const TaskResource& body);
        void insert_Async(
            const gtask::TaskListIdArg&,
            const TaskResource& body,
            std::function<void(std::unique_ptr<TaskResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('list')


            Returns all tasks in the specified task list

            */
        std::unique_ptr<TaskCollectionRes> list(const gtask::TaskListArg& arg);
        void list_Async(
            const gtask::TaskListArg&,
            std::function<void(std::unique_ptr<TaskCollectionRes>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('move')


            Updates the specified task.

            */
        std::unique_ptr<TaskResource> move(const gtask::TaskMoveArg& arg);
        void move_Async(
            const gtask::TaskMoveArg&,
            std::function<void(std::unique_ptr<TaskResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

            /**
            ApiRoute('update')


            Updates the specified task.

            */
        std::unique_ptr<TaskResource> update(const gtask::TaskIdArg& arg, const TaskResource& body);
        void update_Async(
            const gtask::TaskIdArg&,
            const TaskResource& body,
            std::function<void(std::unique_ptr<TaskResource>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//TasksRoutes

}//tasks
}//googleQt
