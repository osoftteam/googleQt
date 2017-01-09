/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#include "gmail/messages/MessagesMessagePartBody.h"
using namespace googleQt;

namespace googleQt{

namespace messages{
///MessagePartBody

MessagePartBody::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void MessagePartBody::toJson(QJsonObject& js)const{

    js["size"] = m_size;
    js["data"] = m_data.constData();
}

void MessagePartBody::fromJson(const QJsonObject& js){

    m_size = js["size"].toVariant().toInt();
    m_data = js["data"].toVariant().toByteArray();
}

QString MessagePartBody::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<MessagePartBody>  MessagePartBody::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<MessagePartBody>  MessagePartBody::factory::create(const QJsonObject& js)
{
    std::unique_ptr<MessagePartBody> rv;
    rv = std::unique_ptr<MessagePartBody>(new MessagePartBody);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<MessagePartBody> MessagePartBody::EXAMPLE(int context_index){
    Q_UNUSED(context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<MessagePartBody> rv(new MessagePartBody);
    rv->m_size = 1;
    rv->m_data = QByteArray("AUTOTEST-DATA").toBase64();
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
