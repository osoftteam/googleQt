/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "revisions"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "gdrive/revisions/RevisionsRevisionUser.h"

namespace googleQt{
namespace revisions{
    class RevisionResource{
        /**
            field: id: The ID of the revision.
            field: kind: Identifies what kind of resource this is. Value: the
                fixed string drive#revision.
            field: mime_type: The MIME type of the revision.
            field: modified_time: The last time the revision was modified  (RFC
                3339 date-time).
            field: keep_forever: Whether to keep this revision forever, even if
                it is no longer the head revision. If not set, the revision will
                be automatically purged 30 days after newer content is uploaded.
                This can be set on a maximum of 200 revisions for a file. This
                field is only applicable to files with binary content in Drive.
            field: published: Whether this revision is published. This is only
                applicable to Google Docs.
            field: publish_auto: Whether subsequent revisions will be
                automatically republished. This is only applicable to Google
                Docs.
            field: published_outside_domain: Whether this revision is published
                outside the domain. This is only applicable to Google Docs.
            field: last_modifying_user: The last user to modify this revision.
            field: original_filename: The original filename used to create this
                revision. This is only applicable to files with binary content
                in Drive.
            field: md5_checksum: The MD5 checksum of the revision's content.
                This is only applicable to files with binary content in Drive.
            field: size: The size of the revision's content in bytes. This is
                only applicable to files with binary content in Drive.
        */

    public:
        RevisionResource():
        m_kind("drive#revision")
        {};

        RevisionResource(const QString& arg):
        m_kind("drive#revision")
        { m_id = arg; };
        virtual ~RevisionResource(){};

    public:
            /**
                The ID of the revision.
            */
        QString id()const{return m_id;};
        RevisionResource& setId(const QString& arg){m_id=arg;return *this;};

            /**
                Identifies what kind of resource this is. Value: the fixed
                string drive#revision.
            */
        QString kind()const{return m_kind;};
        RevisionResource& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                The MIME type of the revision.
            */
        QString mimetype()const{return m_mimeType;};
        RevisionResource& setMimetype(const QString& arg){m_mimeType=arg;return *this;};

            /**
                The last time the revision was modified  (RFC 3339 date-time).
            */
        QDateTime modifiedtime()const{return m_modifiedTime;};
        RevisionResource& setModifiedtime(const QDateTime& arg){m_modifiedTime=arg;return *this;};

            /**
                Whether to keep this revision forever, even if it is no longer
                the head revision. If not set, the revision will be
                automatically purged 30 days after newer content is uploaded.
                This can be set on a maximum of 200 revisions for a file. This
                field is only applicable to files with binary content in Drive.
            */
        bool keepforever()const{return m_keepForever;};
        RevisionResource& setKeepforever(const bool& arg){m_keepForever=arg;return *this;};

            /**
                Whether this revision is published. This is only applicable to
                Google Docs.
            */
        bool published()const{return m_published;};
        RevisionResource& setPublished(const bool& arg){m_published=arg;return *this;};

            /**
                Whether subsequent revisions will be automatically republished.
                This is only applicable to Google Docs.
            */
        bool publishauto()const{return m_publishAuto;};
        RevisionResource& setPublishauto(const bool& arg){m_publishAuto=arg;return *this;};

            /**
                Whether this revision is published outside the domain. This is
                only applicable to Google Docs.
            */
        bool publishedoutsidedomain()const{return m_publishedOutsideDomain;};
        RevisionResource& setPublishedoutsidedomain(const bool& arg){m_publishedOutsideDomain=arg;return *this;};

            /**
                The last user to modify this revision.
            */
        const RevisionUser& lastmodifyinguser()const{return m_lastModifyingUser;};
        RevisionResource& setLastmodifyinguser(const RevisionUser& arg){m_lastModifyingUser=arg;return *this;};

            /**
                The original filename used to create this revision. This is only
                applicable to files with binary content in Drive.
            */
        QString originalfilename()const{return m_originalFilename;};
        RevisionResource& setOriginalfilename(const QString& arg){m_originalFilename=arg;return *this;};

            /**
                The MD5 checksum of the revision's content. This is only
                applicable to files with binary content in Drive.
            */
        QString md5Checksum()const{return m_md5Checksum;};
        RevisionResource& setMd5Checksum(const QString& arg){m_md5Checksum=arg;return *this;};

            /**
                The size of the revision's content in bytes. This is only
                applicable to files with binary content in Drive.
            */
        quint64 size()const{return m_size;};
        RevisionResource& setSize(const quint64& arg){m_size=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<RevisionResource>  create(const QByteArray& data);
            static std::unique_ptr<RevisionResource>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<RevisionResource> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The ID of the revision.
            */
        QString m_id;

            /**
                Identifies what kind of resource this is. Value: the fixed
                string drive#revision.
            */
        QString m_kind;

            /**
                The MIME type of the revision.
            */
        QString m_mimeType;

            /**
                The last time the revision was modified  (RFC 3339 date-time).
            */
        QDateTime m_modifiedTime;

            /**
                Whether to keep this revision forever, even if it is no longer
                the head revision. If not set, the revision will be
                automatically purged 30 days after newer content is uploaded.
                This can be set on a maximum of 200 revisions for a file. This
                field is only applicable to files with binary content in Drive.
            */
        bool m_keepForever;

            /**
                Whether this revision is published. This is only applicable to
                Google Docs.
            */
        bool m_published;

            /**
                Whether subsequent revisions will be automatically republished.
                This is only applicable to Google Docs.
            */
        bool m_publishAuto;

            /**
                Whether this revision is published outside the domain. This is
                only applicable to Google Docs.
            */
        bool m_publishedOutsideDomain;

            /**
                The last user to modify this revision.
            */
        RevisionUser m_lastModifyingUser;

            /**
                The original filename used to create this revision. This is only
                applicable to files with binary content in Drive.
            */
        QString m_originalFilename;

            /**
                The MD5 checksum of the revision's content. This is only
                applicable to files with binary content in Drive.
            */
        QString m_md5Checksum;

            /**
                The size of the revision's content in bytes. This is only
                applicable to files with binary content in Drive.
            */
        quint64 m_size = {0};

    };//RevisionResource

}//revisions
}//googleQt
