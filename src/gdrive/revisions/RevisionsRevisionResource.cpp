/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "revisions"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/revisions/RevisionsRevisionResource.h"
using namespace googleQt;

namespace googleQt{

namespace revisions{
///RevisionResource

RevisionResource::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void RevisionResource::toJson(QJsonObject& js)const{

    if(!m_id.isEmpty())
        js["id"] = QString(m_id);
    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    if(!m_mimeType.isEmpty())
        js["mimeType"] = QString(m_mimeType);
    if(m_modifiedTime.isValid())
        js["modifiedTime"] = m_modifiedTime.toString(Qt::ISODate);
    js["keepForever"] = m_keepForever;
    js["published"] = m_published;
    js["publishAuto"] = m_publishAuto;
    js["publishedOutsideDomain"] = m_publishedOutsideDomain;
    js["lastModifyingUser"] = (QJsonObject)m_lastModifyingUser;
    if(!m_originalFilename.isEmpty())
        js["originalFilename"] = QString(m_originalFilename);
    if(!m_md5Checksum.isEmpty())
        js["md5Checksum"] = QString(m_md5Checksum);
    js["size"] = QString("%1").arg(m_size);
}

void RevisionResource::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    m_kind = js["kind"].toString();
    m_mimeType = js["mimeType"].toString();
    m_modifiedTime = QDateTime::fromString(js["modifiedTime"].toString(), Qt::ISODate);
    m_keepForever = js["keepForever"].toVariant().toBool();
    m_published = js["published"].toVariant().toBool();
    m_publishAuto = js["publishAuto"].toVariant().toBool();
    m_publishedOutsideDomain = js["publishedOutsideDomain"].toVariant().toBool();
    m_lastModifyingUser.fromJson(js["lastModifyingUser"].toObject());
    m_originalFilename = js["originalFilename"].toString();
    m_md5Checksum = js["md5Checksum"].toString();
    m_size = js["size"].toVariant().toString().toULongLong();
}

QString RevisionResource::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<RevisionResource>  RevisionResource::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<RevisionResource>  RevisionResource::factory::create(const QJsonObject& js)
{
    std::unique_ptr<RevisionResource> rv;
    rv = std::unique_ptr<RevisionResource>(new RevisionResource);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<RevisionResource> RevisionResource::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<RevisionResource> rv(new RevisionResource);
    rv->m_id = ApiAutotest::INSTANCE().getId("revisions::RevisionResource", example_idx);
    rv->m_kind = ApiAutotest::INSTANCE().getString("revisions::RevisionResource", "m_kind", QString("kind_%1").arg(example_idx));
    rv->m_mimeType = ApiAutotest::INSTANCE().getString("revisions::RevisionResource", "m_mimeType", QString("mimeType_%1").arg(example_idx));
    rv->m_modifiedTime = QDateTime::currentDateTime();
    rv->m_lastModifyingUser = *(revisions::RevisionUser::EXAMPLE(0, context_index).get());
    rv->m_originalFilename = ApiAutotest::INSTANCE().getString("revisions::RevisionResource", "m_originalFilename", QString("originalFilename_%1").arg(example_idx));
    rv->m_md5Checksum = ApiAutotest::INSTANCE().getString("revisions::RevisionResource", "m_md5Checksum", QString("md5Checksum_%1").arg(example_idx));
    rv->m_size = ApiAutotest::INSTANCE().getInt("revisions::RevisionResource", "m_size", 12 + example_idx);
    return rv;
}
#endif //API_QT_AUTOTEST

}//revisions
}//googleQt
