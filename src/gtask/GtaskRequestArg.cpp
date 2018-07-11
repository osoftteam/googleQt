#include <QUrlQuery>
#include "GtaskRequestArg.h"

using namespace googleQt;
using namespace gtask;

/**
    TaskListIdArg
*/
TaskListIdArg::TaskListIdArg(QString tasklist):m_tasklist(tasklist)
{

};

void TaskListIdArg::build(const QString& link_path, QUrl& url)const
{
    QString link = link_path + QString("/%1/tasks").arg(m_tasklist);
    url.setUrl(link);
};

/**
    TaskListContainerIdArg
*/
TaskListContainerIdArg::TaskListContainerIdArg(QString tasklist_id):TaskListIdArg(tasklist_id)
{

};

void TaskListContainerIdArg::build(const QString& link_path, QUrl& url)const
{
    QString link = link_path + QString("/%1").arg(m_tasklist);
    url.setUrl(link);
};

/**
    TaskListClearArg
*/
TaskListClearArg::TaskListClearArg(QString tasklist_id):TaskListIdArg(tasklist_id)
{

};

void TaskListClearArg::build(const QString& link_path, QUrl& url)const
{
    QString link = link_path + QString("/%1/clear").arg(m_tasklist);
    url.setUrl(link);
};

/**
    TaskIdArg
*/
TaskIdArg::TaskIdArg() 
{

};

TaskIdArg::TaskIdArg(QString task_list_id, QString task_id):TaskListIdArg(task_list_id), m_task(task_id)
{

};

void TaskIdArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/%1/tasks/%2").arg(m_tasklist).arg(m_task),
                 url);
};

/**
    TaskInsertArg
*/
TaskInsertArg::TaskInsertArg() 
{
}

void TaskInsertArg::build(const QString& link_path, QUrl& url)const
{
    TaskListIdArg::build(link_path, url);
    if (!m_parent.isEmpty() || !m_previous.isEmpty())
    {
        QUrlQuery uq;
        if (!m_parent.isEmpty())        
            uq.addQueryItem("parent", m_parent);    
        if (!m_previous.isEmpty())      
            uq.addQueryItem("previous", m_previous);        
        url.setQuery(uq);
    }
};

/**
    TaskMoveArg
*/
TaskMoveArg::TaskMoveArg()
{
}

TaskMoveArg::TaskMoveArg(QString task_list_id, QString task_id):TaskIdArg(task_list_id, task_id)
{

};

void TaskMoveArg::build(const QString& link_path, QUrl& url)const
{    
    UrlBuilder b(link_path + QString("/%1/tasks/%2/move").arg(m_tasklist).arg(m_task),
                 url);
    b.add("parent", m_parent).add("previous", m_previous);
    /*
    if (!m_parent.isEmpty() || !m_previous.isEmpty())
    {
        QUrlQuery uq;
        if (!m_parent.isEmpty())
            uq.addQueryItem("parent", m_parent);
        if (!m_previous.isEmpty())
            uq.addQueryItem("previous", m_previous);
        url.setQuery(uq);
        }*/
};

/**
    TaskListArg
*/
TaskListArg::TaskListArg(QString tasklist):TaskListIdArg(tasklist)
{
    m_maxResults = 100;
    m_showDeleted = m_showHidden = false;
};

void TaskListArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/%1/tasks").arg(m_tasklist), url);
    b.add("maxResults", m_maxResults)
        .add("showDeleted", m_showDeleted)
        .add("showHidden", m_showHidden)
        .add("updatedMin", m_updatedMin)
        .add("completedMax", m_completedMax)
        .add("completedMin", m_completedMin)
        .add("dueMax", m_dueMax)
        .add("dueMin", m_dueMin)
        .add("pageToken", m_pageToken);
};


TasklistsListArg::TasklistsListArg() :m_maxResults(100), m_pageToken("")
{

};

TasklistsListArg::TasklistsListArg(QString pageToken) :m_maxResults(100), m_pageToken(pageToken)
{

};

void TasklistsListArg::build(const QString& link_path, QUrl& url)const 
{
    url.setUrl(link_path);
    QUrlQuery uq;
    uq.addQueryItem("maxResults", QString("%1").arg(m_maxResults));
    if (!m_pageToken.isEmpty())
        uq.addQueryItem("pageToken", m_pageToken);
    url.setQuery(uq);
};

#ifdef API_QT_AUTOTEST
std::unique_ptr<TaskListIdArg> TaskListIdArg::EXAMPLE(int, int) 
{
    std::unique_ptr<TaskListIdArg> rv(new TaskListIdArg);
    rv->setTasklist("task-list1");
    return rv;
};

std::unique_ptr<TaskListContainerIdArg> TaskListContainerIdArg::EXAMPLE(int, int)
{
    std::unique_ptr<TaskListContainerIdArg> rv(new TaskListContainerIdArg);
    rv->setTasklist("task-list1");
    return rv;
};

std::unique_ptr<TaskIdArg> TaskIdArg::EXAMPLE(int, int)
{
    std::unique_ptr<TaskIdArg> rv(new TaskIdArg);
    rv->setTasklist("task-list1");
    rv->setTask("task1");
    return rv;
};

std::unique_ptr<TaskInsertArg> TaskInsertArg::EXAMPLE(int, int)
{
    std::unique_ptr<TaskInsertArg> rv(new TaskInsertArg);
    rv->setTasklist("task-list1");
    rv->setParent("parent-task2");
    rv->setPrevious("prev-task3");
    return rv;
};

std::unique_ptr<TaskMoveArg> TaskMoveArg::EXAMPLE(int, int)
{
    std::unique_ptr<TaskMoveArg> rv(new TaskMoveArg);
    rv->setTasklist("task-list1");
    rv->setTask("task1");
    rv->setParent("parent-task2");
    rv->setPrevious("prev-task3");
    return rv;
};


std::unique_ptr<TaskListClearArg> TaskListClearArg::EXAMPLE(int, int)
{
    std::unique_ptr<TaskListClearArg> rv(new TaskListClearArg);
    rv->setTasklist("task-list1");
    return rv;
};

std::unique_ptr<TaskListArg> TaskListArg::EXAMPLE(int, int)
{
    std::unique_ptr<TaskListArg> rv(new TaskListArg);
    rv->setTasklist("task-list1");
    rv->setMaxResults(20);
    rv->setPageToken("nextToken");
    rv->setCompletedMax("2016-09-08T10:47:31Z");
    rv->setCompletedMin("2016-09-08T10:47:31Z");
    rv->setDueMax("2016-09-08T10:47:31Z");
    rv->setDueMin("2016-09-08T10:47:31Z");
    return rv;
};

std::unique_ptr<TasklistsListArg> TasklistsListArg::EXAMPLE(int, int)
{
    std::unique_ptr<TasklistsListArg> rv(new TasklistsListArg);
    rv->setMaxResults(20);
    rv->setPageToken("nextToken");  
    return rv;
};
#endif
