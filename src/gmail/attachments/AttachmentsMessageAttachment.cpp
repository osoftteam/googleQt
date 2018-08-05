/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "attachments"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/attachments/AttachmentsMessageAttachment.h"
using namespace googleQt;

namespace googleQt{

namespace attachments{
///MessageAttachment

MessageAttachment::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void MessageAttachment::toJson(QJsonObject& js)const{

    if(!m_attachmentId.isEmpty())
        js["attachmentId"] = QString(m_attachmentId);
    js["data"] = m_data.constData();
    js["size"] = QString("%1").arg(m_size);
}

void MessageAttachment::fromJson(const QJsonObject& js){

    m_attachmentId = js["attachmentId"].toString();
    m_data = js["data"].toVariant().toByteArray();
    m_size = js["size"].toVariant().toString().toULongLong();
}

QString MessageAttachment::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<MessageAttachment>  MessageAttachment::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<MessageAttachment>  MessageAttachment::factory::create(const QJsonObject& js)
{
    std::unique_ptr<MessageAttachment> rv;
    rv = std::unique_ptr<MessageAttachment>(new MessageAttachment);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<MessageAttachment> MessageAttachment::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<MessageAttachment> rv(new MessageAttachment);
    rv->m_attachmentId = ApiAutotest::INSTANCE().getString("attachments::MessageAttachment", "m_attachmentId", QString("attachmentId_%1").arg(example_idx));
    rv->m_data = ApiAutotest::INSTANCE().generateData("attachments::MessageAttachment", context_index, parent_context_index);
    rv->m_size = ApiAutotest::INSTANCE().getInt("attachments::MessageAttachment", "m_size", 3 + example_idx);
    return rv;
}
#endif //API_QT_AUTOTEST

}//attachments
}//googleQt
