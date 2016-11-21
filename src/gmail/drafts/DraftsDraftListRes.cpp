/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "drafts"
 www.prokarpaty.net
***********************************************************/

#include "gmail/drafts/DraftsDraftListRes.h"
using namespace googleQt;

namespace googleQt{

namespace drafts{
///DraftListRes

DraftListRes::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void DraftListRes::toJson(QJsonObject& js)const{

    js["drafts"] = struct_list2jsonarray(m_drafts);
    if(!m_nextPageToken.isEmpty())
        js["nextPageToken"] = QString(m_nextPageToken);
    js["resultSizeEstimate"] = m_resultSizeEstimate;
}

void DraftListRes::fromJson(const QJsonObject& js){

    jsonarray2struct_list(js["drafts"].toArray(), m_drafts);
    m_nextPageToken = js["nextPageToken"].toString();
    m_resultSizeEstimate = js["resultSizeEstimate"].toVariant().toInt();
}

QString DraftListRes::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<DraftListRes>  DraftListRes::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<DraftListRes>  DraftListRes::factory::create(const QJsonObject& js)
{
    std::unique_ptr<DraftListRes> rv;
    rv = std::unique_ptr<DraftListRes>(new DraftListRes);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<DraftListRes> DraftListRes::EXAMPLE(){
    std::unique_ptr<DraftListRes> rv(new DraftListRes);
    rv->m_nextPageToken = "test2value";
    rv->m_resultSizeEstimate = 3;
    return rv;
}
#endif //API_QT_AUTOTEST

}//drafts
}//googleQt
