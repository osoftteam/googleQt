/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/messages/MessagesMessageListRes.h"
using namespace googleQt;

namespace googleQt{

namespace messages{
///MessageListRes

MessageListRes::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void MessageListRes::toJson(QJsonObject& js)const{

    js["messages"] = struct_list2jsonarray(m_messages);
    if(!m_nextPageToken.isEmpty())
        js["nextPageToken"] = QString(m_nextPageToken);
    js["resultSizeEstimate"] = QString("%1").arg(m_resultSizeEstimate);
}

void MessageListRes::fromJson(const QJsonObject& js){

    jsonarray2struct_list(js["messages"].toArray(), m_messages);
    m_nextPageToken = js["nextPageToken"].toString();
    m_resultSizeEstimate = js["resultSizeEstimate"].toVariant().toString().toULongLong();
}

QString MessageListRes::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<MessageListRes>  MessageListRes::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<MessageListRes>  MessageListRes::factory::create(const QJsonObject& js)
{
    std::unique_ptr<MessageListRes> rv;
    rv = std::unique_ptr<MessageListRes>(new MessageListRes);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<MessageListRes> MessageListRes::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<MessageListRes> rv(new MessageListRes);
    std::vector<messages::MessageResource> list_of_messages;
    for(int i = 0; i < 5; i++){
        messages::MessageResource p = *(messages::MessageResource::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("messages::MessageListRes", "messages::MessageResource", &p, i, context_index);
        rv->m_messages.push_back(p);
    }
    rv->m_nextPageToken = ApiAutotest::INSTANCE().getString("messages::MessageListRes", "m_nextPageToken", QString("nextPageToken_%1").arg(example_idx));
    rv->m_resultSizeEstimate = ApiAutotest::INSTANCE().getInt("messages::MessageListRes", "m_resultSizeEstimate", 3 + example_idx);
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
