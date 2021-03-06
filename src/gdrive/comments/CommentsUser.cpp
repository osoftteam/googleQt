/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/comments/CommentsUser.h"
using namespace googleQt;

namespace googleQt{

namespace comments{
///User

User::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void User::toJson(QJsonObject& js)const{

    if(!m_displayName.isEmpty())
        js["displayName"] = QString(m_displayName);
    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    if(!m_photoLink.isEmpty())
        js["photoLink"] = QString(m_photoLink);
    js["me"] = m_me;
    if(!m_permissionId.isEmpty())
        js["permissionId"] = QString(m_permissionId);
    if(!m_emailAddress.isEmpty())
        js["emailAddress"] = QString(m_emailAddress);
}

void User::fromJson(const QJsonObject& js){

    m_displayName = js["displayName"].toString();
    m_kind = js["kind"].toString();
    m_photoLink = js["photoLink"].toString();
    m_me = js["me"].toVariant().toBool();
    m_permissionId = js["permissionId"].toString();
    m_emailAddress = js["emailAddress"].toString();
}

QString User::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<User>  User::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<User>  User::factory::create(const QJsonObject& js)
{
    std::unique_ptr<User> rv;
    rv = std::unique_ptr<User>(new User);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<User> User::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<User> rv(new User);
    rv->m_displayName = ApiAutotest::INSTANCE().getString("comments::User", "m_displayName", QString("displayName_%1").arg(example_idx));
    rv->m_kind = ApiAutotest::INSTANCE().getString("comments::User", "m_kind", QString("kind_%1").arg(example_idx));
    rv->m_photoLink = ApiAutotest::INSTANCE().getString("comments::User", "m_photoLink", QString("photoLink_%1").arg(example_idx));
    rv->m_permissionId = ApiAutotest::INSTANCE().getString("comments::User", "m_permissionId", QString("permissionId_%1").arg(example_idx));
    rv->m_emailAddress = ApiAutotest::INSTANCE().getString("comments::User", "m_emailAddress", QString("emailAddress_%1").arg(example_idx));
    return rv;
}
#endif //API_QT_AUTOTEST

}//comments
}//googleQt
