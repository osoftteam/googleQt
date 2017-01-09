/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "permissions"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"

namespace googleQt{
namespace permissions{
    class ResourcePermission{
        /**
            field: id: The ID of this permission. This is a unique identifier
                for the grantee, and is published in User resources as
                permissionId.
            field: kind: This is always drive#permission.
            field: type: The type of the grantee. Valid values are: user group
                domain anyone
            field: email_address: The email address of the user or group to
                which this permission refers.
            field: domain: The domain to which this permission refers.
            field: role: The role granted by this permission. Valid values are:
                owner writer commenter reader
            field: allow_file_discovery: Whether the permission allows the file
                to be discovered through search. This is only applicable for
                permissions of type domain or anyone.
            field: display_name: A displayable name for users, groups or
                domains.
            field: photo_link: A link to the user's profile photo, if available.
            field: expiration_time: The time at which this permission will
                expire (RFC 3339 date-time).
        */

    public:
        ResourcePermission():
        m_kind("drive#permission")
        {};

        ResourcePermission(const QString& arg):
        m_kind("drive#permission")
        { m_id = arg; };

    public:
            /**
                The ID of this permission. This is a unique identifier for the
                grantee, and is published in User resources as permissionId.
            */
        QString id()const{return m_id;};
        ResourcePermission& setId(const QString& arg){m_id=arg;return *this;};

            /**
                This is always drive#permission.
            */
        QString kind()const{return m_kind;};
        ResourcePermission& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                The type of the grantee. Valid values are: user group domain
                anyone
            */
        QString type()const{return m_type;};
        ResourcePermission& setType(const QString& arg){m_type=arg;return *this;};

            /**
                The email address of the user or group to which this permission
                refers.
            */
        QString emailaddress()const{return m_emailAddress;};
        ResourcePermission& setEmailaddress(const QString& arg){m_emailAddress=arg;return *this;};

            /**
                The domain to which this permission refers.
            */
        QString domain()const{return m_domain;};
        ResourcePermission& setDomain(const QString& arg){m_domain=arg;return *this;};

            /**
                The role granted by this permission. Valid values are: owner
                writer commenter reader
            */
        QString role()const{return m_role;};
        ResourcePermission& setRole(const QString& arg){m_role=arg;return *this;};

            /**
                Whether the permission allows the file to be discovered through
                search. This is only applicable for permissions of type domain
                or anyone.
            */
        bool allowfilediscovery()const{return m_allowFileDiscovery;};
        ResourcePermission& setAllowfilediscovery(const bool& arg){m_allowFileDiscovery=arg;return *this;};

            /**
                A displayable name for users, groups or domains.
            */
        QString displayname()const{return m_displayName;};
        ResourcePermission& setDisplayname(const QString& arg){m_displayName=arg;return *this;};

            /**
                A link to the user's profile photo, if available.
            */
        QString photolink()const{return m_photoLink;};
        ResourcePermission& setPhotolink(const QString& arg){m_photoLink=arg;return *this;};

            /**
                The time at which this permission will expire (RFC 3339
                date-time).
            */
        QDateTime expirationtime()const{return m_expirationTime;};
        ResourcePermission& setExpirationtime(const QDateTime& arg){m_expirationTime=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<ResourcePermission>  create(const QByteArray& data);
            static std::unique_ptr<ResourcePermission>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<ResourcePermission> EXAMPLE(int context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The ID of this permission. This is a unique identifier for the
                grantee, and is published in User resources as permissionId.
            */
        QString m_id;

            /**
                This is always drive#permission.
            */
        QString m_kind;

            /**
                The type of the grantee. Valid values are: user group domain
                anyone
            */
        QString m_type;

            /**
                The email address of the user or group to which this permission
                refers.
            */
        QString m_emailAddress;

            /**
                The domain to which this permission refers.
            */
        QString m_domain;

            /**
                The role granted by this permission. Valid values are: owner
                writer commenter reader
            */
        QString m_role;

            /**
                Whether the permission allows the file to be discovered through
                search. This is only applicable for permissions of type domain
                or anyone.
            */
        bool m_allowFileDiscovery;

            /**
                A displayable name for users, groups or domains.
            */
        QString m_displayName;

            /**
                A link to the user's profile photo, if available.
            */
        QString m_photoLink;

            /**
                The time at which this permission will expire (RFC 3339
                date-time).
            */
        QDateTime m_expirationTime;

    };//ResourcePermission

}//permissions
}//googleQt
