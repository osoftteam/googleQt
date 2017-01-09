/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
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

    js["width"] = m_width;
    js["height"] = m_height;
    js["rotation"] = m_rotation;
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
    js["isoSpeed"] = m_isoSpeed;
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
    js["subjectDistance"] = m_subjectDistance;
    if(!m_lens.isEmpty())
        js["lens"] = QString(m_lens);
}

void ImageMediaMetadata::fromJson(const QJsonObject& js){

    m_width = js["width"].toVariant().toInt();
    m_height = js["height"].toVariant().toInt();
    m_rotation = js["rotation"].toVariant().toInt();
    m_location.fromJson(js["location"].toObject());
    m_time = js["time"].toString();
    m_cameraMake = js["cameraMake"].toString();
    m_cameraModel = js["cameraModel"].toString();
    m_exposureTime = js["exposureTime"].toVariant().toFloat();
    m_flashUsed = js["flashUsed"].toVariant().toBool();
    m_focalLength = js["focalLength"].toVariant().toFloat();
    m_isoSpeed = js["isoSpeed"].toVariant().toInt();
    m_meteringMode = js["meteringMode"].toString();
    m_sensor = js["sensor"].toString();
    m_exposureMode = js["exposureMode"].toString();
    m_colorSpace = js["colorSpace"].toString();
    m_whiteBalance = js["whiteBalance"].toString();
    m_exposureBias = js["exposureBias"].toVariant().toFloat();
    m_maxApertureValue = js["maxApertureValue"].toVariant().toFloat();
    m_subjectDistance = js["subjectDistance"].toVariant().toInt();
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
std::unique_ptr<ImageMediaMetadata> ImageMediaMetadata::EXAMPLE(int context_index){
    Q_UNUSED(context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<ImageMediaMetadata> rv(new ImageMediaMetadata);
    rv->m_width = 1;
    rv->m_height = 2;
    rv->m_rotation = 3;
    rv->m_location = *(files::LocationData::EXAMPLE(0).get());
    rv->m_time = QString("test5value_%1").arg(example_idx);
    rv->m_cameraMake = QString("test6value_%1").arg(example_idx);
    rv->m_cameraModel = QString("test7value_%1").arg(example_idx);
    rv->m_exposureTime = 8;
    rv->m_focalLength = 10;
    rv->m_isoSpeed = 11;
    rv->m_meteringMode = QString("test12value_%1").arg(example_idx);
    rv->m_sensor = QString("test13value_%1").arg(example_idx);
    rv->m_exposureMode = QString("test14value_%1").arg(example_idx);
    rv->m_colorSpace = QString("test15value_%1").arg(example_idx);
    rv->m_whiteBalance = QString("test16value_%1").arg(example_idx);
    rv->m_exposureBias = 17;
    rv->m_maxApertureValue = 18;
    rv->m_subjectDistance = 19;
    rv->m_lens = QString("test20value_%1").arg(example_idx);
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
