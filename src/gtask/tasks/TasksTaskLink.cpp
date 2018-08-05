/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasks"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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
std::unique_ptr<TaskLink> TaskLink::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<TaskLink> rv(new TaskLink);
    rv->m_type = ApiAutotest::INSTANCE().getString("tasks::TaskLink", "m_type", QString("type_%1").arg(example_idx));
    rv->m_description = ApiAutotest::INSTANCE().getString("tasks::TaskLink", "m_description", QString("description_%1").arg(example_idx));
    rv->m_link = ApiAutotest::INSTANCE().getString("tasks::TaskLink", "m_link", QString("link_%1").arg(example_idx));
    return rv;
}
#endif //API_QT_AUTOTEST

}//tasks
}//googleQt
