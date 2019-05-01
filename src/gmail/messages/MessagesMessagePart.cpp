/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/messages/MessagesMessagePart.h"
using namespace googleQt;

namespace googleQt{

namespace messages{
///MessagePart

MessagePart::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void MessagePart::toJson(QJsonObject& js)const{

    if(!m_partId.isEmpty())
        js["partId"] = QString(m_partId);
    if(!m_mimeType.isEmpty())
        js["mimeType"] = QString(m_mimeType);
    if(!m_filename.isEmpty())
        js["filename"] = QString(m_filename);
    js["headers"] = struct_list2jsonarray(m_headers);
    js["body"] = (QJsonObject)m_body;
}

void MessagePart::fromJson(const QJsonObject& js){

    m_partId = js["partId"].toString();
    m_mimeType = js["mimeType"].toString();
    m_filename = js["filename"].toString();
    jsonarray2struct_list(js["headers"].toArray(), m_headers);
    m_body.fromJson(js["body"].toObject());
}

QString MessagePart::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<MessagePart>  MessagePart::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<MessagePart>  MessagePart::factory::create(const QJsonObject& js)
{
    std::unique_ptr<MessagePart> rv;
    rv = std::unique_ptr<MessagePart>(new MessagePart);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<MessagePart> MessagePart::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<MessagePart> rv(new MessagePart);
    rv->m_partId = ApiAutotest::INSTANCE().getString("messages::MessagePart", "m_partId", QString("partId_%1").arg(example_idx));
    rv->m_mimeType = ApiAutotest::INSTANCE().getString("messages::MessagePart", "m_mimeType", QString("mimeType_%1").arg(example_idx));
    rv->m_filename = ApiAutotest::INSTANCE().getString("messages::MessagePart", "m_filename", QString("filename_%1").arg(example_idx));
    std::vector<messages::MessagePayloadHeader> list_of_headers;
    for(int i = 0; i < 5; i++){
        messages::MessagePayloadHeader p = *(messages::MessagePayloadHeader::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("messages::MessagePart", "messages::MessagePayloadHeader", &p, i, context_index);
        rv->m_headers.push_back(p);
    }
    rv->m_body = *(messages::MessagePartBody::EXAMPLE(0, context_index).get());
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
