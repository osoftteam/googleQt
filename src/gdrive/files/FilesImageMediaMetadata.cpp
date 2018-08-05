/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/files/FilesImageMediaMetadata.h"
using namespace googleQt;

namespace googleQt{

namespace files{
///ImageMediaMetadata

ImageMediaMetadata::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void ImageMediaMetadata::toJson(QJsonObject& js)const{

    js["width"] = QString("%1").arg(m_width);
    js["height"] = QString("%1").arg(m_height);
    js["rotation"] = QString("%1").arg(m_rotation);
    js["location"] = (QJsonObject)m_location;
    if(!m_time.isEmpty())
        js["time"] = QString(m_time);
    if(!m_cameraMake.isEmpty())
        js["cameraMake"] = QString(m_cameraMake);
    if(!m_cameraModel.isEmpty())
        js["cameraModel"] = QString(m_cameraModel);
    js["exposureTime"] = m_exposureTime;
    js["flashUsed"] = m_flashUsed;
    js["focalLength"] = m_focalLength;
    js["isoSpeed"] = QString("%1").arg(m_isoSpeed);
    if(!m_meteringMode.isEmpty())
        js["meteringMode"] = QString(m_meteringMode);
    if(!m_sensor.isEmpty())
        js["sensor"] = QString(m_sensor);
    if(!m_exposureMode.isEmpty())
        js["exposureMode"] = QString(m_exposureMode);
    if(!m_colorSpace.isEmpty())
        js["colorSpace"] = QString(m_colorSpace);
    if(!m_whiteBalance.isEmpty())
        js["whiteBalance"] = QString(m_whiteBalance);
    js["exposureBias"] = m_exposureBias;
    js["maxApertureValue"] = m_maxApertureValue;
    js["subjectDistance"] = QString("%1").arg(m_subjectDistance);
    if(!m_lens.isEmpty())
        js["lens"] = QString(m_lens);
}

void ImageMediaMetadata::fromJson(const QJsonObject& js){

    m_width = js["width"].toVariant().toString().toULongLong();
    m_height = js["height"].toVariant().toString().toULongLong();
    m_rotation = js["rotation"].toVariant().toString().toULongLong();
    m_location.fromJson(js["location"].toObject());
    m_time = js["time"].toString();
    m_cameraMake = js["cameraMake"].toString();
    m_cameraModel = js["cameraModel"].toString();
    m_exposureTime = js["exposureTime"].toVariant().toFloat();
    m_flashUsed = js["flashUsed"].toVariant().toBool();
    m_focalLength = js["focalLength"].toVariant().toFloat();
    m_isoSpeed = js["isoSpeed"].toVariant().toString().toULongLong();
    m_meteringMode = js["meteringMode"].toString();
    m_sensor = js["sensor"].toString();
    m_exposureMode = js["exposureMode"].toString();
    m_colorSpace = js["colorSpace"].toString();
    m_whiteBalance = js["whiteBalance"].toString();
    m_exposureBias = js["exposureBias"].toVariant().toFloat();
    m_maxApertureValue = js["maxApertureValue"].toVariant().toFloat();
    m_subjectDistance = js["subjectDistance"].toVariant().toString().toULongLong();
    m_lens = js["lens"].toString();
}

QString ImageMediaMetadata::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<ImageMediaMetadata>  ImageMediaMetadata::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<ImageMediaMetadata>  ImageMediaMetadata::factory::create(const QJsonObject& js)
{
    std::unique_ptr<ImageMediaMetadata> rv;
    rv = std::unique_ptr<ImageMediaMetadata>(new ImageMediaMetadata);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<ImageMediaMetadata> ImageMediaMetadata::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<ImageMediaMetadata> rv(new ImageMediaMetadata);
    rv->m_width = ApiAutotest::INSTANCE().getInt("files::ImageMediaMetadata", "m_width", 1 + example_idx);
    rv->m_height = ApiAutotest::INSTANCE().getInt("files::ImageMediaMetadata", "m_height", 2 + example_idx);
    rv->m_rotation = ApiAutotest::INSTANCE().getInt("files::ImageMediaMetadata", "m_rotation", 3 + example_idx);
    rv->m_location = *(files::LocationData::EXAMPLE(0, context_index).get());
    rv->m_time = ApiAutotest::INSTANCE().getString("files::ImageMediaMetadata", "m_time", QString("time_%1").arg(example_idx));
    rv->m_cameraMake = ApiAutotest::INSTANCE().getString("files::ImageMediaMetadata", "m_cameraMake", QString("cameraMake_%1").arg(example_idx));
    rv->m_cameraModel = ApiAutotest::INSTANCE().getString("files::ImageMediaMetadata", "m_cameraModel", QString("cameraModel_%1").arg(example_idx));
    rv->m_exposureTime = ApiAutotest::INSTANCE().getInt("files::ImageMediaMetadata", "m_exposureTime", 8 + example_idx);
    rv->m_focalLength = ApiAutotest::INSTANCE().getInt("files::ImageMediaMetadata", "m_focalLength", 10 + example_idx);
    rv->m_isoSpeed = ApiAutotest::INSTANCE().getInt("files::ImageMediaMetadata", "m_isoSpeed", 11 + example_idx);
    rv->m_meteringMode = ApiAutotest::INSTANCE().getString("files::ImageMediaMetadata", "m_meteringMode", QString("meteringMode_%1").arg(example_idx));
    rv->m_sensor = ApiAutotest::INSTANCE().getString("files::ImageMediaMetadata", "m_sensor", QString("sensor_%1").arg(example_idx));
    rv->m_exposureMode = ApiAutotest::INSTANCE().getString("files::ImageMediaMetadata", "m_exposureMode", QString("exposureMode_%1").arg(example_idx));
    rv->m_colorSpace = ApiAutotest::INSTANCE().getString("files::ImageMediaMetadata", "m_colorSpace", QString("colorSpace_%1").arg(example_idx));
    rv->m_whiteBalance = ApiAutotest::INSTANCE().getString("files::ImageMediaMetadata", "m_whiteBalance", QString("whiteBalance_%1").arg(example_idx));
    rv->m_exposureBias = ApiAutotest::INSTANCE().getInt("files::ImageMediaMetadata", "m_exposureBias", 17 + example_idx);
    rv->m_maxApertureValue = ApiAutotest::INSTANCE().getInt("files::ImageMediaMetadata", "m_maxApertureValue", 18 + example_idx);
    rv->m_subjectDistance = ApiAutotest::INSTANCE().getInt("files::ImageMediaMetadata", "m_subjectDistance", 19 + example_idx);
    rv->m_lens = ApiAutotest::INSTANCE().getString("files::ImageMediaMetadata", "m_lens", QString("lens_%1").arg(example_idx));
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
