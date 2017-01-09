/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "gdrive/about/AboutUserInfo.h"
#include "gdrive/permissions/PermissionsResourcePermission.h"
#include "gdrive/files/FilesContentHints.h"
#include "gdrive/files/FilesImageMediaMetadata.h"
#include "gdrive/files/FilesVideoMediaMetadata.h"

namespace googleQt{
namespace files{
    class FileResource{
        /**
            field: id: The ID of the file.
            field: kind: This is always drive#file.
            field: name: The name of the file. This is not necessarily unique
                within a folder.
            field: mime_type: The MIME type of the file. Drive will attempt to
                automatically detect an appropriate value from uploaded content
                if no value is provided. The value cannot be changed unless a
                new revision is uploaded.
            field: description: A short description of the file.
            field: starred: Whether the user has starred the file.
            field: trashed: Whether the file has been trashed, either explicitly
                or from a trashed parent folder. Only the owner may trash a
                file, and other users cannot see files in the owner's trash.
            field: explicitly_trashed: Whether the file has been explicitly
                trashed, as opposed to recursively trashed from a parent folder.
            field: parent: The IDs of the parent folders which contain the file.
                If not specified as part of a create request, the file will be
                placed directly in the My Drive folder. Update requests must use
                the addParents and removeParents parameters to modify the
                values.
            field: spaces: The list of spaces which contain the file. The
                currently supported values are 'drive', 'appDataFolder' and
                'photos'.
            field: version: A monotonically increasing version number for the
                file. This reflects every change made to the file on the server,
                even those not visible to the user.
            field: web_content_link: A link for downloading the content of the
                file in a browser. This is only available for files with binary
                content in Drive.
            field: web_view_link: A link for opening the file in a relevant
                Google editor or viewer in a browser.
            field: icon_link: A static, unauthenticated link to the file's icon.
            field: thumbnail_link: A short-lived link to the file's thumbnail,
                if available. Typically lasts on the order of hours.
            field: viewed_by_me: Whether the file has been viewed by this user.
            field: viewed_by_me_time: The last time the file was viewed by the
                user (RFC 3339 date-time).
            field: created_time: The time at which the file was created (RFC
                3339 date-time).
            field: modified_time: The last time the file was modified by anyone
                (RFC 3339 date-time). Note that setting modifiedTime will also
                update modifiedByMeTime for the user.
            field: modified_by_me_time: The last time the file was modified by
                the user (RFC 3339 date-time).
            field: shared_with_me_time: The time at which the file was shared
                with the user, if applicable (RFC 3339 date-time).
            field: sharing_user: The user who shared the file with the
                requesting user, if applicable.
            field: owners: The owners of the file. Currently, only certain
                legacy files may have more than one owner.
            field: last_modifying_user: The last user to modify the file.
            field: shared: Whether the file has been shared.
            field: owned_by_me: Whether the user owns the file.
            field: viewers_can_copy_content: Whether users with only reader or
                commenter permission can copy the file's content. This affects
                copy, download, and print operations.
            field: permissions: The full list of permissions for the file. This
                is only available if the requesting user can share the file.
            field: folder_color_rgb: The color for a folder as an RGB hex
                string. The supported colors are published in the
                folderColorPalette field of the About resource. If an
                unsupported color is specified, the closest color in the palette
                will be used instead.
            field: original_filename: The original filename of the uploaded
                content if available, or else the original value of the name
                field. This is only available for files with binary content in
                Drive.
            field: full_file_extension: The full file extension extracted from
                the name field. May contain multiple concatenated extensions,
                such as 'tar.gz'. This is only available for files with binary
                content in Drive. This is automatically updated when the name
                field changes, however it is not cleared if the new name does
                not contain a valid extension.
            field: file_extension: The final component of fullFileExtension.
                This is only available for files with binary content in Drive.
            field: md5_checksum: The MD5 checksum for the content of the file.
                This is only applicable to files with binary content in Drive.
            field: size: The size of the file's content in bytes. This is only
                applicable to files with binary content in Drive.
            field: quota_bytes_used: The number of storage quota bytes used by
                the file. This includes the head revision as well as previous
                revisions with keepForever enabled.
            field: head_revision_id: The ID of the file's head revision. This is
                currently only available for files with binary content in Drive.
            field: content_hints: Additional information about the content of
                the file. These fields are never populated in responses.
            field: image_media_metadata: Additional metadata about image media,
                if available.
            field: video_media_metadata: Additional metadata about video media.
                This may not be available immediately upon upload.
            field: is_app_authorized: Whether the file was created or opened by
                the requesting app.
        */

