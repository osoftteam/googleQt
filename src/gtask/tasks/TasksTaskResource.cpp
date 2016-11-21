/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasks"
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
std::unique_ptr<TaskResource> TaskResource::EXAMPLE(){
    std::unique_ptr<TaskResource> rv(new TaskResource);
    rv->m_id = "test1value";
    rv->m_title = "test2value";
    rv->m_kind = "test3value";
    rv->m_etag = "test4value";
    rv->m_updated = QDateTime::currentDateTime();
    rv->m_selfLink = "test6value";
    rv->m_parent = "test7value";
    rv->m_position = "test8value";
    rv->m_notes = "test9value";
    rv->m_status = "test10value";
    rv->m_due = QDateTime::currentDateTime();
    rv->m_completed = QDateTime::currentDateTime();
    return rv;
}
#endif //API_QT_AUTOTEST

}//tasks
}//googleQt
