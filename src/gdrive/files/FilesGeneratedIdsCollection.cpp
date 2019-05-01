/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/files/FilesGeneratedIdsCollection.h"
using namespace googleQt;

namespace googleQt{

namespace files{
///GeneratedIdsCollection

GeneratedIdsCollection::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void GeneratedIdsCollection::toJson(QJsonObject& js)const{

    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    if(!m_space.isEmpty())
        js["space"] = QString(m_space);
    js["ids"] = ingrl_list2jsonarray(m_ids);
}

void GeneratedIdsCollection::fromJson(const QJsonObject& js){

    m_kind = js["kind"].toString();
    m_space = js["space"].toString();
    jsonarray2ingrl_list(js["ids"].toArray(), m_ids);
}

QString GeneratedIdsCollection::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<GeneratedIdsCollection>  GeneratedIdsCollection::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<GeneratedIdsCollection>  GeneratedIdsCollection::factory::create(const QJsonObject& js)
{
    std::unique_ptr<GeneratedIdsCollection> rv;
    rv = std::unique_ptr<GeneratedIdsCollection>(new GeneratedIdsCollection);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<GeneratedIdsCollection> GeneratedIdsCollection::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<GeneratedIdsCollection> rv(new GeneratedIdsCollection);
    rv->m_kind = ApiAutotest::INSTANCE().getString("files::GeneratedIdsCollection", "m_kind", QString("kind_%1").arg(example_idx));
    rv->m_space = ApiAutotest::INSTANCE().getString("files::GeneratedIdsCollection", "m_space", QString("space_%1").arg(example_idx));
    std::vector<QString> list_of_ids;
    for(int i = 0; i < 5; i++){
        rv->m_ids.push_back(QString("id_%1").arg(i+1));
    }
    QString tmp_m_ids = ApiAutotest::INSTANCE().getString4List("files::GeneratedIdsCollection", "m_ids");
    if(!tmp_m_ids.isEmpty())rv->m_ids.push_back(tmp_m_ids);
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
