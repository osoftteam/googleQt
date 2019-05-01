/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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
    js["parents"] = ingrl_list2jsonarray(m_parents);
    js["spaces"] = ingrl_list2jsonarray(m_spaces);
    js["version"] = QString("%1").arg(m_version);
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
    js["size"] = QString("%1").arg(m_size);
    js["quotaBytesUsed"] = QString("%1").arg(m_quotaBytesUsed);
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
    jsonarray2ingrl_list(js["parents"].toArray(), m_parents);
    jsonarray2ingrl_list(js["spaces"].toArray(), m_spaces);
    m_version = js["version"].toVariant().toString().toULongLong();
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
    m_size = js["size"].toVariant().toString().toULongLong();
    m_quotaBytesUsed = js["quotaBytesUsed"].toVariant().toString().toULongLong();
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
std::unique_ptr<FileResource> FileResource::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<FileResource> rv(new FileResource);
    rv->m_id = ApiAutotest::INSTANCE().getId("files::FileResource", example_idx);
    rv->m_kind = ApiAutotest::INSTANCE().getString("files::FileResource", "m_kind", QString("kind_%1").arg(example_idx));
    rv->m_name = ApiAutotest::INSTANCE().getString("files::FileResource", "m_name", QString("name_%1").arg(example_idx));
    rv->m_mimeType = ApiAutotest::INSTANCE().getString("files::FileResource", "m_mimeType", QString("mimeType_%1").arg(example_idx));
    rv->m_description = ApiAutotest::INSTANCE().getString("files::FileResource", "m_description", QString("description_%1").arg(example_idx));
    std::vector<QString> list_of_parents;
    for(int i = 0; i < 5; i++){
        rv->m_parents.push_back(QString("id_%1").arg(i+1));
    }
    QString tmp_m_parents = ApiAutotest::INSTANCE().getString4List("files::FileResource", "m_parents");
    if(!tmp_m_parents.isEmpty())rv->m_parents.push_back(tmp_m_parents);
    std::vector<QString> list_of_spaces;
    for(int i = 0; i < 5; i++){
        rv->m_spaces.push_back(QString("id_%1").arg(i+1));
    }
    QString tmp_m_spaces = ApiAutotest::INSTANCE().getString4List("files::FileResource", "m_spaces");
    if(!tmp_m_spaces.isEmpty())rv->m_spaces.push_back(tmp_m_spaces);
    rv->m_version = ApiAutotest::INSTANCE().getInt("files::FileResource", "m_version", 11 + example_idx);
    rv->m_webContentLink = ApiAutotest::INSTANCE().getString("files::FileResource", "m_webContentLink", QString("webContentLink_%1").arg(example_idx));
    rv->m_webViewLink = ApiAutotest::INSTANCE().getString("files::FileResource", "m_webViewLink", QString("webViewLink_%1").arg(example_idx));
    rv->m_iconLink = ApiAutotest::INSTANCE().getString("files::FileResource", "m_iconLink", QString("iconLink_%1").arg(example_idx));
    rv->m_thumbnailLink = ApiAutotest::INSTANCE().getString("files::FileResource", "m_thumbnailLink", QString("thumbnailLink_%1").arg(example_idx));
    rv->m_viewedByMeTime = QDateTime::currentDateTime();
    rv->m_createdTime = QDateTime::currentDateTime();
    rv->m_modifiedTime = QDateTime::currentDateTime();
    rv->m_modifiedByMeTime = QDateTime::currentDateTime();
    rv->m_sharedWithMeTime = QDateTime::currentDateTime();
    rv->m_sharingUser = *(about::UserInfo::EXAMPLE(0, context_index).get());
    std::vector<about::UserInfo> list_of_owners;
    for(int i = 0; i < 5; i++){
        about::UserInfo p = *(about::UserInfo::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("files::FileResource", "about::UserInfo", &p, i, context_index);
        rv->m_owners.push_back(p);
    }
    rv->m_lastModifyingUser = *(about::UserInfo::EXAMPLE(0, context_index).get());
    std::vector<permissions::ResourcePermission> list_of_permissions;
    for(int i = 0; i < 5; i++){
        permissions::ResourcePermission p = *(permissions::ResourcePermission::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("files::FileResource", "permissions::ResourcePermission", &p, i, context_index);
        rv->m_permissions.push_back(p);
    }
    rv->m_folderColorRgb = ApiAutotest::INSTANCE().getString("files::FileResource", "m_folderColorRgb", QString("folderColorRgb_%1").arg(example_idx));
    rv->m_originalFilename = ApiAutotest::INSTANCE().getString("files::FileResource", "m_originalFilename", QString("originalFilename_%1").arg(example_idx));
    rv->m_fullFileExtension = ApiAutotest::INSTANCE().getString("files::FileResource", "m_fullFileExtension", QString("fullFileExtension_%1").arg(example_idx));
    rv->m_fileExtension = ApiAutotest::INSTANCE().getString("files::FileResource", "m_fileExtension", QString("fileExtension_%1").arg(example_idx));
    rv->m_md5Checksum = ApiAutotest::INSTANCE().getString("files::FileResource", "m_md5Checksum", QString("md5Checksum_%1").arg(example_idx));
    rv->m_size = ApiAutotest::INSTANCE().getInt("files::FileResource", "m_size", 34 + example_idx);
    rv->m_quotaBytesUsed = ApiAutotest::INSTANCE().getInt("files::FileResource", "m_quotaBytesUsed", 35 + example_idx);
    rv->m_headRevisionId = ApiAutotest::INSTANCE().getString("files::FileResource", "m_headRevisionId", QString("headRevisionId_%1").arg(example_idx));
    rv->m_contentHints = *(files::ContentHints::EXAMPLE(0, context_index).get());
    rv->m_imageMediaMetadata = *(files::ImageMediaMetadata::EXAMPLE(0, context_index).get());
    rv->m_videoMediaMetadata = *(files::VideoMediaMetadata::EXAMPLE(0, context_index).get());
    return rv;
}
#endif //API_QT_AUTOTEST

}//files
}//googleQt
