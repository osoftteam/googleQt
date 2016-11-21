/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
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
std::unique_ptr<User> User::EXAMPLE(){
    std::unique_ptr<User> rv(new User);
    rv->m_displayName = "test1value";
    rv->m_kind = "test2value";
    rv->m_photoLink = "test3value";
    rv->m_permissionId = "test5value";
    rv->m_emailAddress = "test6value";
    return rv;
}
#endif //API_QT_AUTOTEST

}//comments
}//googleQt
