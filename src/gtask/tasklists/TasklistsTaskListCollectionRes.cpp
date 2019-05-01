/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasklists"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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
std::unique_ptr<TaskListCollectionRes> TaskListCollectionRes::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<TaskListCollectionRes> rv(new TaskListCollectionRes);
    rv->m_kind = ApiAutotest::INSTANCE().getString("tasklists::TaskListCollectionRes", "m_kind", QString("kind_%1").arg(example_idx));
    rv->m_etag = ApiAutotest::INSTANCE().getString("tasklists::TaskListCollectionRes", "m_etag", QString("etag_%1").arg(example_idx));
    rv->m_nextPageToken = ApiAutotest::INSTANCE().getString("tasklists::TaskListCollectionRes", "m_nextPageToken", QString("nextPageToken_%1").arg(example_idx));
    std::vector<tasklists::TaskListResource> list_of_items;
    for(int i = 0; i < 5; i++){
        tasklists::TaskListResource p = *(tasklists::TaskListResource::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("tasklists::TaskListCollectionRes", "tasklists::TaskListResource", &p, i, context_index);
        rv->m_items.push_back(p);
    }
    return rv;
}
#endif //API_QT_AUTOTEST

}//tasklists
}//googleQt
