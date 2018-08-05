/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/messages/MessagesMessagePayloadHeader.h"
using namespace googleQt;

namespace googleQt{

namespace messages{
///MessagePayloadHeader

MessagePayloadHeader::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void MessagePayloadHeader::toJson(QJsonObject& js)const{

    if(!m_name.isEmpty())
        js["name"] = QString(m_name);
    if(!m_value.isEmpty())
        js["value"] = QString(m_value);
}

void MessagePayloadHeader::fromJson(const QJsonObject& js){

    m_name = js["name"].toString();
    m_value = js["value"].toString();
}

QString MessagePayloadHeader::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<MessagePayloadHeader>  MessagePayloadHeader::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<MessagePayloadHeader>  MessagePayloadHeader::factory::create(const QJsonObject& js)
{
    std::unique_ptr<MessagePayloadHeader> rv;
    rv = std::unique_ptr<MessagePayloadHeader>(new MessagePayloadHeader);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<MessagePayloadHeader> MessagePayloadHeader::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<MessagePayloadHeader> rv(new MessagePayloadHeader);
    rv->m_name = ApiAutotest::INSTANCE().getString("messages::MessagePayloadHeader", "m_name", QString("name_%1").arg(example_idx));
    rv->m_value = ApiAutotest::INSTANCE().getString("messages::MessagePayloadHeader", "m_value", QString("value_%1").arg(example_idx));
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
