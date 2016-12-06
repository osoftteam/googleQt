#include <iostream>
#include <functional>
#include <QBuffer>
#include <QFile>
#include <QFileInfo>
#include <iomanip>
#include "GtaskCommands.h"
#include "google/demo/ApiTerminal.h"
#include "gtask/GtaskRoutes.h"
#include "gtask/tasks/TasksRoutes.h"
#include "gtask/GtaskRequestArg.h"

using namespace googleQt;
using namespace gtask;
using namespace demo;

GtaskCommands::GtaskCommands(GoogleClient& c):m_c(c)
{
    m_gt = m_c.gtask();
};


void GtaskCommands::ls(QString tasklist_arg)
{
    if(tasklist_arg.isEmpty())
        {
            std::cout << "Missing parameters, expected <task_list_id>" << std::endl;     
            return;
        }

    bool showDeleted = false;
    bool showHidden = false;
    QString tasklist = "";
    if(tasklist_arg.indexOf(" ") != -1){
        //with parameters..
        QStringList arg_list = tasklist_arg.split(" ", QString::SkipEmptyParts);
        if (arg_list.size() > 0)            
                tasklist = arg_list[0];
        if (arg_list.size() > 1){
            if(arg_list[1] == "all"){
                showDeleted = true;
                showHidden = true;
            }
        }            
        
    }else{
        tasklist = tasklist_arg;
    }
    
    try
    {
        TaskListArg arg(tasklist);
        arg.setShowDeleted(showDeleted);
        arg.setShowHidden(showHidden);
        auto tasks_col = m_gt->getTasks()->list(arg);
        std::cout << tasklist << " [" << tasks_col->items().size() << "]" << std::endl;
        std::cout << Terminal::pad(QString(""), 80, '-') << std::endl;
        int idx = 1;
        for (auto t : tasks_col->items())
        {
            tasks::TaskResource& r = t;
            std::cout << idx++ << ". " << Terminal::pad_trunc(r.title(), 30)
                      << " " << Terminal::pad(r.id(), 50)
                      << std::endl;
        }
        QString nextToken = tasks_col->nextpagetoken();
        if (!nextToken.isEmpty()) {
            std::cout << "next-token: " << nextToken << std::endl;
        }
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GtaskCommands::get(QString tlistid_space_taskid) 
{
    QStringList arg_list = tlistid_space_taskid.split(" ", QString::SkipEmptyParts);
    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <task_list_id> <task_id>" << std::endl;
        return;
    }

    QString tasklist_id = arg_list[0];
    QString task_id = arg_list[1];

    try
    {
        TaskIdArg arg(tasklist_id, task_id);
        std::unique_ptr<tasks::TaskResource> t = m_gt->getTasks()->get(arg);
        printTask(t.get());
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GtaskCommands::insert(QString tlistid_space_title) 
{
    QStringList arg_list = tlistid_space_title.split(" ", QString::SkipEmptyParts);
    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <task_list_id> <title>" << std::endl;
        return;
    }

    QString tasklist_id = arg_list[0];
    QString title = arg_list[1];

    try
    {
        TaskListIdArg arg(tasklist_id);
        tasks::TaskResource new_task;
        new_task.setTitle(title).setNotes("created from gtaskbash");        

        std::unique_ptr<tasks::TaskResource> t = m_gt->getTasks()->insert(arg, new_task);
        printTask(t.get());
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GtaskCommands::update(QString tlistid_space_taskid_title)
{
    QStringList arg_list = tlistid_space_taskid_title.split(" ", QString::SkipEmptyParts);
    if (arg_list.size() < 3)
    {
        std::cout << "Invalid parameters, expected <task_list_id> <task_id> <title>" << std::endl;
        return;
    }

    QString tasklist_id = arg_list[0];
    QString task_id = arg_list[1];
    QString title = arg_list[2];

    try
    {
        TaskIdArg arg(tasklist_id, task_id);
        tasks::TaskResource new_task(task_id);
        new_task.setTitle(title);

        std::unique_ptr<tasks::TaskResource> t = m_gt->getTasks()->update(arg, new_task);
        printTask(t.get());
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GtaskCommands::delete_task(QString tlistid_space_taskid)
{
    QStringList arg_list = tlistid_space_taskid.split(" ", QString::SkipEmptyParts);
    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <task_list_id> <task_id>" << std::endl;
        return;
    }

    QString tasklist_id = arg_list[0];
    QString task_id = arg_list[1];

    try
    {
        TaskIdArg arg(tasklist_id, task_id);
        m_gt->getTasks()->deleteOperation(arg);
        std::cout << "task deleted" << std::endl;
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GtaskCommands::clearCompleted(QString tlistid) 
{
    try
    {
        TaskListClearArg arg(tlistid);
        m_gt->getTasks()->clear(arg);
        std::cout << "cleared" << std::endl;
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GtaskCommands::move(QString four_arguments)
{
    QStringList arg_list = four_arguments.split(" ", QString::SkipEmptyParts);
    if (arg_list.size() < 4)
    {
        std::cout << "Invalid parameters, expected <task_list_id> <task_id> <parent_id> <position_id>" << std::endl;
        return;
    }

    QString tasklist_id = arg_list[0];
    QString task_id = arg_list[1];
    QString parent_id = arg_list[2];
    QString position_id = arg_list[3];

    try
    {
        TaskMoveArg arg(tasklist_id, task_id);
        arg.setParent(parent_id);
        arg.setPrevious(position_id);

        std::unique_ptr<tasks::TaskResource> t = m_gt->getTasks()->move(arg);
        printTask(t.get());
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};


void GtaskCommands::ls_tlist(QString pageToken)
{
    try
    {
        TasklistsListArg arg(pageToken);

        auto task_lists_col = m_gt->getTasklists()->list(arg);
        int idx = 1;
        std::cout << "tasklists [" << task_lists_col->items().size() << "]" << std::endl;
        std::cout << Terminal::pad(QString(""), 80, '-') << std::endl;
        if(task_lists_col->items().size() > 0)
            {
                std::cout << "#" << " " << Terminal::pad_trunc(QString("title"), 30)
                          << " " << Terminal::pad(QString("id"), 50)
                          << std::endl;
            }
        std::cout << Terminal::pad(QString(""), 80, '-') << std::endl;

        for (auto t : task_lists_col->items())
        {
            tasklists::TaskListResource& r = t;
            std::cout << idx++ << ". " << Terminal::pad_trunc(r.title(), 30)
                      << " " << Terminal::pad(r.id(), 50)
                      << std::endl;
        }
        QString nextToken = task_lists_col->nextpagetoken();
        if (!nextToken.isEmpty()) {
            std::cout << "next-token: " << nextToken << std::endl;
        }
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GtaskCommands::get_tlist(QString tasklist_id) 
{
    try
    {
        TaskListContainerIdArg arg(tasklist_id);
        std::unique_ptr<tasklists::TaskListResource> t = m_gt->getTasklists()->get(arg);
        printTaskList(t.get());
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GtaskCommands::insert_tlist(QString title)
{
    try
    {
        tasklists::TaskListResource arg(title);
        std::unique_ptr<tasklists::TaskListResource> t = m_gt->getTasklists()->insert(arg);
        printTaskList(t.get());
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GtaskCommands::delete_tlist(QString tlistid)
{
    try
    {
        gtask::TaskListContainerIdArg arg(tlistid);
        m_gt->getTasklists()->deleteOperation(arg);
        std::cout << "task list deleted" << std::endl;
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GtaskCommands::update_tlist(QString tlistid_space_title) 
{
    QStringList arg_list = tlistid_space_title.split(" ", QString::SkipEmptyParts);
    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <task_list_id> <title>" << std::endl;
        return;
    }

    QString tasklist_id = arg_list[0];
    QString title = arg_list[1];

    try
    {
        TaskListContainerIdArg arg(tasklist_id);
        tasklists::TaskListResource new_task_list(tasklist_id);
        new_task_list.setTitle(title);

        std::unique_ptr<tasklists::TaskListResource> t = m_gt->getTasklists()->update(arg, new_task_list);
        printTaskList(t.get());
        m_c.printLastApiCall();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GtaskCommands::printTask(tasks::TaskResource* r) 
{
    std::cout << Terminal::pad("id", 15) << r->id() << std::endl
              << Terminal::pad("title", 15) << r->title() << std::endl
              << Terminal::pad("etag", 15) << r->etag() << std::endl
              << Terminal::pad("updated", 15) << r->updated().toString(Qt::SystemLocaleShortDate) << std::endl
              << Terminal::pad("selflink", 15) << r->selflink() << std::endl
              << Terminal::pad("parent", 15) << r->parent() << std::endl
              << Terminal::pad("notes", 15) << r->notes() << std::endl
              << Terminal::pad("due", 15) << r->due().toString() << std::endl
              << Terminal::pad("completed", 15) << r->completed().toString() << std::endl
              << Terminal::pad("deleted", 15) << (r->deleted() ? "true" : "false") << std::endl
              << Terminal::pad("completed", 15) << "hidden=" << (r->hidden() ? "true" : "false") << std::endl
              << std::endl;
};


void GtaskCommands::printTaskList(tasklists::TaskListResource* r)
{
    std::cout << Terminal::pad("id", 15) << r->id() << std::endl
              << Terminal::pad("title", 15) << r->title() << std::endl
              << Terminal::pad("etag", 15) << r->etag() << std::endl
              << Terminal::pad("updated", 15) << r->updated().toString(Qt::SystemLocaleShortDate) << std::endl
              << Terminal::pad("selflink", 15) << r->selflink() << std::endl;
};
