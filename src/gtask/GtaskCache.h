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
        class TaskList;
        class TaskCache;
        class GtaskCacheRoutes;
        class TaskListReceiver;
        class TaskReceiver;
        
        class TaskInfo
        {
        public:
            using ptr  = std::shared_ptr<TaskInfo>;
            using tarr = std::vector<ptr>;

            QString etag()const   { return m_etag; }
            QString id()const     { return m_id; }
            QString title()const  { return m_title; }
            QString notes()const  { return m_notes; }
            QString parent()const { return m_parent;}
            QString position()const{return m_position;}
            const QDateTime& updated()const { return m_updated; }            

            tarr&   items(){return m_items;}

        protected:
            TaskInfo();
            TaskInfo(const tasks::TaskResource* r);
            void setFromResource(const tasks::TaskResource* r);
            
            QString             m_etag, m_id, m_title, m_notes;
            QString             m_parent, m_position;
            QDateTime           m_updated;
            tarr                m_items;
            
            friend class TaskList;
        };

        class TaskList
        {
        public:
            class RootTaskInfo: public TaskInfo
            {
            public:
                using ptr  = std::shared_ptr<RootTaskInfo>;
            };

            using ptr   = std::shared_ptr<TaskList>;
            using ID2T  = std::map<QString, TaskInfo::ptr>;

            RootTaskInfo::ptr  root(){return m_root_task;}
            ID2T&              tasks_map(){return m_id2t;}
            const ID2T&        tasks_map()const{return m_id2t;}
            
            QString etag()const { return m_etag; }
            QString id()const { return m_id; }
            QString title()const { return m_title; }
            const QDateTime& updated()const { return m_updated; }

        protected:
            TaskList(const tasklists::TaskListResource* r);
            void setFromResource(const tasklists::TaskListResource* r);
            void setDetailsFromResource(const tasks::TaskCollectionRes* r);

        protected:
            QString             m_etag, m_id, m_title;
            QDateTime           m_updated;
            RootTaskInfo::ptr   m_root_task;
            ID2T                m_id2t;

            friend class TaskCache;
            friend class TaskReceiver;
        };

        
        class TaskCache
        {
        public:
            using ptr = std::shared_ptr<TaskCache>;
            using ID2TLIST = std::map<QString, TaskList::ptr>;
       
            ID2TLIST& task_lists(){return m_id2tlist;}

        protected:
            TaskCache(ApiEndpoint& e);
            void updateTaskList(tasklists::TaskListResource*);

        protected:
            ID2TLIST m_id2tlist;
            ApiEndpoint& m_endpoint;
            friend class GtaskCacheRoutes;
            friend class TaskListReceiver;
            friend class TaskReceiver;
        };

        /**
            we know how to query for one task list, we can be inside concurrrent ValueBatchRunner
        */
        class TaskListReceiver
        {
        public:
            TaskListReceiver(GtaskCacheRoutes& r);
            GoogleTask<tasklists::TaskListResource>* routeRequest(QString tasklist_id);
        protected:
            GtaskCacheRoutes&        m_r;            
        };

        class TaskReceiver
        {
        public:
            TaskReceiver(GtaskCacheRoutes& r);
            GoogleTask<tasks::TaskCollectionRes>* routeRequest(QString tasklist_id);
        protected:
            GtaskCacheRoutes&  m_r;
        };


        class GtaskCacheRoutes : public QObject
        {
            Q_OBJECT
        public:
            using TIDLIST = std::list<QString>;

            GtaskCacheRoutes(GtaskRoutes& gtask_routes);

            GtaskRoutes&   task_routes(){return m_t_routes;};
            TaskCache::ptr cache(){return m_cache;}
            
            GoogleVoidTask* loadTaskLists(const TIDLIST& idlist);

        protected:
            ConcurrentValueRunner<QString, TaskListReceiver, tasklists::TaskListResource>* getTaskList_Async(const TIDLIST& idlist);
            ConcurrentValueRunner<QString, TaskReceiver, tasks::TaskCollectionRes>* getTasks_Async(const TIDLIST& idlist);

        protected:
            GtaskRoutes&    m_t_routes;
            TaskCache::ptr  m_cache;
        };
        
    };

};
