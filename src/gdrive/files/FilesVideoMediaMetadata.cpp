/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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

    js["width"] = QString("%1").arg(m_width);
    js["height"] = QString("%1").arg(m_height);
    js["durationMillis"] = QString("%1").arg(m_durationMillis);
}

void VideoMediaMetadata::fromJson(const QJsonObject& js){

    m_width = js["width"].toVariant().toString().toULongLong();
    m_height = js["height"].toVariant().toString().toULongLong();
    m_durationMillis = js["durationMillis"].toVariant().toString().toULongLong();
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
std::unique_ptr<VideoMediaMetadata> VideoMediaMetadata::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<VideoMediaMetadata> rv(new VideoMediaMetadata);
    rv->m_width = ApiAutotest::INSTANCE().getInt("files::VideoMediaMetadata", "m_width", 1 + example_idx);
    rv->m_height = ApiAutotest::INSTANCE().getInt("files::VideoMediaMetadata", "m_height", 2 + example_idx);
    rv->m_durationMillis = ApiAutotest::INSTANCE().getInt("files::VideoMediaMetadata", "m_durationMillis", 3 + example_idx);
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
