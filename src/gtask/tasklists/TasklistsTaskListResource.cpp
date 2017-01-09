/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasklists"
 www.prokarpaty.net
***********************************************************/

#include "gtask/tasklists/TasklistsTaskListResource.h"
using namespace googleQt;

namespace googleQt{

namespace tasklists{
///TaskListResource

TaskListResource::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void TaskListResource::toJson(QJsonObject& js)const{

    if(!m_id.isEmpty())
        js["id"] = QString(m_id);
    if(!m_title.isEmpty())
        js["title"] = QString(m_title);
    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    if(!m_etag.isEmpty())
        js["etag"] = QString(m_etag);
    if(!m_selfLink.isEmpty())
        js["selfLink"] = QString(m_selfLink);
    if(m_updated.isValid())
        js["updated"] = m_updated.toString(Qt::ISODate);
}

void TaskListResource::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    m_title = js["title"].toString();
    m_kind = js["kind"].toString();
    m_etag = js["etag"].toString();
    m_selfLink = js["selfLink"].toString();
    m_updated = QDateTime::fromString(js["updated"].toString(), Qt::ISODate);
}

QString TaskListResource::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<TaskListResource>  TaskListResource::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<TaskListResource>  TaskListResource::factory::create(const QJsonObject& js)
{
    std::unique_ptr<TaskListResource> rv;
    rv = std::unique_ptr<TaskListResource>(new TaskListResource);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<TaskListResource> TaskListResource::EXAMPLE(int context_index){
    Q_UNUSED(context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<TaskListResource> rv(new TaskListResource);
    rv->m_id = QString("test1value_%1").arg(example_idx);
    rv->m_title = QString("test2value_%1").arg(example_idx);
    rv->m_kind = QString("test3value_%1").arg(example_idx);
    rv->m_etag = QString("test4value_%1").arg(example_idx);
    rv->m_selfLink = QString("test5value_%1").arg(example_idx);
    rv->m_updated = QDateTime::currentDateTime();
    return rv;
}
#endif //API_QT_AUTOTEST

}//tasklists
}//googleQt