    public:
        FileResource():
        m_kind("drive#file")
        {};

        FileResource(const QString& arg):
        m_kind("drive#file")
        { m_id = arg; };

    public:
            /**
                The ID of the file.
            */
        QString id()const{return m_id;};
        FileResource& setId(const QString& arg){m_id=arg;return *this;};

            /**
                This is always drive#file.
            */
        QString kind()const{return m_kind;};
        FileResource& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                The name of the file. This is not necessarily unique within a
                folder.
            */
        QString name()const{return m_name;};
        FileResource& setName(const QString& arg){m_name=arg;return *this;};

            /**
                The MIME type of the file. Drive will attempt to automatically
                detect an appropriate value from uploaded content if no value is
                provided. The value cannot be changed unless a new revision is
                uploaded.
            */
        QString mimetype()const{return m_mimeType;};
        FileResource& setMimetype(const QString& arg){m_mimeType=arg;return *this;};

            /**
                A short description of the file.
            */
        QString description()const{return m_description;};
        FileResource& setDescription(const QString& arg){m_description=arg;return *this;};

            /**
                Whether the user has starred the file.
            */
        bool starred()const{return m_starred;};
        FileResource& setStarred(const bool& arg){m_starred=arg;return *this;};

            /**
                Whether the file has been trashed, either explicitly or from a
                trashed parent folder. Only the owner may trash a file, and
                other users cannot see files in the owner's trash.
            */
        bool trashed()const{return m_trashed;};
        FileResource& setTrashed(const bool& arg){m_trashed=arg;return *this;};

            /**
                Whether the file has been explicitly trashed, as opposed to
                recursively trashed from a parent folder.
            */
        bool explicitlytrashed()const{return m_explicitlyTrashed;};
        FileResource& setExplicitlytrashed(const bool& arg){m_explicitlyTrashed=arg;return *this;};

            /**
                The IDs of the parent folders which contain the file. If not
                specified as part of a create request, the file will be placed
                directly in the My Drive folder. Update requests must use the
                addParents and removeParents parameters to modify the values.
            */
        const std::list <QString>& parent()const{return m_parent;};
        FileResource& setParent(const std::list <QString>& arg){m_parent=arg;return *this;};

            /**
                The list of spaces which contain the file. The currently
                supported values are 'drive', 'appDataFolder' and 'photos'.
            */
        const std::list <QString>& spaces()const{return m_spaces;};
        FileResource& setSpaces(const std::list <QString>& arg){m_spaces=arg;return *this;};

            /**
                A monotonically increasing version number for the file. This
                reflects every change made to the file on the server, even those
                not visible to the user.
            */
        qint64 version()const{return m_version;};
        FileResource& setVersion(const qint64& arg){m_version=arg;return *this;};

            /**
                A link for downloading the content of the file in a browser.
                This is only available for files with binary content in Drive.
            */
        QString webcontentlink()const{return m_webContentLink;};
        FileResource& setWebcontentlink(const QString& arg){m_webContentLink=arg;return *this;};

            /**
                A link for opening the file in a relevant Google editor or
                viewer in a browser.
            */
        QString webviewlink()const{return m_webViewLink;};
        FileResource& setWebviewlink(const QString& arg){m_webViewLink=arg;return *this;};

            /**
                A static, unauthenticated link to the file's icon.
            */
        QString iconlink()const{return m_iconLink;};
        FileResource& setIconlink(const QString& arg){m_iconLink=arg;return *this;};

            /**
                A short-lived link to the file's thumbnail, if available.
                Typically lasts on the order of hours.
            */
        QString thumbnaillink()const{return m_thumbnailLink;};
        FileResource& setThumbnaillink(const QString& arg){m_thumbnailLink=arg;return *this;};

