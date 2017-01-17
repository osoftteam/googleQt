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
std::unique_ptr<TaskCollectionRes> TaskCollectionRes::EXAMPLE(int context_index){
    Q_UNUSED(context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<TaskCollectionRes> rv(new TaskCollectionRes);
    rv->m_kind = QString("kind_%1").arg(example_idx);
    rv->m_etag = QString("etag_%1").arg(example_idx);
    rv->m_nextPageToken = QString("nextPageToken_%1").arg(example_idx);
    std::list<tasks::TaskResource> list_of_items;
    for(int i = 0; i < 3; i++){
        tasks::TaskResource p = *(tasks::TaskResource::EXAMPLE(i).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("tasks::TaskCollectionRes", "tasks::TaskResource", &p, i, context_index);
        rv->m_items.push_back(p);
    }
    return rv;
}
#endif //API_QT_AUTOTEST

}//tasks
}//googleQt
