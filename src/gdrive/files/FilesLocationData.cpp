/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/files/FilesLocationData.h"
using namespace googleQt;

namespace googleQt{

namespace files{
///LocationData

LocationData::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void LocationData::toJson(QJsonObject& js)const{

    js["latitude"] = m_latitude;
    js["longitude"] = m_longitude;
    js["altitude"] = m_altitude;
}

void LocationData::fromJson(const QJsonObject& js){

    m_latitude = js["latitude"].toVariant().toFloat();
    m_longitude = js["longitude"].toVariant().toFloat();
    m_altitude = js["altitude"].toVariant().toFloat();
}

QString LocationData::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<LocationData>  LocationData::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<LocationData>  LocationData::factory::create(const QJsonObject& js)
{
    std::unique_ptr<LocationData> rv;
    rv = std::unique_ptr<LocationData>(new LocationData);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<LocationData> LocationData::EXAMPLE(int context_index){
    Q_UNUSED(context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<LocationData> rv(new LocationData);
    rv->m_latitude = 1;
    rv->m_longitude = 2;
    rv->m_altitude = 3;
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
