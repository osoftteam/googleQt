/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasklists"
 www.prokarpaty.net
***********************************************************/

#include "gtask/tasklists/TasklistsTaskListCollectionRes.h"
using namespace googleQt;

namespace googleQt{

namespace tasklists{
///TaskListCollectionRes

TaskListCollectionRes::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void TaskListCollectionRes::toJson(QJsonObject& js)const{

    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    if(!m_etag.isEmpty())
        js["etag"] = QString(m_etag);
    if(!m_nextPageToken.isEmpty())
        js["nextPageToken"] = QString(m_nextPageToken);
    js["items"] = struct_list2jsonarray(m_items);
}

void TaskListCollectionRes::fromJson(const QJsonObject& js){

    m_kind = js["kind"].toString();
    m_etag = js["etag"].toString();
    m_nextPageToken = js["nextPageToken"].toString();
    jsonarray2struct_list(js["items"].toArray(), m_items);
}

QString TaskListCollectionRes::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<TaskListCollectionRes>  TaskListCollectionRes::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<TaskListCollectionRes>  TaskListCollectionRes::factory::create(const QJsonObject& js)
{
    std::unique_ptr<TaskListCollectionRes> rv;
    rv = std::unique_ptr<TaskListCollectionRes>(new TaskListCollectionRes);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<TaskListCollectionRes> TaskListCollectionRes::EXAMPLE(){
    std::unique_ptr<TaskListCollectionRes> rv(new TaskListCollectionRes);
    rv->m_kind = "test1value";
    rv->m_etag = "test2value";
    rv->m_nextPageToken = "test3value";
    return rv;
}
#endif //API_QT_AUTOTEST

}//tasklists
}//googleQt
