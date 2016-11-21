/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
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
    js["resultSizeEstimate"] = m_resultSizeEstimate;
}

void MessageListRes::fromJson(const QJsonObject& js){

    jsonarray2struct_list(js["messages"].toArray(), m_messages);
    m_nextPageToken = js["nextPageToken"].toString();
    m_resultSizeEstimate = js["resultSizeEstimate"].toVariant().toInt();
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
std::unique_ptr<MessageListRes> MessageListRes::EXAMPLE(){
    std::unique_ptr<MessageListRes> rv(new MessageListRes);
    rv->m_nextPageToken = "test2value";
    rv->m_resultSizeEstimate = 3;
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
