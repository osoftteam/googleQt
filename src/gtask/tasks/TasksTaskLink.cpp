/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasks"
 www.prokarpaty.net
***********************************************************/

#include "gtask/tasks/TasksTaskLink.h"
using namespace googleQt;

namespace googleQt{

namespace tasks{
///TaskLink

TaskLink::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void TaskLink::toJson(QJsonObject& js)const{

    if(!m_type.isEmpty())
        js["type"] = QString(m_type);
    if(!m_description.isEmpty())
        js["description"] = QString(m_description);
    if(!m_link.isEmpty())
        js["link"] = QString(m_link);
}

void TaskLink::fromJson(const QJsonObject& js){

    m_type = js["type"].toString();
    m_description = js["description"].toString();
    m_link = js["link"].toString();
}

QString TaskLink::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<TaskLink>  TaskLink::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<TaskLink>  TaskLink::factory::create(const QJsonObject& js)
{
    std::unique_ptr<TaskLink> rv;
    rv = std::unique_ptr<TaskLink>(new TaskLink);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<TaskLink> TaskLink::EXAMPLE(){
    std::unique_ptr<TaskLink> rv(new TaskLink);
    rv->m_type = "test1value";
    rv->m_description = "test2value";
    rv->m_link = "test3value";
    return rv;
}
#endif //API_QT_AUTOTEST

}//tasks
}//googleQt
