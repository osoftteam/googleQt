/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "permissions"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/permissions/PermissionsResourcePermission.h"
using namespace googleQt;

namespace googleQt{

namespace permissions{
///ResourcePermission

ResourcePermission::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void ResourcePermission::toJson(QJsonObject& js)const{

    if(!m_id.isEmpty())
        js["id"] = QString(m_id);
    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    if(!m_type.isEmpty())
        js["type"] = QString(m_type);
    if(!m_emailAddress.isEmpty())
        js["emailAddress"] = QString(m_emailAddress);
    if(!m_domain.isEmpty())
        js["domain"] = QString(m_domain);
    if(!m_role.isEmpty())
        js["role"] = QString(m_role);
    js["allowFileDiscovery"] = m_allowFileDiscovery;
    if(!m_displayName.isEmpty())
        js["displayName"] = QString(m_displayName);
    if(!m_photoLink.isEmpty())
        js["photoLink"] = QString(m_photoLink);
    if(m_expirationTime.isValid())
        js["expirationTime"] = m_expirationTime.toString(Qt::ISODate);
}

void ResourcePermission::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    m_kind = js["kind"].toString();
    m_type = js["type"].toString();
    m_emailAddress = js["emailAddress"].toString();
    m_domain = js["domain"].toString();
    m_role = js["role"].toString();
    m_allowFileDiscovery = js["allowFileDiscovery"].toVariant().toBool();
    m_displayName = js["displayName"].toString();
    m_photoLink = js["photoLink"].toString();
    m_expirationTime = QDateTime::fromString(js["expirationTime"].toString(), Qt::ISODate);
}

QString ResourcePermission::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<ResourcePermission>  ResourcePermission::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<ResourcePermission>  ResourcePermission::factory::create(const QJsonObject& js)
{
    std::unique_ptr<ResourcePermission> rv;
    rv = std::unique_ptr<ResourcePermission>(new ResourcePermission);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<ResourcePermission> ResourcePermission::EXAMPLE(int context_index){
    Q_UNUSED(context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<ResourcePermission> rv(new ResourcePermission);
    rv->m_id = QString("test1value_%1").arg(example_idx);
    rv->m_kind = QString("test2value_%1").arg(example_idx);
    rv->m_type = QString("test3value_%1").arg(example_idx);
    rv->m_emailAddress = QString("test4value_%1").arg(example_idx);
    rv->m_domain = QString("test5value_%1").arg(example_idx);
    rv->m_role = QString("test6value_%1").arg(example_idx);
    rv->m_displayName = QString("test8value_%1").arg(example_idx);
    rv->m_photoLink = QString("test9value_%1").arg(example_idx);
    rv->m_expirationTime = QDateTime::currentDateTime();
    return rv;
}
#endif //API_QT_AUTOTEST

}//permissions
}//googleQt