            /**
                Whether the file has been viewed by this user.
            */
        bool viewedbyme()const{return m_viewedByMe;};
        FileResource& setViewedbyme(const bool& arg){m_viewedByMe=arg;return *this;};

            /**
                The last time the file was viewed by the user (RFC 3339
                date-time).
            */
        QDateTime viewedbymetime()const{return m_viewedByMeTime;};
        FileResource& setViewedbymetime(const QDateTime& arg){m_viewedByMeTime=arg;return *this;};

            /**
                The time at which the file was created (RFC 3339 date-time).
            */
        QDateTime createdtime()const{return m_createdTime;};
        FileResource& setCreatedtime(const QDateTime& arg){m_createdTime=arg;return *this;};

            /**
                The last time the file was modified by anyone (RFC 3339
                date-time). Note that setting modifiedTime will also update
                modifiedByMeTime for the user.
            */
        QDateTime modifiedtime()const{return m_modifiedTime;};
        FileResource& setModifiedtime(const QDateTime& arg){m_modifiedTime=arg;return *this;};

            /**
                The last time the file was modified by the user (RFC 3339
                date-time).
            */
        QDateTime modifiedbymetime()const{return m_modifiedByMeTime;};
        FileResource& setModifiedbymetime(const QDateTime& arg){m_modifiedByMeTime=arg;return *this;};

            /**
                The time at which the file was shared with the user, if
                applicable (RFC 3339 date-time).
            */
        QDateTime sharedwithmetime()const{return m_sharedWithMeTime;};
        FileResource& setSharedwithmetime(const QDateTime& arg){m_sharedWithMeTime=arg;return *this;};

            /**
                The user who shared the file with the requesting user, if
                applicable.
            */
        const about::UserInfo& sharinguser()const{return m_sharingUser;};
        FileResource& setSharinguser(const about::UserInfo& arg){m_sharingUser=arg;return *this;};

            /**
                The owners of the file. Currently, only certain legacy files may
                have more than one owner.
            */
        const std::list <about::UserInfo>& owners()const{return m_owners;};
        FileResource& setOwners(const std::list <about::UserInfo>& arg){m_owners=arg;return *this;};

            /**
                The last user to modify the file.
            */
        const about::UserInfo& lastmodifyinguser()const{return m_lastModifyingUser;};
        FileResource& setLastmodifyinguser(const about::UserInfo& arg){m_lastModifyingUser=arg;return *this;};

            /**
                Whether the file has been shared.
            */
        bool shared()const{return m_shared;};
        FileResource& setShared(const bool& arg){m_shared=arg;return *this;};

            /**
                Whether the user owns the file.
            */
        bool ownedbyme()const{return m_ownedByMe;};
        FileResource& setOwnedbyme(const bool& arg){m_ownedByMe=arg;return *this;};

            /**
                Whether users with only reader or commenter permission can copy
                the file's content. This affects copy, download, and print
                operations.
            */
        bool viewerscancopycontent()const{return m_viewersCanCopyContent;};
        FileResource& setViewerscancopycontent(const bool& arg){m_viewersCanCopyContent=arg;return *this;};

            /**
                The full list of permissions for the file. This is only
                available if the requesting user can share the file.
            */
        const std::list <permissions::ResourcePermission>& permissions()const{return m_permissions;};
        FileResource& setPermissions(const std::list <permissions::ResourcePermission>& arg){m_permissions=arg;return *this;};

            /**
                The color for a folder as an RGB hex string. The supported
                colors are published in the folderColorPalette field of the
                About resource. If an unsupported color is specified, the
                closest color in the palette will be used instead.
            */
        QString foldercolorrgb()const{return m_folderColorRgb;};
        FileResource& setFoldercolorrgb(const QString& arg){m_folderColorRgb=arg;return *this;};

            /**
                The original filename of the uploaded content if available, or
                else the original value of the name field. This is only
                available for files with binary content in Drive.
            */
        QString originalfilename()const{return m_originalFilename;};
        FileResource& setOriginalfilename(const QString& arg){m_originalFilename=arg;return *this;};

