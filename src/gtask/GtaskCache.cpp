#include "Endpoint.h"
#include "GtaskCache.h"
#include "GtaskRoutes.h"
#include "gtask/GtaskRequestArg.h"

using namespace googleQt;
using namespace gtask_cache;
using namespace gtask;

/**
    TaskListReceiver
*/
TaskListReceiver::TaskListReceiver(GtaskRoutes& r) :m_r(r)
{
};

GoogleTask<tasklists::TaskListResource>* TaskListReceiver::routeRequest(QString tasklist_id)
{
    TaskListContainerIdArg arg(tasklist_id);
    return m_r.getTasklists()->get_Async(arg);
};

/**
    TaskReceiver
*/
TaskReceiver::TaskReceiver(GtaskRoutes& r) :m_r(r)
{
};

GoogleTask<tasks::TaskCollectionRes>* TaskReceiver::routeRequest(QString tasklist_id)
{
    TaskListArg arg(tasklist_id);
    return m_r.getTasks()->list_Async(arg);
};


TaskList::TaskList(tasklists::TaskListResource* r) 
{
    setFromResource(r);
};

void TaskList::setFromResource(tasklists::TaskListResource* r) 
{
    m_id = r->id();
    m_etag = r->etag();
    m_title = r->title();
    m_updated = r->updated();
};


/**
    TaskCache
*/
TaskCache::TaskCache(ApiEndpoint& e) :m_endpoint(e)
{
    
};

void TaskCache::updateTaskList(tasklists::TaskListResource* r)
{
    auto i = m_id2tlist.find(r->id());
    if (i != m_id2tlist.end()) {
        i->second->setFromResource(r);
    }
    else {
        TaskList::ptr p(new TaskList(r));
        m_id2tlist[r->id()] = std::move(p);
    }
};

/**
   GtaskCacheRoutes
*/
GtaskCacheRoutes::GtaskCacheRoutes(GtaskRoutes& gtask_routes):m_t_routes(gtask_routes)
{
    m_cache.reset(new TaskCache(*(m_t_routes.endpoint())));
};

GoogleVoidTask* GtaskCacheRoutes::loadTaskLists(const TIDLIST& idlist)
{
    GoogleVoidTask* rv = m_t_routes.endpoint()->produceVoidTask();

    auto par_runner = getTaskList_Async(idlist);
    connect(par_runner, &EndpointRunnable::finished, [=]()
    {
        RESULT_LIST<tasklists::TaskListResource>&& res = par_runner->detachResult();
        for (auto& m : res)
        {
            m_cache->updateTaskList(m.get());
        }
        par_runner->disposeLater();
        rv->completed_callback();
    });

    return rv;
};

ConcurrentValueRunner<QString, TaskListReceiver, tasklists::TaskListResource>* GtaskCacheRoutes::getTaskList_Async(const TIDLIST& idlist) 
{
    std::unique_ptr<TaskListReceiver> mr(new TaskListReceiver(m_t_routes));

    ConcurrentValueRunner<QString,
        TaskListReceiver,
        tasklists::TaskListResource>* r = new ConcurrentValueRunner<QString,
        TaskListReceiver,
        tasklists::TaskListResource>(idlist, std::move(mr), *(m_t_routes.endpoint()));
    r->run();
    return r;
};