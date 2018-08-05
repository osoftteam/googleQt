/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "labels"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/labels/LabelsLabelResource.h"
using namespace googleQt;

namespace googleQt{

namespace labels{
///LabelResource

LabelResource::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void LabelResource::toJson(QJsonObject& js)const{

    if(!m_id.isEmpty())
        js["id"] = QString(m_id);
    if(!m_name.isEmpty())
        js["name"] = QString(m_name);
    if(!m_messageListVisibility.isEmpty())
        js["messageListVisibility"] = QString(m_messageListVisibility);
    if(!m_labelListVisibility.isEmpty())
        js["labelListVisibility"] = QString(m_labelListVisibility);
    if(!m_type.isEmpty())
        js["type"] = QString(m_type);
    js["messagesTotal"] = QString("%1").arg(m_messagesTotal);
    js["messagesUnread"] = QString("%1").arg(m_messagesUnread);
    js["threadsTotal"] = QString("%1").arg(m_threadsTotal);
}

void LabelResource::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    m_name = js["name"].toString();
    m_messageListVisibility = js["messageListVisibility"].toString();
    m_labelListVisibility = js["labelListVisibility"].toString();
    m_type = js["type"].toString();
    m_messagesTotal = js["messagesTotal"].toVariant().toString().toULongLong();
    m_messagesUnread = js["messagesUnread"].toVariant().toString().toULongLong();
    m_threadsTotal = js["threadsTotal"].toVariant().toString().toULongLong();
}

QString LabelResource::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<LabelResource>  LabelResource::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<LabelResource>  LabelResource::factory::create(const QJsonObject& js)
{
    std::unique_ptr<LabelResource> rv;
    rv = std::unique_ptr<LabelResource>(new LabelResource);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<LabelResource> LabelResource::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<LabelResource> rv(new LabelResource);
    rv->m_id = ApiAutotest::INSTANCE().getId("labels::LabelResource", example_idx);
    rv->m_name = ApiAutotest::INSTANCE().getString("labels::LabelResource", "m_name", QString("name_%1").arg(example_idx));
    rv->m_messageListVisibility = ApiAutotest::INSTANCE().getString("labels::LabelResource", "m_messageListVisibility", QString("messageListVisibility_%1").arg(example_idx));
    rv->m_labelListVisibility = ApiAutotest::INSTANCE().getString("labels::LabelResource", "m_labelListVisibility", QString("labelListVisibility_%1").arg(example_idx));
    rv->m_type = ApiAutotest::INSTANCE().getString("labels::LabelResource", "m_type", QString("type_%1").arg(example_idx));
    rv->m_messagesTotal = ApiAutotest::INSTANCE().getInt("labels::LabelResource", "m_messagesTotal", 6 + example_idx);
    rv->m_messagesUnread = ApiAutotest::INSTANCE().getInt("labels::LabelResource", "m_messagesUnread", 7 + example_idx);
    rv->m_threadsTotal = ApiAutotest::INSTANCE().getInt("labels::LabelResource", "m_threadsTotal", 8 + example_idx);
    return rv;
}
#endif //API_QT_AUTOTEST

}//labels
}//googleQt
