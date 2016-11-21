/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/files/FilesFileResource.h"
using namespace googleQt;

namespace googleQt{

namespace files{
///FileResource

FileResource::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void FileResource::toJson(QJsonObject& js)const{

    if(!m_id.isEmpty())
        js["id"] = QString(m_id);
    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    if(!m_name.isEmpty())
        js["name"] = QString(m_name);
    if(!m_mimeType.isEmpty())
        js["mimeType"] = QString(m_mimeType);
    if(!m_description.isEmpty())
        js["description"] = QString(m_description);
    js["starred"] = m_starred;
    js["trashed"] = m_trashed;
    js["explicitlyTrashed"] = m_explicitlyTrashed;
    js["parent"] = ingrl_list2jsonarray(m_parent);
    js["spaces"] = ingrl_list2jsonarray(m_spaces);
    js["version"] = m_version;
    if(!m_webContentLink.isEmpty())
        js["webContentLink"] = QString(m_webContentLink);
    if(!m_webViewLink.isEmpty())
        js["webViewLink"] = QString(m_webViewLink);
    if(!m_iconLink.isEmpty())
        js["iconLink"] = QString(m_iconLink);
    if(!m_thumbnailLink.isEmpty())
        js["thumbnailLink"] = QString(m_thumbnailLink);
    js["viewedByMe"] = m_viewedByMe;
    if(m_viewedByMeTime.isValid())
        js["viewedByMeTime"] = m_viewedByMeTime.toString(Qt::ISODate);
    if(m_createdTime.isValid())
        js["createdTime"] = m_createdTime.toString(Qt::ISODate);
    if(m_modifiedTime.isValid())
        js["modifiedTime"] = m_modifiedTime.toString(Qt::ISODate);
    if(m_modifiedByMeTime.isValid())
        js["modifiedByMeTime"] = m_modifiedByMeTime.toString(Qt::ISODate);
    if(m_sharedWithMeTime.isValid())
        js["sharedWithMeTime"] = m_sharedWithMeTime.toString(Qt::ISODate);
    js["sharingUser"] = (QJsonObject)m_sharingUser;
    js["owners"] = struct_list2jsonarray(m_owners);
    js["lastModifyingUser"] = (QJsonObject)m_lastModifyingUser;
    js["shared"] = m_shared;
    js["ownedByMe"] = m_ownedByMe;
    js["viewersCanCopyContent"] = m_viewersCanCopyContent;
    js["permissions"] = struct_list2jsonarray(m_permissions);
    if(!m_folderColorRgb.isEmpty())
        js["folderColorRgb"] = QString(m_folderColorRgb);
    if(!m_originalFilename.isEmpty())
        js["originalFilename"] = QString(m_originalFilename);
    if(!m_fullFileExtension.isEmpty())
        js["fullFileExtension"] = QString(m_fullFileExtension);
    if(!m_fileExtension.isEmpty())
        js["fileExtension"] = QString(m_fileExtension);
    if(!m_md5Checksum.isEmpty())
        js["md5Checksum"] = QString(m_md5Checksum);
    js["size"] = m_size;
    js["quotaBytesUsed"] = m_quotaBytesUsed;
    if(!m_headRevisionId.isEmpty())
        js["headRevisionId"] = QString(m_headRevisionId);
    js["contentHints"] = (QJsonObject)m_contentHints;
    js["imageMediaMetadata"] = (QJsonObject)m_imageMediaMetadata;
    js["videoMediaMetadata"] = (QJsonObject)m_videoMediaMetadata;
    js["isAppAuthorized"] = m_isAppAuthorized;
}

void FileResource::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    m_kind = js["kind"].toString();
    m_name = js["name"].toString();
    m_mimeType = js["mimeType"].toString();
    m_description = js["description"].toString();
    m_starred = js["starred"].toVariant().toBool();
    m_trashed = js["trashed"].toVariant().toBool();
    m_explicitlyTrashed = js["explicitlyTrashed"].toVariant().toBool();
    jsonarray2ingrl_list(js["parent"].toArray(), m_parent);
    jsonarray2ingrl_list(js["spaces"].toArray(), m_spaces);
    m_version = js["version"].toVariant().toInt();
    m_webContentLink = js["webContentLink"].toString();
    m_webViewLink = js["webViewLink"].toString();
    m_iconLink = js["iconLink"].toString();
    m_thumbnailLink = js["thumbnailLink"].toString();
    m_viewedByMe = js["viewedByMe"].toVariant().toBool();
    m_viewedByMeTime = QDateTime::fromString(js["viewedByMeTime"].toString(), Qt::ISODate);
    m_createdTime = QDateTime::fromString(js["createdTime"].toString(), Qt::ISODate);
    m_modifiedTime = QDateTime::fromString(js["modifiedTime"].toString(), Qt::ISODate);
    m_modifiedByMeTime = QDateTime::fromString(js["modifiedByMeTime"].toString(), Qt::ISODate);
    m_sharedWithMeTime = QDateTime::fromString(js["sharedWithMeTime"].toString(), Qt::ISODate);
    m_sharingUser.fromJson(js["sharingUser"].toObject());
    jsonarray2struct_list(js["owners"].toArray(), m_owners);
    m_lastModifyingUser.fromJson(js["lastModifyingUser"].toObject());
    m_shared = js["shared"].toVariant().toBool();
    m_ownedByMe = js["ownedByMe"].toVariant().toBool();
    m_viewersCanCopyContent = js["viewersCanCopyContent"].toVariant().toBool();
    jsonarray2struct_list(js["permissions"].toArray(), m_permissions);
    m_folderColorRgb = js["folderColorRgb"].toString();
    m_originalFilename = js["originalFilename"].toString();
    m_fullFileExtension = js["fullFileExtension"].toString();
    m_fileExtension = js["fileExtension"].toString();
    m_md5Checksum = js["md5Checksum"].toString();
    m_size = js["size"].toVariant().toInt();
    m_quotaBytesUsed = js["quotaBytesUsed"].toVariant().toInt();
    m_headRevisionId = js["headRevisionId"].toString();
    m_contentHints.fromJson(js["contentHints"].toObject());
    m_imageMediaMetadata.fromJson(js["imageMediaMetadata"].toObject());
    m_videoMediaMetadata.fromJson(js["videoMediaMetadata"].toObject());
    m_isAppAuthorized = js["isAppAuthorized"].toVariant().toBool();
}

QString FileResource::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<FileResource>  FileResource::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<FileResource>  FileResource::factory::create(const QJsonObject& js)
{
    std::unique_ptr<FileResource> rv;
    rv = std::unique_ptr<FileResource>(new FileResource);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<FileResource> FileResource::EXAMPLE(){
    std::unique_ptr<FileResource> rv(new FileResource);
    rv->m_id = "test1value";
    rv->m_kind = "test2value";
    rv->m_name = "test3value";
    rv->m_mimeType = "test4value";
    rv->m_description = "test5value";
    rv->m_version = 11;
    rv->m_webContentLink = "test12value";
    rv->m_webViewLink = "test13value";
    rv->m_iconLink = "test14value";
    rv->m_thumbnailLink = "test15value";
    rv->m_viewedByMeTime = QDateTime::currentDateTime();
    rv->m_createdTime = QDateTime::currentDateTime();
    rv->m_modifiedTime = QDateTime::currentDateTime();
    rv->m_modifiedByMeTime = QDateTime::currentDateTime();
    rv->m_sharedWithMeTime = QDateTime::currentDateTime();
    rv->m_sharingUser = *(about::UserInfo::EXAMPLE().get());
    rv->m_lastModifyingUser = *(about::UserInfo::EXAMPLE().get());
    rv->m_folderColorRgb = "test29value";
    rv->m_originalFilename = "test30value";
    rv->m_fullFileExtension = "test31value";
    rv->m_fileExtension = "test32value";
    rv->m_md5Checksum = "test33value";
    rv->m_size = 34;
    rv->m_quotaBytesUsed = 35;
    rv->m_headRevisionId = "test36value";
    rv->m_contentHints = *(files::ContentHints::EXAMPLE().get());
    rv->m_imageMediaMetadata = *(files::ImageMediaMetadata::EXAMPLE().get());
    rv->m_videoMediaMetadata = *(files::VideoMediaMetadata::EXAMPLE().get());
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
