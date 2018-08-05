/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/files/FilesContentHints.h"
using namespace googleQt;

namespace googleQt{

namespace files{
///ContentHints

ContentHints::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void ContentHints::toJson(QJsonObject& js)const{

    js["thumbnail"] = (QJsonObject)m_thumbnail;
    if(!m_indexableText.isEmpty())
        js["indexableText"] = QString(m_indexableText);
}

void ContentHints::fromJson(const QJsonObject& js){

    m_thumbnail.fromJson(js["thumbnail"].toObject());
    m_indexableText = js["indexableText"].toString();
}

QString ContentHints::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<ContentHints>  ContentHints::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<ContentHints>  ContentHints::factory::create(const QJsonObject& js)
{
    std::unique_ptr<ContentHints> rv;
    rv = std::unique_ptr<ContentHints>(new ContentHints);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<ContentHints> ContentHints::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<ContentHints> rv(new ContentHints);
    rv->m_thumbnail = *(files::ThumbnailInfo::EXAMPLE(0, context_index).get());
    rv->m_indexableText = ApiAutotest::INSTANCE().getString("files::ContentHints", "m_indexableText", QString("indexableText_%1").arg(example_idx));
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
