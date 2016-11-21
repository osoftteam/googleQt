/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
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
std::unique_ptr<FileResourcesCollection> FileResourcesCollection::EXAMPLE(){
    std::unique_ptr<FileResourcesCollection> rv(new FileResourcesCollection);
    rv->m_nextPageToken = "test1value";
    rv->m_kind = "test2value";
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
