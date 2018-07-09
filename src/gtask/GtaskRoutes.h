#pragma once
#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"

#include "gtask/tasks/TasksRoutes.h"
#include "gtask/tasklists/TasklistsRoutes.h"
#include "GtaskCache.h"

namespace googleQt
{
    class Endpoint;

    namespace gtask_cache {
        class GtaskCacheRoutes;
    };
    
    class GtaskRoutes{  
    public:

        GtaskRoutes(Endpoint*);
        Endpoint* endpoint() { return m_endpoint; }

        tasklists::TasklistsRoutes* getTasklists();
        tasks::TasksRoutes* getTasks();
        googleQt::gtask_cache::GtaskCacheRoutes* cacheRoutes();

#ifdef API_QT_AUTOTEST
        void autotest() {};
#endif

    protected:
        Endpoint*  m_endpoint;
        std::unique_ptr<tasklists::TasklistsRoutes> m_TaskLists;
        std::unique_ptr<tasks::TasksRoutes> m_Tasks;
        std::unique_ptr<googleQt::gtask_cache::GtaskCacheRoutes>    m_CacheRoutes;
    };  
};//googleQt
