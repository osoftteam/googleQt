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

