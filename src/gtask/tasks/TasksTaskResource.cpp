/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasks"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gtask/tasks/TasksTaskResource.h"
using namespace googleQt;

namespace googleQt{

namespace tasks{
///TaskResource

TaskResource::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void TaskResource::toJson(QJsonObject& js)const{

    if(!m_id.isEmpty())
        js["id"] = QString(m_id);
    if(!m_title.isEmpty())
        js["title"] = QString(m_title);
    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    if(!m_etag.isEmpty())
        js["etag"] = QString(m_etag);
    if(m_updated.isValid())
        js["updated"] = m_updated.toString(Qt::ISODate);
    if(!m_selfLink.isEmpty())
        js["selfLink"] = QString(m_selfLink);
    if(!m_parent.isEmpty())
        js["parent"] = QString(m_parent);
    if(!m_position.isEmpty())
        js["position"] = QString(m_position);
    if(!m_notes.isEmpty())
        js["notes"] = QString(m_notes);
    if(!m_status.isEmpty())
        js["status"] = QString(m_status);
    if(m_due.isValid())
        js["due"] = m_due.toString(Qt::ISODate);
    if(m_completed.isValid())
        js["completed"] = m_completed.toString(Qt::ISODate);
    js["deleted"] = m_deleted;
    js["hidden"] = m_hidden;
    js["links"] = struct_list2jsonarray(m_links);
}

void TaskResource::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    m_title = js["title"].toString();
    m_kind = js["kind"].toString();
    m_etag = js["etag"].toString();
    m_updated = QDateTime::fromString(js["updated"].toString(), Qt::ISODate);
    m_selfLink = js["selfLink"].toString();
    m_parent = js["parent"].toString();
    m_position = js["position"].toString();
    m_notes = js["notes"].toString();
    m_status = js["status"].toString();
    m_due = QDateTime::fromString(js["due"].toString(), Qt::ISODate);
    m_completed = QDateTime::fromString(js["completed"].toString(), Qt::ISODate);
    m_deleted = js["deleted"].toVariant().toBool();
    m_hidden = js["hidden"].toVariant().toBool();
    jsonarray2struct_list(js["links"].toArray(), m_links);
}

QString TaskResource::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<TaskResource>  TaskResource::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<TaskResource>  TaskResource::factory::create(const QJsonObject& js)
{
    std::unique_ptr<TaskResource> rv;
    rv = std::unique_ptr<TaskResource>(new TaskResource);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<TaskResource> TaskResource::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<TaskResource> rv(new TaskResource);
    rv->m_id = ApiAutotest::INSTANCE().getId("tasks::TaskResource", example_idx);
    rv->m_title = ApiAutotest::INSTANCE().getString("tasks::TaskResource", "m_title", QString("title_%1").arg(example_idx));
    rv->m_kind = ApiAutotest::INSTANCE().getString("tasks::TaskResource", "m_kind", QString("kind_%1").arg(example_idx));
    rv->m_etag = ApiAutotest::INSTANCE().getString("tasks::TaskResource", "m_etag", QString("etag_%1").arg(example_idx));
    rv->m_updated = QDateTime::currentDateTime();
    rv->m_selfLink = ApiAutotest::INSTANCE().getString("tasks::TaskResource", "m_selfLink", QString("selfLink_%1").arg(example_idx));
    rv->m_parent = ApiAutotest::INSTANCE().getString("tasks::TaskResource", "m_parent", QString("parent_%1").arg(example_idx));
    rv->m_position = ApiAutotest::INSTANCE().getString("tasks::TaskResource", "m_position", QString("position_%1").arg(example_idx));
    rv->m_notes = ApiAutotest::INSTANCE().getString("tasks::TaskResource", "m_notes", QString("notes_%1").arg(example_idx));
    rv->m_status = ApiAutotest::INSTANCE().getString("tasks::TaskResource", "m_status", QString("status_%1").arg(example_idx));
    rv->m_due = QDateTime::currentDateTime();
    rv->m_completed = QDateTime::currentDateTime();
    std::vector<tasks::TaskLink> list_of_links;
    for(int i = 0; i < 5; i++){
        tasks::TaskLink p = *(tasks::TaskLink::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("tasks::TaskResource", "tasks::TaskLink", &p, i, context_index);
        rv->m_links.push_back(p);
    }
    return rv;
}
#endif //API_QT_AUTOTEST

}//tasks
}//googleQt
