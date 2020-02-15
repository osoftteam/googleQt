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
TaskInfo::TaskInfo()
{

};

TaskListReceiver::TaskListReceiver(GtaskCacheRoutes& r) :m_r(r)
{
};

GoogleTask<tasklists::TaskListResource>* TaskListReceiver::routeRequest(QString tasklist_id)
{
    TaskListContainerIdArg arg(tasklist_id);
    auto t = m_r.task_routes().getTasklists()->get_Async(arg);
    if(t){
        QObject::connect(t, &GoogleTask<tasklists::TaskListResource>::finished, [=]()
            {
                if (t->isCompleted()) {
                    auto r = t->get();
                    if(r){
                        m_r.cache()->updateTaskList(r);
                    }
                };
            });        
    }
    return t;
};

/**
    TaskReceiver
*/
TaskReceiver::TaskReceiver(GtaskCacheRoutes& r)
    :m_r(r)
{
};

GoogleTask<tasks::TaskCollectionRes>* TaskReceiver::routeRequest(QString tasklist_id)
{
    TaskListArg arg(tasklist_id);
    auto t = m_r.task_routes().getTasks()->list_Async(arg);
    if(t){
        QObject::connect(t, &GoogleTask<tasks::TaskCollectionRes>::finished, [=]()
            {
                if (t->isCompleted()) {
                    auto r = t->get();
                    if(r){
                        auto i = m_r.cache()->task_lists().find(tasklist_id);
                        if(i != m_r.cache()->task_lists().end()){
                            i->second->setDetailsFromResource(r);
                            qDebug() << "TaskReceiver::routeRequest got tlist" << tasklist_id;
                        }
                    }
                }
            });
    }
    return t;
};

/**
   TaskInfo
*/
TaskInfo::TaskInfo(const tasks::TaskResource* r) 
{
    setFromResource(r);
};

void TaskInfo::setFromResource(const tasks::TaskResource* r) 
{
    m_id = r->id();
    m_etag = r->etag();
    m_title = r->title();
    m_notes = r->notes();
    m_updated = r->updated();
    m_parent = r->parent();
    m_position = r->position();
};


/**
   TaskList
*/
TaskList::TaskList(const tasklists::TaskListResource* r) 
{
    setFromResource(r);
};

void TaskList::setFromResource(const tasklists::TaskListResource* r) 
{
    m_id = r->id();
    m_etag = r->etag();
    m_title = r->title();
    m_updated = r->updated();
};

void TaskList::setDetailsFromResource(const tasks::TaskCollectionRes* c)
{
    m_id2t.clear();
    m_root_task.reset(new RootTaskInfo);
    
    for (auto& tr : c->items())
        {
            auto i = m_id2t.find(tr.id());
            if(i != m_id2t.end()){
                TaskInfo::ptr t = i->second;
                t->setFromResource(&tr);
            }
            else{
                TaskInfo::ptr t(new TaskInfo(&tr));
                m_id2t[tr.id()] = t;
            }
        }

    /// rebuild tree
    for(auto i : m_id2t){
        auto t = i.second;
        if(t->parent().isEmpty()){
            m_root_task->m_items.push_back(t);
        }
        else{
            auto j = m_id2t.find(t->parent());
            if(j != m_id2t.end()){
                auto t2 = j->second;
                t2->m_items.push_back(t);
            }
            else{
                qWarning() << "failed to locate parent task" << t->parent() << "for" << t->id();
            }
        }
    }
    
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

GoogleVoidTask* GtaskCacheRoutes::loadTaskLists(const STRING_LIST& idlist)
{
    GoogleVoidTask* rv = m_t_routes.endpoint()->produceVoidTask();

    auto par_runner = getTaskList_Async(idlist);
    if(par_runner){
        connect(par_runner, &EndpointRunnable::finished, [=]()
                {
                    par_runner->disposeLater();

                    auto par_details_runner = getTasks_Async(idlist);
                    if(par_details_runner){
                        connect(par_details_runner, &EndpointRunnable::finished, [=]()
                                {
                                    par_details_runner->disposeLater();
                                    rv->completed_callback();
                                });
                    }
                    else{
                        rv->completed_callback();
                    }
                });
    }
    else{
        rv->completed_callback();
    }

    return rv;
};

ConcurrentValueRunner<QString, TaskListReceiver, tasklists::TaskListResource>* GtaskCacheRoutes::getTaskList_Async(const STRING_LIST& idlist)
{
    if(idlist.empty()){
        return nullptr;
    }
    
    std::unique_ptr<TaskListReceiver> mr(new TaskListReceiver(*this));

    ConcurrentValueRunner<QString,
        TaskListReceiver,
        tasklists::TaskListResource>* r = new ConcurrentValueRunner<QString,
        TaskListReceiver,
        tasklists::TaskListResource>(idlist, std::move(mr), m_t_routes.endpoint()->apiClient());
    r->run();
    return r;
};

ConcurrentValueRunner<QString, TaskReceiver, tasks::TaskCollectionRes>* GtaskCacheRoutes::getTasks_Async(const STRING_LIST& idlist)
{
    if(idlist.empty()){
        return nullptr;
    }
    
    std::unique_ptr<TaskReceiver> mr(new TaskReceiver(*this));

    auto r = new ConcurrentValueRunner<QString,
                                       TaskReceiver,
                                       tasks::TaskCollectionRes>(idlist, std::move(mr), m_t_routes.endpoint()->apiClient());
    r->run();
    return r;
};

