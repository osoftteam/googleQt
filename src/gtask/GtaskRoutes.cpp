#include "GtaskRoutes.h"

using namespace googleQt;

GtaskRoutes::GtaskRoutes(Endpoint* e):m_endpoint(e)
{

};

tasklists::TasklistsRoutes* GtaskRoutes::getTasklists()
{
  if(!m_TaskLists){
    m_TaskLists.reset(new tasklists::TasklistsRoutes(m_endpoint));
  }
  return m_TaskLists.get();
};

tasks::TasksRoutes* GtaskRoutes::getTasks()
{
  if(!m_Tasks){
    m_Tasks.reset(new tasks::TasksRoutes(m_endpoint));
  }
  return m_Tasks.get();
};

googleQt::gtask_cache::GtaskCacheRoutes* GtaskRoutes::cacheRoutes()
{
    if(!m_CacheRoutes){
        m_CacheRoutes.reset(new gtask_cache::GtaskCacheRoutes(*this));
    }

    return m_CacheRoutes.get();
};
