/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/files/FilesUpdateFileDetails.h"
using namespace googleQt;

namespace googleQt{

namespace files{
///UpdateFileDetails

UpdateFileDetails::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void UpdateFileDetails::toJson(QJsonObject& js)const{

    if(!m_name.isEmpty())
        js["name"] = QString(m_name);
    if(!m_mimeType.isEmpty())
        js["mimeType"] = QString(m_mimeType);
    if(!m_description.isEmpty())
        js["description"] = QString(m_description);
    js["contentHints"] = (QJsonObject)m_contentHints;
    if(!m_originalFilename.isEmpty())
        js["originalFilename"] = QString(m_originalFilename);
}

void UpdateFileDetails::fromJson(const QJsonObject& js){

    m_name = js["name"].toString();
    m_mimeType = js["mimeType"].toString();
    m_description = js["description"].toString();
    m_contentHints.fromJson(js["contentHints"].toObject());
    m_originalFilename = js["originalFilename"].toString();
}

QString UpdateFileDetails::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<UpdateFileDetails>  UpdateFileDetails::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<UpdateFileDetails>  UpdateFileDetails::factory::create(const QJsonObject& js)
{
    std::unique_ptr<UpdateFileDetails> rv;
    rv = std::unique_ptr<UpdateFileDetails>(new UpdateFileDetails);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<UpdateFileDetails> UpdateFileDetails::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<UpdateFileDetails> rv(new UpdateFileDetails);
    rv->m_name = ApiAutotest::INSTANCE().getString("files::UpdateFileDetails", "m_name", QString("name_%1").arg(example_idx));
    rv->m_mimeType = ApiAutotest::INSTANCE().getString("files::UpdateFileDetails", "m_mimeType", QString("mimeType_%1").arg(example_idx));
    rv->m_description = ApiAutotest::INSTANCE().getString("files::UpdateFileDetails", "m_description", QString("description_%1").arg(example_idx));
    rv->m_contentHints = *(files::ContentHints::EXAMPLE(0, context_index).get());
    rv->m_originalFilename = ApiAutotest::INSTANCE().getString("files::UpdateFileDetails", "m_originalFilename", QString("originalFilename_%1").arg(example_idx));
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
