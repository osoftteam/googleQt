/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/files/FilesVideoMediaMetadata.h"
using namespace googleQt;

namespace googleQt{

namespace files{
///VideoMediaMetadata

VideoMediaMetadata::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void VideoMediaMetadata::toJson(QJsonObject& js)const{

    js["width"] = m_width;
    js["height"] = m_height;
    js["durationMillis"] = m_durationMillis;
}

void VideoMediaMetadata::fromJson(const QJsonObject& js){

    m_width = js["width"].toVariant().toInt();
    m_height = js["height"].toVariant().toInt();
    m_durationMillis = js["durationMillis"].toVariant().toInt();
}

QString VideoMediaMetadata::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<VideoMediaMetadata>  VideoMediaMetadata::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<VideoMediaMetadata>  VideoMediaMetadata::factory::create(const QJsonObject& js)
{
    std::unique_ptr<VideoMediaMetadata> rv;
    rv = std::unique_ptr<VideoMediaMetadata>(new VideoMediaMetadata);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<VideoMediaMetadata> VideoMediaMetadata::EXAMPLE(int context_index){
    Q_UNUSED(context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<VideoMediaMetadata> rv(new VideoMediaMetadata);
    rv->m_width = 1;
    rv->m_height = 2;
    rv->m_durationMillis = 3;
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
