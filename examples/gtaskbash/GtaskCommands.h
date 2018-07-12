#pragma once

#include "GoogleClient.h"
#include "gtask/GtaskCache.h"

using namespace googleQt;

namespace googleQt {
    namespace tasks
    {
        class TaskResource;
    };

    namespace tasklists {
        class TaskListResource;
    };
    /*
    namespace gtask_cache{
        class TaskInfo;
        class TaskInfo{
            class ptr;
        };
        };*/
}//googleQt

class GtaskCommands
{
public:
    GtaskCommands(GoogleClient& c);

    /**
    * ls - list tasks in a tasklist
    */
    void ls(QString tasklist);

    /**
    * get - get task by tasklist ID and task ID
    */
    void get(QString tlistid_space_taskid);

    /**
    * insert - insert new task into tasklist ID with title
    */
    void insert(QString tlistid_space_title);

    /**
    * update - update title of a task, requires tasklist ID, task ID
    */
    void update(QString tlistid_space_taskid_title);

    /**
    * update_note - update note of a task, requires tasklist ID, task ID
    */
    void update_note(QString tlistid_space_taskid_note);

    
    /**
    * deleteTask - delete task by task list ID and task ID
    */
    void delete_task(QString tlistid_space_taskid);

    /**
    * clearCompleted - hide all completed tasks in task list
    */
    void clearCompleted(QString tlistid);

    /**
    * move - move task, requires four space-separated arguments tasklistID, taskID, parentID, positionID
    */
    void move(QString four_arguments);

    /**
    * display list of tasklists
    */
    void ls_tlist(QString pageToken);

    /**
    * get tasklist by tasklist_id
    */
    void get_tlist(QString tasklist_id);

    /**
    * insert new tasklist with title
    */
    void insert_tlist(QString title);

    /**
    * update title of a tasklist ID, by tasklist ID
    */
    void update_tlist(QString tlistid_space_title);

    /**
    * deleteTask - delete tasklist by tasklistID
    */
    void delete_tlist(QString tlistid);

    void reload_cache(QString tlistids);

protected:
    void printTask(tasks::TaskResource*);
    void printTaskList(tasklists::TaskListResource*);
    void printCacheTitles();
    void printCacheParents();
    void printCacheTree();
    void printTaskInTree(googleQt::gtask_cache::TaskInfo::ptr p, int ident);
    
protected:
    GoogleClient& m_c;
    GtaskRoutes*  m_gt;
};
