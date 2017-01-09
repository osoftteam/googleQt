/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "about"
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
    js["usageInDrive"] = m_usageInDrive;
    js["usageInDriveTrash"] = m_usageInDriveTrash;
    js["maxUploadSize"] = m_maxUploadSize;
    js["appInstalled"] = m_appInstalled;
}

void StorageQuota::fromJson(const QJsonObject& js){

    m_limit = js["limit"].toVariant().toFloat();
    m_usage = js["usage"].toVariant().toFloat();
    m_usageInDrive = js["usageInDrive"].toVariant().toInt();
    m_usageInDriveTrash = js["usageInDriveTrash"].toVariant().toInt();
    m_maxUploadSize = js["maxUploadSize"].toVariant().toInt();
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
std::unique_ptr<StorageQuota> StorageQuota::EXAMPLE(int context_index){
    Q_UNUSED(context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<StorageQuota> rv(new StorageQuota);
    rv->m_limit = 1;
    rv->m_usage = 2;
    rv->m_usageInDrive = 3;
    rv->m_usageInDriveTrash = 4;
    rv->m_maxUploadSize = 5;
    return rv;
}
#endif //API_QT_AUTOTEST

}//about
}//googleQt
