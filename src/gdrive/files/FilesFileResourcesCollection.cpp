/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/files/FilesFileResourcesCollection.h"
using namespace googleQt;

namespace googleQt{

namespace files{
///FileResourcesCollection

FileResourcesCollection::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void FileResourcesCollection::toJson(QJsonObject& js)const{

    if(!m_nextPageToken.isEmpty())
        js["nextPageToken"] = QString(m_nextPageToken);
    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    js["files"] = struct_list2jsonarray(m_files);
}

void FileResourcesCollection::fromJson(const QJsonObject& js){

    m_nextPageToken = js["nextPageToken"].toString();
    m_kind = js["kind"].toString();
    jsonarray2struct_list(js["files"].toArray(), m_files);
}

QString FileResourcesCollection::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<FileResourcesCollection>  FileResourcesCollection::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<FileResourcesCollection>  FileResourcesCollection::factory::create(const QJsonObject& js)
{
    std::unique_ptr<FileResourcesCollection> rv;
    rv = std::unique_ptr<FileResourcesCollection>(new FileResourcesCollection);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<FileResourcesCollection> FileResourcesCollection::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<FileResourcesCollection> rv(new FileResourcesCollection);
    rv->m_nextPageToken = ApiAutotest::INSTANCE().getString("files::FileResourcesCollection", "m_nextPageToken", QString("nextPageToken_%1").arg(example_idx));
    rv->m_kind = ApiAutotest::INSTANCE().getString("files::FileResourcesCollection", "m_kind", QString("kind_%1").arg(example_idx));
    std::vector<files::FileResource> list_of_files;
    for(int i = 0; i < 5; i++){
        files::FileResource p = *(files::FileResource::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("files::FileResourcesCollection", "files::FileResource", &p, i, context_index);
        rv->m_files.push_back(p);
    }
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
