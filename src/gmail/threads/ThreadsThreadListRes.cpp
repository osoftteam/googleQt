/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "threads"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/threads/ThreadsThreadListRes.h"
using namespace googleQt;

namespace googleQt{

namespace threads{
///ThreadListRes

ThreadListRes::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void ThreadListRes::toJson(QJsonObject& js)const{

    js["threads"] = struct_list2jsonarray(m_threads);
    if(!m_nextPageToken.isEmpty())
        js["nextPageToken"] = QString(m_nextPageToken);
    js["resultSizeEstimate"] = QString("%1").arg(m_resultSizeEstimate);
}

void ThreadListRes::fromJson(const QJsonObject& js){

    jsonarray2struct_list(js["threads"].toArray(), m_threads);
    m_nextPageToken = js["nextPageToken"].toString();
    m_resultSizeEstimate = js["resultSizeEstimate"].toVariant().toString().toULongLong();
}

QString ThreadListRes::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<ThreadListRes>  ThreadListRes::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<ThreadListRes>  ThreadListRes::factory::create(const QJsonObject& js)
{
    std::unique_ptr<ThreadListRes> rv;
    rv = std::unique_ptr<ThreadListRes>(new ThreadListRes);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<ThreadListRes> ThreadListRes::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<ThreadListRes> rv(new ThreadListRes);
    std::vector<threads::ThreadResource> list_of_threads;
    for(int i = 0; i < 5; i++){
        threads::ThreadResource p = *(threads::ThreadResource::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("threads::ThreadListRes", "threads::ThreadResource", &p, i, context_index);
        rv->m_threads.push_back(p);
    }
    rv->m_nextPageToken = ApiAutotest::INSTANCE().getString("threads::ThreadListRes", "m_nextPageToken", QString("nextPageToken_%1").arg(example_idx));
    rv->m_resultSizeEstimate = ApiAutotest::INSTANCE().getInt("threads::ThreadListRes", "m_resultSizeEstimate", 3 + example_idx);
    return rv;
}
#endif //API_QT_AUTOTEST

}//threads
}//googleQt
