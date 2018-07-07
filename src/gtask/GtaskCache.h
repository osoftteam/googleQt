#pragma once

#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"
#include "google/endpoint/BatchRunner.h"

#include "gtask/tasks/TasksRoutes.h"
#include "gtask/tasklists/TasklistsRoutes.h"

namespace googleQt
{
    class Endpoint;
    class GtaskRoutes;

    namespace gtask_cache {
        class TaskCache;
        class GtaskCacheRoutes;

        class TaskInfo
        {
        public:
            using ptr  = std::shared_ptr<TaskInfo>;
            //            using cptr  = std::shared_ptr<const TaskInfo>;
            using tarr = std::vector<ptr>;

            QString etag()const { return m_etag; }
            QString id()const { return m_id; }
            QString title()const { return m_title; }
            QString content()const { return m_content; }
            const QDateTime& updated()const { return m_updated; }            

            tarr&   items(){return m_items;}

        protected:
            QString             m_etag, m_id, m_title, m_content;
            QDateTime           m_updated;
            tarr                m_items;
        };

        class TaskList
        {
        public:
            class RootTaskInfo: public TaskInfo
            {
            public:
                using ptr  = std::shared_ptr<RootTaskInfo>;
            };

            using ptr  = std::shared_ptr<TaskList>;

            QString etag()const { return m_etag; }
            QString id()const { return m_id; }
            QString title()const { return m_title; }
            const QDateTime& updated()const { return m_updated; }

        protected:
            TaskList(tasklists::TaskListResource* r);
            void setFromResource(tasklists::TaskListResource* r);

        protected:
            QString             m_etag, m_id, m_title;
            QDateTime           m_updated;
            RootTaskInfo::ptr   m_root_task;

            friend class TaskCache;
        };
        
        class TaskCache
        {
        public:
            using ptr = std::shared_ptr<TaskCache>;
            using ID2TLIST = std::map<QString, TaskList::ptr>;
       
            ID2TLIST& all_task_lists(){return m_id2tlist;}

        protected:
            TaskCache(ApiEndpoint& e);
            void updateTaskList(tasklists::TaskListResource*);

        protected:
            ID2TLIST m_id2tlist;
            ApiEndpoint& m_endpoint;
            friend class GtaskCacheRoutes;
        };

        /**
            we know how to query for one task list, we can be inside concurrrent ValueBatchRunner
        */
        class TaskListReceiver
        {
        public:
            TaskListReceiver(GtaskRoutes& r);
            GoogleTask<tasklists::TaskListResource>* routeRequest(QString tasklist_id);
        protected:
            GtaskRoutes&        m_r;            
        };

        class TaskReceiver
        {
        public:
            TaskReceiver(GtaskRoutes& r);
            GoogleTask<tasks::TaskCollectionRes>* routeRequest(QString tasklist_id);
        protected:
            GtaskRoutes&        m_r;
        };


        class GtaskCacheRoutes : public QObject
        {
            Q_OBJECT
        public:
            using TIDLIST = std::list<QString>;

            GtaskCacheRoutes(GtaskRoutes& gtask_routes);

            GoogleVoidTask* loadTaskLists(const TIDLIST& idlist);

        protected:
            ConcurrentValueRunner<QString, TaskListReceiver, tasklists::TaskListResource>* getTaskList_Async(const TIDLIST& idlist);

        protected:
            GtaskRoutes&    m_t_routes;
            TaskCache::ptr  m_cache;
        };
        
    };

};
