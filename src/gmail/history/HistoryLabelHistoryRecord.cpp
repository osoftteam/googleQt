/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "history"
 www.prokarpaty.net
***********************************************************/

#include "gmail/history/HistoryLabelHistoryRecord.h"
using namespace googleQt;

namespace googleQt{

namespace history{
///LabelHistoryRecord

LabelHistoryRecord::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void LabelHistoryRecord::toJson(QJsonObject& js)const{

    js["message"] = (QJsonObject)m_message;
    js["labelIds"] = ingrl_list2jsonarray(m_labelIds);
}

void LabelHistoryRecord::fromJson(const QJsonObject& js){

    m_message.fromJson(js["message"].toObject());
    jsonarray2ingrl_list(js["labelIds"].toArray(), m_labelIds);
}

QString LabelHistoryRecord::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<LabelHistoryRecord>  LabelHistoryRecord::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<LabelHistoryRecord>  LabelHistoryRecord::factory::create(const QJsonObject& js)
{
    std::unique_ptr<LabelHistoryRecord> rv;
    rv = std::unique_ptr<LabelHistoryRecord>(new LabelHistoryRecord);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<LabelHistoryRecord> LabelHistoryRecord::EXAMPLE(){
    std::unique_ptr<LabelHistoryRecord> rv(new LabelHistoryRecord);
    rv->m_message = *(messages::MessageResource::EXAMPLE().get());
    return rv;
}
#endif //API_QT_AUTOTEST

}//history
}//googleQt
