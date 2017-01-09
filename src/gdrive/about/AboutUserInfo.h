/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "about"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"

namespace googleQt{
namespace about{
    class UserInfo{
        /**
            field: display_name: A plain text displayable name for this user.
            field: kind: This is always drive#user.
            field: photo_link: A link to the user's profile photo, if available.
            field: me: Whether this user is the requesting user.
            field: permission_id: The user's ID as visible in Permission
                resources.
            field: email_address: The email address of the user. This may not be
                present in certain contexts if the user has not made their email
                address visible to the requester.
        */

    public:
        UserInfo():
        m_kind("drive#user")
        {};

        UserInfo(const QString& arg):
        m_kind("drive#user")
        { m_displayName = arg; };

    public:
            /**
                A plain text displayable name for this user.
            */
        QString displayname()const{return m_displayName;};
        UserInfo& setDisplayname(const QString& arg){m_displayName=arg;return *this;};

            /**
                This is always drive#user.
            */
        QString kind()const{return m_kind;};
        UserInfo& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                A link to the user's profile photo, if available.
            */
        QString photolink()const{return m_photoLink;};
        UserInfo& setPhotolink(const QString& arg){m_photoLink=arg;return *this;};

            /**
                Whether this user is the requesting user.
            */
        bool me()const{return m_me;};
        UserInfo& setMe(const bool& arg){m_me=arg;return *this;};

            /**
                The user's ID as visible in Permission resources.
            */
        QString permissionid()const{return m_permissionId;};
        UserInfo& setPermissionid(const QString& arg){m_permissionId=arg;return *this;};

            /**
                The email address of the user. This may not be present in
                certain contexts if the user has not made their email address
                visible to the requester.
            */
        QString emailaddress()const{return m_emailAddress;};
        UserInfo& setEmailaddress(const QString& arg){m_emailAddress=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<UserInfo>  create(const QByteArray& data);
            static std::unique_ptr<UserInfo>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<UserInfo> EXAMPLE(int context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                A plain text displayable name for this user.
            */
        QString m_displayName;

            /**
                This is always drive#user.
            */
        QString m_kind;

            /**
                A link to the user's profile photo, if available.
            */
        QString m_photoLink;

            /**
                Whether this user is the requesting user.
            */
        bool m_me;

            /**
                The user's ID as visible in Permission resources.
            */
        QString m_permissionId;

            /**
                The email address of the user. This may not be present in
                certain contexts if the user has not made their email address
                visible to the requester.
            */
        QString m_emailAddress;

    };//UserInfo

}//about
}//googleQt
