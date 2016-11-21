/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "permissions"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/permissions/PermissionsPermissionResourcesCollection.h"
using namespace googleQt;

namespace googleQt{

namespace permissions{
///PermissionResourcesCollection

PermissionResourcesCollection::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void PermissionResourcesCollection::toJson(QJsonObject& js)const{

    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    js["permissions"] = struct_list2jsonarray(m_permissions);
}

void PermissionResourcesCollection::fromJson(const QJsonObject& js){

    m_kind = js["kind"].toString();
    jsonarray2struct_list(js["permissions"].toArray(), m_permissions);
}

QString PermissionResourcesCollection::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<PermissionResourcesCollection>  PermissionResourcesCollection::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<PermissionResourcesCollection>  PermissionResourcesCollection::factory::create(const QJsonObject& js)
{
    std::unique_ptr<PermissionResourcesCollection> rv;
    rv = std::unique_ptr<PermissionResourcesCollection>(new PermissionResourcesCollection);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<PermissionResourcesCollection> PermissionResourcesCollection::EXAMPLE(){
    std::unique_ptr<PermissionResourcesCollection> rv(new PermissionResourcesCollection);
    rv->m_kind = "test1value";
    return rv;
}
#endif //API_QT_AUTOTEST

}//permissions
}//googleQt
