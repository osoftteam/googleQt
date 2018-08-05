/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/comments/CommentsQuotedFileContent.h"
using namespace googleQt;

namespace googleQt{

namespace comments{
///QuotedFileContent

QuotedFileContent::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void QuotedFileContent::toJson(QJsonObject& js)const{

    if(!m_mimeType.isEmpty())
        js["mimeType"] = QString(m_mimeType);
    if(!m_value.isEmpty())
        js["value"] = QString(m_value);
}

void QuotedFileContent::fromJson(const QJsonObject& js){

    m_mimeType = js["mimeType"].toString();
    m_value = js["value"].toString();
}

QString QuotedFileContent::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<QuotedFileContent>  QuotedFileContent::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<QuotedFileContent>  QuotedFileContent::factory::create(const QJsonObject& js)
{
    std::unique_ptr<QuotedFileContent> rv;
    rv = std::unique_ptr<QuotedFileContent>(new QuotedFileContent);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<QuotedFileContent> QuotedFileContent::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<QuotedFileContent> rv(new QuotedFileContent);
    rv->m_mimeType = ApiAutotest::INSTANCE().getString("comments::QuotedFileContent", "m_mimeType", QString("mimeType_%1").arg(example_idx));
    rv->m_value = ApiAutotest::INSTANCE().getString("comments::QuotedFileContent", "m_value", QString("value_%1").arg(example_idx));
    return rv;
}
#endif //API_QT_AUTOTEST

}//comments
}//googleQt
