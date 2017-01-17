/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#include "gmail/messages/MessagesMessageResource.h"
using namespace googleQt;

namespace googleQt{

namespace messages{
///MessageResource

MessageResource::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void MessageResource::toJson(QJsonObject& js)const{

    if(!m_id.isEmpty())
        js["id"] = QString(m_id);
    if(!m_threadId.isEmpty())
        js["threadId"] = QString(m_threadId);
    js["labelIds"] = ingrl_list2jsonarray(m_labelIds);
    if(!m_snippet.isEmpty())
        js["snippet"] = QString(m_snippet);
    js["historyId"] = m_historyId;
    js["internalDate"] = m_internalDate;
    js["payload"] = (QJsonObject)m_payload;
    js["sizeEstimate"] = m_sizeEstimate;
    js["raw"] = m_raw.constData();
}

void MessageResource::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    m_threadId = js["threadId"].toString();
    jsonarray2ingrl_list(js["labelIds"].toArray(), m_labelIds);
    m_snippet = js["snippet"].toString();
    m_historyId = js["historyId"].toVariant().toInt();
    m_internalDate = js["internalDate"].toVariant().toInt();
    m_payload.fromJson(js["payload"].toObject());
    m_sizeEstimate = js["sizeEstimate"].toVariant().toInt();
    m_raw = js["raw"].toVariant().toByteArray();
}

QString MessageResource::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<MessageResource>  MessageResource::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<MessageResource>  MessageResource::factory::create(const QJsonObject& js)
{
    std::unique_ptr<MessageResource> rv;
    rv = std::unique_ptr<MessageResource>(new MessageResource);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<MessageResource> MessageResource::EXAMPLE(int context_index){
    Q_UNUSED(context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<MessageResource> rv(new MessageResource);
    rv->m_id = ApiAutotest::INSTANCE().getId("messages::MessageResource", example_idx);
    rv->m_threadId = QString("threadId_%1").arg(example_idx);
    std::list<QString> list_of_labelIds;
    for(int i = 0; i < 3; i++){
        rv->m_labelIds.push_back(QString("_%1_%2").arg(i).arg(example_idx));
    }
    rv->m_snippet = QString("snippet_%1").arg(example_idx);
    rv->m_historyId = 5;
    rv->m_internalDate = 6;
    rv->m_payload = *(messages::MessagePayload::EXAMPLE(0).get());
    rv->m_sizeEstimate = 8;
    rv->m_raw = QByteArray("AUTOTEST-DATA").toBase64();
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
