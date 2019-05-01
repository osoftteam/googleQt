/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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
    js["historyId"] = QString("%1").arg(m_historyId);
    js["internalDate"] = QString("%1").arg(m_internalDate);
    js["payload"] = (QJsonObject)m_payload;
    js["sizeEstimate"] = QString("%1").arg(m_sizeEstimate);
    js["raw"] = m_raw.constData();
}

void MessageResource::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    m_threadId = js["threadId"].toString();
    jsonarray2ingrl_list(js["labelIds"].toArray(), m_labelIds);
    m_snippet = js["snippet"].toString();
    m_historyId = js["historyId"].toVariant().toString().toULongLong();
    m_internalDate = js["internalDate"].toVariant().toString().toULongLong();
    m_payload.fromJson(js["payload"].toObject());
    m_sizeEstimate = js["sizeEstimate"].toVariant().toString().toULongLong();
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
std::unique_ptr<MessageResource> MessageResource::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<MessageResource> rv(new MessageResource);
    rv->m_id = ApiAutotest::INSTANCE().getId("messages::MessageResource", example_idx);
    rv->m_threadId = ApiAutotest::INSTANCE().getString("messages::MessageResource", "m_threadId", QString("threadId_%1").arg(example_idx));
    std::vector<QString> list_of_labelIds;
    for(int i = 0; i < 5; i++){
        rv->m_labelIds.push_back(QString("id_%1").arg(i+1));
    }
    QString tmp_m_labelIds = ApiAutotest::INSTANCE().getString4List("messages::MessageResource", "m_labelIds");
    if(!tmp_m_labelIds.isEmpty())rv->m_labelIds.push_back(tmp_m_labelIds);
    rv->m_snippet = ApiAutotest::INSTANCE().getString("messages::MessageResource", "m_snippet", QString("snippet_%1").arg(example_idx));
    rv->m_historyId = ApiAutotest::INSTANCE().getInt("messages::MessageResource", "m_historyId", 5 + example_idx);
    rv->m_internalDate = ApiAutotest::INSTANCE().getInt("messages::MessageResource", "m_internalDate", 6 + example_idx);
    rv->m_payload = *(messages::MessagePayload::EXAMPLE(0, context_index).get());
    rv->m_sizeEstimate = ApiAutotest::INSTANCE().getInt("messages::MessageResource", "m_sizeEstimate", 8 + example_idx);
    rv->m_raw = ApiAutotest::INSTANCE().generateData("messages::MessageResource", context_index, parent_context_index);
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
