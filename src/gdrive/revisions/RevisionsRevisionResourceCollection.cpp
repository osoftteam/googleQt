/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "revisions"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/revisions/RevisionsRevisionResourceCollection.h"
using namespace googleQt;

namespace googleQt{

namespace revisions{
///RevisionResourceCollection

RevisionResourceCollection::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void RevisionResourceCollection::toJson(QJsonObject& js)const{

    if(!m_nextPageToken.isEmpty())
        js["nextPageToken"] = QString(m_nextPageToken);
    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    js["files"] = struct_list2jsonarray(m_files);
}

void RevisionResourceCollection::fromJson(const QJsonObject& js){

    m_nextPageToken = js["nextPageToken"].toString();
    m_kind = js["kind"].toString();
    jsonarray2struct_list(js["files"].toArray(), m_files);
}

QString RevisionResourceCollection::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<RevisionResourceCollection>  RevisionResourceCollection::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<RevisionResourceCollection>  RevisionResourceCollection::factory::create(const QJsonObject& js)
{
    std::unique_ptr<RevisionResourceCollection> rv;
    rv = std::unique_ptr<RevisionResourceCollection>(new RevisionResourceCollection);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<RevisionResourceCollection> RevisionResourceCollection::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<RevisionResourceCollection> rv(new RevisionResourceCollection);
    rv->m_nextPageToken = ApiAutotest::INSTANCE().getString("revisions::RevisionResourceCollection", "m_nextPageToken", QString("nextPageToken_%1").arg(example_idx));
    rv->m_kind = ApiAutotest::INSTANCE().getString("revisions::RevisionResourceCollection", "m_kind", QString("kind_%1").arg(example_idx));
    std::vector<revisions::RevisionResource> list_of_files;
    for(int i = 0; i < 5; i++){
        revisions::RevisionResource p = *(revisions::RevisionResource::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("revisions::RevisionResourceCollection", "revisions::RevisionResource", &p, i, context_index);
        rv->m_files.push_back(p);
    }
    return rv;
}
#endif //API_QT_AUTOTEST

}//revisions
}//googleQt
