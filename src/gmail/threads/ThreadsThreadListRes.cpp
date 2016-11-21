/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "threads"
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
    js["resultSizeEstimate"] = m_resultSizeEstimate;
}

void ThreadListRes::fromJson(const QJsonObject& js){

    jsonarray2struct_list(js["threads"].toArray(), m_threads);
    m_nextPageToken = js["nextPageToken"].toString();
    m_resultSizeEstimate = js["resultSizeEstimate"].toVariant().toInt();
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
std::unique_ptr<ThreadListRes> ThreadListRes::EXAMPLE(){
    std::unique_ptr<ThreadListRes> rv(new ThreadListRes);
    rv->m_nextPageToken = "test2value";
    rv->m_resultSizeEstimate = 3;
    return rv;
}
#endif //API_QT_AUTOTEST

}//threads
}//googleQt