            /**
                The full file extension extracted from the name field. May
                contain multiple concatenated extensions, such as 'tar.gz'. This
                is only available for files with binary content in Drive. This
                is automatically updated when the name field changes, however it
                is not cleared if the new name does not contain a valid
                extension.
            */
        QString fullfileextension()const{return m_fullFileExtension;};
        FileResource& setFullfileextension(const QString& arg){m_fullFileExtension=arg;return *this;};

            /**
                The final component of fullFileExtension. This is only available
                for files with binary content in Drive.
            */
        QString fileextension()const{return m_fileExtension;};
        FileResource& setFileextension(const QString& arg){m_fileExtension=arg;return *this;};

            /**
                The MD5 checksum for the content of the file. This is only
                applicable to files with binary content in Drive.
            */
        QString md5Checksum()const{return m_md5Checksum;};
        FileResource& setMd5Checksum(const QString& arg){m_md5Checksum=arg;return *this;};

            /**
                The size of the file's content in bytes. This is only applicable
                to files with binary content in Drive.
            */
        qint64 size()const{return m_size;};
        FileResource& setSize(const qint64& arg){m_size=arg;return *this;};

            /**
                The number of storage quota bytes used by the file. This
                includes the head revision as well as previous revisions with
                keepForever enabled.
            */
        qint64 quotabytesused()const{return m_quotaBytesUsed;};
        FileResource& setQuotabytesused(const qint64& arg){m_quotaBytesUsed=arg;return *this;};

            /**
                The ID of the file's head revision. This is currently only
                available for files with binary content in Drive.
            */
        QString headrevisionid()const{return m_headRevisionId;};
        FileResource& setHeadrevisionid(const QString& arg){m_headRevisionId=arg;return *this;};

            /**
                Additional information about the content of the file. These
                fields are never populated in responses.
            */
        const ContentHints& contenthints()const{return m_contentHints;};
        FileResource& setContenthints(const ContentHints& arg){m_contentHints=arg;return *this;};

            /**
                Additional metadata about image media, if available.
            */
        const ImageMediaMetadata& imagemediametadata()const{return m_imageMediaMetadata;};
        FileResource& setImagemediametadata(const ImageMediaMetadata& arg){m_imageMediaMetadata=arg;return *this;};

            /**
                Additional metadata about video media. This may not be available
                immediately upon upload.
            */
        const VideoMediaMetadata& videomediametadata()const{return m_videoMediaMetadata;};
        FileResource& setVideomediametadata(const VideoMediaMetadata& arg){m_videoMediaMetadata=arg;return *this;};

