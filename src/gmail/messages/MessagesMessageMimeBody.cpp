/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#include "gmail/messages/MessagesMessageMimeBody.h"
using namespace googleQt;

namespace googleQt{

namespace messages{
///MessageMimeBody

MessageMimeBody::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void MessageMimeBody::toJson(QJsonObject& js)const{

    if(!m_attachmentId.isEmpty())
        js["attachmentId"] = QString(m_attachmentId);
    js["size"] = m_size;
    js["data"] = m_data.constData();
}

void MessageMimeBody::fromJson(const QJsonObject& js){

    m_attachmentId = js["attachmentId"].toString();
    m_size = js["size"].toVariant().toInt();
    m_data = js["data"].toVariant().toByteArray();
}

QString MessageMimeBody::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<MessageMimeBody>  MessageMimeBody::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<MessageMimeBody>  MessageMimeBody::factory::create(const QJsonObject& js)
{
    std::unique_ptr<MessageMimeBody> rv;
    rv = std::unique_ptr<MessageMimeBody>(new MessageMimeBody);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<MessageMimeBody> MessageMimeBody::EXAMPLE(int context_index){
    Q_UNUSED(context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<MessageMimeBody> rv(new MessageMimeBody);
    rv->m_attachmentId = QString("attachmentId_%1").arg(example_idx);
    rv->m_size = 2;
    rv->m_data = QByteArray("AUTOTEST-DATA").toBase64();
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
