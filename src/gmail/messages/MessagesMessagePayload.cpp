/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#include "gmail/messages/MessagesMessagePayload.h"
using namespace googleQt;

namespace googleQt{

namespace messages{
///MessagePayload

MessagePayload::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void MessagePayload::toJson(QJsonObject& js)const{

    if(!m_partId.isEmpty())
        js["partId"] = QString(m_partId);
    if(!m_mimeType.isEmpty())
        js["mimeType"] = QString(m_mimeType);
    if(!m_filename.isEmpty())
        js["filename"] = QString(m_filename);
    js["headers"] = struct_list2jsonarray(m_headers);
    js["body"] = (QJsonObject)m_body;
    js["parts"] = struct_list2jsonarray(m_parts);
}

void MessagePayload::fromJson(const QJsonObject& js){

    m_partId = js["partId"].toString();
    m_mimeType = js["mimeType"].toString();
    m_filename = js["filename"].toString();
    jsonarray2struct_list(js["headers"].toArray(), m_headers);
    m_body.fromJson(js["body"].toObject());
    jsonarray2struct_list(js["parts"].toArray(), m_parts);
}

QString MessagePayload::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<MessagePayload>  MessagePayload::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<MessagePayload>  MessagePayload::factory::create(const QJsonObject& js)
{
    std::unique_ptr<MessagePayload> rv;
    rv = std::unique_ptr<MessagePayload>(new MessagePayload);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<MessagePayload> MessagePayload::EXAMPLE(){
    std::unique_ptr<MessagePayload> rv(new MessagePayload);
    rv->m_partId = "test1value";
    rv->m_mimeType = "test2value";
    rv->m_filename = "test3value";
    rv->m_body = *(messages::MessageMimeBody::EXAMPLE().get());
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
