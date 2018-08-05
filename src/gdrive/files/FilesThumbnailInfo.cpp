/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/files/FilesThumbnailInfo.h"
using namespace googleQt;

namespace googleQt{

namespace files{
///ThumbnailInfo

ThumbnailInfo::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void ThumbnailInfo::toJson(QJsonObject& js)const{

    js["image"] = m_image.constData();
    if(!m_mimeType.isEmpty())
        js["mimeType"] = QString(m_mimeType);
}

void ThumbnailInfo::fromJson(const QJsonObject& js){

    m_image = js["image"].toVariant().toByteArray();
    m_mimeType = js["mimeType"].toString();
}

QString ThumbnailInfo::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<ThumbnailInfo>  ThumbnailInfo::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<ThumbnailInfo>  ThumbnailInfo::factory::create(const QJsonObject& js)
{
    std::unique_ptr<ThumbnailInfo> rv;
    rv = std::unique_ptr<ThumbnailInfo>(new ThumbnailInfo);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<ThumbnailInfo> ThumbnailInfo::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<ThumbnailInfo> rv(new ThumbnailInfo);
    rv->m_image = ApiAutotest::INSTANCE().generateData("files::ThumbnailInfo", context_index, parent_context_index);
    rv->m_mimeType = ApiAutotest::INSTANCE().getString("files::ThumbnailInfo", "m_mimeType", QString("mimeType_%1").arg(example_idx));
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
