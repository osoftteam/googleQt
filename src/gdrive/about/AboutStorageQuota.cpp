/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "about"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/about/AboutStorageQuota.h"
using namespace googleQt;

namespace googleQt{

namespace about{
///StorageQuota

StorageQuota::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void StorageQuota::toJson(QJsonObject& js)const{

    js["limit"] = m_limit;
    js["usage"] = m_usage;
    js["usageInDrive"] = QString("%1").arg(m_usageInDrive);
    js["usageInDriveTrash"] = QString("%1").arg(m_usageInDriveTrash);
    js["maxUploadSize"] = QString("%1").arg(m_maxUploadSize);
    js["appInstalled"] = m_appInstalled;
}

void StorageQuota::fromJson(const QJsonObject& js){

    m_limit = js["limit"].toVariant().toFloat();
    m_usage = js["usage"].toVariant().toFloat();
    m_usageInDrive = js["usageInDrive"].toVariant().toString().toULongLong();
    m_usageInDriveTrash = js["usageInDriveTrash"].toVariant().toString().toULongLong();
    m_maxUploadSize = js["maxUploadSize"].toVariant().toString().toULongLong();
    m_appInstalled = js["appInstalled"].toVariant().toBool();
}

QString StorageQuota::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<StorageQuota>  StorageQuota::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<StorageQuota>  StorageQuota::factory::create(const QJsonObject& js)
{
    std::unique_ptr<StorageQuota> rv;
    rv = std::unique_ptr<StorageQuota>(new StorageQuota);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<StorageQuota> StorageQuota::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<StorageQuota> rv(new StorageQuota);
    rv->m_limit = ApiAutotest::INSTANCE().getInt("about::StorageQuota", "m_limit", 1 + example_idx);
    rv->m_usage = ApiAutotest::INSTANCE().getInt("about::StorageQuota", "m_usage", 2 + example_idx);
    rv->m_usageInDrive = ApiAutotest::INSTANCE().getInt("about::StorageQuota", "m_usageInDrive", 3 + example_idx);
    rv->m_usageInDriveTrash = ApiAutotest::INSTANCE().getInt("about::StorageQuota", "m_usageInDriveTrash", 4 + example_idx);
    rv->m_maxUploadSize = ApiAutotest::INSTANCE().getInt("about::StorageQuota", "m_maxUploadSize", 5 + example_idx);
    return rv;
}
#endif //API_QT_AUTOTEST

}//about
}//googleQt