            /**
                Whether the file was created or opened by the requesting app.
            */
        bool isappauthorized()const{return m_isAppAuthorized;};
        FileResource& setIsappauthorized(const bool& arg){m_isAppAuthorized=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<FileResource>  create(const QByteArray& data);
            static std::unique_ptr<FileResource>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<FileResource> EXAMPLE(int context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The ID of the file.
            */
        QString m_id;

            /**
                This is always drive#file.
            */
        QString m_kind;

            /**
                The name of the file. This is not necessarily unique within a
                folder.
            */
        QString m_name;

            /**
                The MIME type of the file. Drive will attempt to automatically
                detect an appropriate value from uploaded content if no value is
                provided. The value cannot be changed unless a new revision is
                uploaded.
            */
        QString m_mimeType;

            /**
                A short description of the file.
            */
        QString m_description;

            /**
                Whether the user has starred the file.
            */
        bool m_starred;

            /**
                Whether the file has been trashed, either explicitly or from a
                trashed parent folder. Only the owner may trash a file, and
                other users cannot see files in the owner's trash.
            */
        bool m_trashed;

            /**
                Whether the file has been explicitly trashed, as opposed to
                recursively trashed from a parent folder.
            */
        bool m_explicitlyTrashed;

            /**
                The IDs of the parent folders which contain the file. If not
                specified as part of a create request, the file will be placed
                directly in the My Drive folder. Update requests must use the
                addParents and removeParents parameters to modify the values.
            */
        std::list <QString> m_parent;

            /**
                The list of spaces which contain the file. The currently
                supported values are 'drive', 'appDataFolder' and 'photos'.
            */
        std::list <QString> m_spaces;

            /**
                A monotonically increasing version number for the file. This
                reflects every change made to the file on the server, even those
                not visible to the user.
            */
        qint64 m_version = {0};

            /**
                A link for downloading the content of the file in a browser.
                This is only available for files with binary content in Drive.
            */
        QString m_webContentLink;

            /**
                A link for opening the file in a relevant Google editor or
                viewer in a browser.
            */
        QString m_webViewLink;

            /**
                A static, unauthenticated link to the file's icon.
            */
        QString m_iconLink;

            /**
                A short-lived link to the file's thumbnail, if available.
                Typically lasts on the order of hours.
            */
        QString m_thumbnailLink;

            /**
                Whether the file has been viewed by this user.
            */
        bool m_viewedByMe;

            /**
                The last time the file was viewed by the user (RFC 3339
                date-time).
            */
        QDateTime m_viewedByMeTime;

            /**
                The time at which the file was created (RFC 3339 date-time).
            */
        QDateTime m_createdTime;

            /**
                The last time the file was modified by anyone (RFC 3339
                date-time). Note that setting modifiedTime will also update
                modifiedByMeTime for the user.
            */
        QDateTime m_modifiedTime;

            /**
                The last time the file was modified by the user (RFC 3339
                date-time).
            */
        QDateTime m_modifiedByMeTime;

            /**
                The time at which the file was shared with the user, if
                applicable (RFC 3339 date-time).
            */
        QDateTime m_sharedWithMeTime;

            /**
                The user who shared the file with the requesting user, if
                applicable.
            */
        about::UserInfo m_sharingUser;

            /**
                The owners of the file. Currently, only certain legacy files may
                have more than one owner.
            */
        std::list <about::UserInfo> m_owners;

            /**
                The last user to modify the file.
            */
        about::UserInfo m_lastModifyingUser;

            /**
                Whether the file has been shared.
            */
        bool m_shared;

            /**
                Whether the user owns the file.
            */
        bool m_ownedByMe;

            /**
                Whether users with only reader or commenter permission can copy
                the file's content. This affects copy, download, and print
                operations.
            */
        bool m_viewersCanCopyContent;

            /**
                The full list of permissions for the file. This is only
                available if the requesting user can share the file.
            */
        std::list <permissions::ResourcePermission> m_permissions;

            /**
                The color for a folder as an RGB hex string. The supported
                colors are published in the folderColorPalette field of the
                About resource. If an unsupported color is specified, the
                closest color in the palette will be used instead.
            */
        QString m_folderColorRgb;

            /**
                The original filename of the uploaded content if available, or
                else the original value of the name field. This is only
                available for files with binary content in Drive.
            */
        QString m_originalFilename;

            /**
                The full file extension extracted from the name field. May
                contain multiple concatenated extensions, such as 'tar.gz'. This
                is only available for files with binary content in Drive. This
                is automatically updated when the name field changes, however it
                is not cleared if the new name does not contain a valid
                extension.
            */
        QString m_fullFileExtension;

            /**
                The final component of fullFileExtension. This is only available
                for files with binary content in Drive.
            */
        QString m_fileExtension;

            /**
                The MD5 checksum for the content of the file. This is only
                applicable to files with binary content in Drive.
            */
        QString m_md5Checksum;

            /**
                The size of the file's content in bytes. This is only applicable
                to files with binary content in Drive.
            */
        qint64 m_size = {0};

            /**
                The number of storage quota bytes used by the file. This
                includes the head revision as well as previous revisions with
                keepForever enabled.
            */
        qint64 m_quotaBytesUsed = {0};

            /**
                The ID of the file's head revision. This is currently only
                available for files with binary content in Drive.
            */
        QString m_headRevisionId;

            /**
                Additional information about the content of the file. These
                fields are never populated in responses.
            */
        ContentHints m_contentHints;

            /**
                Additional metadata about image media, if available.
            */
        ImageMediaMetadata m_imageMediaMetadata;

            /**
                Additional metadata about video media. This may not be available
                immediately upon upload.
            */
        VideoMediaMetadata m_videoMediaMetadata;

            /**
                Whether the file was created or opened by the requesting app.
            */
        bool m_isAppAuthorized;

    };//FileResource

}//files
}//googleQt
