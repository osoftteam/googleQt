/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/files/FilesCreateFileDetails.h"
using namespace googleQt;

namespace googleQt{

namespace files{
///CreateFileDetails

CreateFileDetails::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void CreateFileDetails::toJson(QJsonObject& js)const{

    if(!m_id.isEmpty())
        js["id"] = QString(m_id);
    if(!m_mimeType.isEmpty())
        js["mimeType"] = QString(m_mimeType);
    if(!m_description.isEmpty())
        js["description"] = QString(m_description);
    js["contentHints"] = (QJsonObject)m_contentHints;
    if(!m_name.isEmpty())
        js["name"] = QString(m_name);
    if(!m_originalFilename.isEmpty())
        js["originalFilename"] = QString(m_originalFilename);
    js["parents"] = ingrl_list2jsonarray(m_parents);
    js["starred"] = m_starred;
}

void CreateFileDetails::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    m_mimeType = js["mimeType"].toString();
    m_description = js["description"].toString();
    m_contentHints.fromJson(js["contentHints"].toObject());
    m_name = js["name"].toString();
    m_originalFilename = js["originalFilename"].toString();
    jsonarray2ingrl_list(js["parents"].toArray(), m_parents);
    m_starred = js["starred"].toVariant().toBool();
}

QString CreateFileDetails::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<CreateFileDetails>  CreateFileDetails::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<CreateFileDetails>  CreateFileDetails::factory::create(const QJsonObject& js)
{
    std::unique_ptr<CreateFileDetails> rv;
    rv = std::unique_ptr<CreateFileDetails>(new CreateFileDetails);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<CreateFileDetails> CreateFileDetails::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<CreateFileDetails> rv(new CreateFileDetails);
    rv->m_id = ApiAutotest::INSTANCE().getId("files::CreateFileDetails", example_idx);
    rv->m_mimeType = ApiAutotest::INSTANCE().getString("files::CreateFileDetails", "m_mimeType", QString("mimeType_%1").arg(example_idx));
    rv->m_description = ApiAutotest::INSTANCE().getString("files::CreateFileDetails", "m_description", QString("description_%1").arg(example_idx));
    rv->m_contentHints = *(files::ContentHints::EXAMPLE(0, context_index).get());
    rv->m_name = ApiAutotest::INSTANCE().getString("files::CreateFileDetails", "m_name", QString("name_%1").arg(example_idx));
    rv->m_originalFilename = ApiAutotest::INSTANCE().getString("files::CreateFileDetails", "m_originalFilename", QString("originalFilename_%1").arg(example_idx));
    std::vector<QString> list_of_parents;
    for(int i = 0; i < 5; i++){
        rv->m_parents.push_back(QString("id_%1").arg(i+1));
    }
    QString tmp_m_parents = ApiAutotest::INSTANCE().getString4List("files::CreateFileDetails", "m_parents");
    if(!tmp_m_parents.isEmpty())rv->m_parents.push_back(tmp_m_parents);
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
