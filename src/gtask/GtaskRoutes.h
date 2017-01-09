#pragma once
#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"

#include "gtask/tasks/TasksRoutes.h"
#include "gtask/tasklists/TasklistsRoutes.h"

namespace googleQt
{
    class Endpoint;

    class GtaskRoutes{  
    public:

        GtaskRoutes(Endpoint*);
        Endpoint* endpoint() { return m_endpoint; }

        tasklists::TasklistsRoutes* getTasklists();
        tasks::TasksRoutes* getTasks();

#ifdef API_QT_AUTOTEST
        void autotest() {};
#endif

    protected:
        std::unique_ptr<tasklists::TasklistsRoutes> m_TaskLists;
        std::unique_ptr<tasks::TasksRoutes> m_Tasks;
        Endpoint*  m_endpoint;
    };  
};//googleQt
