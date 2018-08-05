/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "users"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/users/UsersGetProfileResponse.h"
using namespace googleQt;

namespace googleQt{

namespace users{
///GetProfileResponse

GetProfileResponse::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void GetProfileResponse::toJson(QJsonObject& js)const{

    if(!m_emailAddress.isEmpty())
        js["emailAddress"] = QString(m_emailAddress);
    js["messagesTotal"] = QString("%1").arg(m_messagesTotal);
    js["threadsTotal"] = QString("%1").arg(m_threadsTotal);
    js["historyId"] = QString("%1").arg(m_historyId);
}

void GetProfileResponse::fromJson(const QJsonObject& js){

    m_emailAddress = js["emailAddress"].toString();
    m_messagesTotal = js["messagesTotal"].toVariant().toString().toULongLong();
    m_threadsTotal = js["threadsTotal"].toVariant().toString().toULongLong();
    m_historyId = js["historyId"].toVariant().toString().toULongLong();
}

QString GetProfileResponse::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<GetProfileResponse>  GetProfileResponse::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<GetProfileResponse>  GetProfileResponse::factory::create(const QJsonObject& js)
{
    std::unique_ptr<GetProfileResponse> rv;
    rv = std::unique_ptr<GetProfileResponse>(new GetProfileResponse);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<GetProfileResponse> GetProfileResponse::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<GetProfileResponse> rv(new GetProfileResponse);
    rv->m_emailAddress = ApiAutotest::INSTANCE().getString("users::GetProfileResponse", "m_emailAddress", QString("emailAddress_%1").arg(example_idx));
    rv->m_messagesTotal = ApiAutotest::INSTANCE().getInt("users::GetProfileResponse", "m_messagesTotal", 2 + example_idx);
    rv->m_threadsTotal = ApiAutotest::INSTANCE().getInt("users::GetProfileResponse", "m_threadsTotal", 3 + example_idx);
    rv->m_historyId = ApiAutotest::INSTANCE().getInt("users::GetProfileResponse", "m_historyId", 4 + example_idx);
    return rv;
}
#endif //API_QT_AUTOTEST

}//users
}//googleQt
