/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasks"
 www.prokarpaty.net
***********************************************************/

#include "gtask/tasks/TasksTaskCollectionRes.h"
using namespace googleQt;

namespace googleQt{

namespace tasks{
///TaskCollectionRes

TaskCollectionRes::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void TaskCollectionRes::toJson(QJsonObject& js)const{

    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    if(!m_etag.isEmpty())
        js["etag"] = QString(m_etag);
    if(!m_nextPageToken.isEmpty())
        js["nextPageToken"] = QString(m_nextPageToken);
    js["items"] = struct_list2jsonarray(m_items);
}

void TaskCollectionRes::fromJson(const QJsonObject& js){

    m_kind = js["kind"].toString();
    m_etag = js["etag"].toString();
    m_nextPageToken = js["nextPageToken"].toString();
    jsonarray2struct_list(js["items"].toArray(), m_items);
}

QString TaskCollectionRes::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<TaskCollectionRes>  TaskCollectionRes::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<TaskCollectionRes>  TaskCollectionRes::factory::create(const QJsonObject& js)
{
    std::unique_ptr<TaskCollectionRes> rv;
    rv = std::unique_ptr<TaskCollectionRes>(new TaskCollectionRes);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<TaskCollectionRes> TaskCollectionRes::EXAMPLE(){
    std::unique_ptr<TaskCollectionRes> rv(new TaskCollectionRes);
    rv->m_kind = "test1value";
    rv->m_etag = "test2value";
    rv->m_nextPageToken = "test3value";
    return rv;
}
#endif //API_QT_AUTOTEST

}//tasks
}//googleQt
