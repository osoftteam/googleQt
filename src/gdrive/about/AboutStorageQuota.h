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
    class StorageQuota{
        /**
            field: limit: The usage limit, if applicable. This will not be
                present if the user has unlimited storage.
            field: usage: The total usage across all services.
            field: usage_in_drive: The usage by all files in Google Drive.
            field: usage_in_drive_trash: The usage by trashed files in Google
                Drive.
            field: max_upload_size: The maximum upload size in bytes.
            field: app_installed: Whether the user has installed the requesting
                app.
        */

    public:
        StorageQuota(){};

        StorageQuota(const int& arg){ m_limit = arg; };

    public:
            /**
                The usage limit, if applicable. This will not be present if the
                user has unlimited storage.
            */
        int limit()const{return m_limit;};
        StorageQuota& setLimit(const int& arg){m_limit=arg;return *this;};

            /**
                The total usage across all services.
            */
        int usage()const{return m_usage;};
        StorageQuota& setUsage(const int& arg){m_usage=arg;return *this;};

            /**
                The usage by all files in Google Drive.
            */
        int usageindrive()const{return m_usageInDrive;};
        StorageQuota& setUsageindrive(const int& arg){m_usageInDrive=arg;return *this;};

            /**
                The usage by trashed files in Google Drive.
            */
        int usageindrivetrash()const{return m_usageInDriveTrash;};
        StorageQuota& setUsageindrivetrash(const int& arg){m_usageInDriveTrash=arg;return *this;};

            /**
                The maximum upload size in bytes.
            */
        int maxuploadsize()const{return m_maxUploadSize;};
        StorageQuota& setMaxuploadsize(const int& arg){m_maxUploadSize=arg;return *this;};

            /**
                Whether the user has installed the requesting app.
            */
        bool appinstalled()const{return m_appInstalled;};
        StorageQuota& setAppinstalled(const bool& arg){m_appInstalled=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<StorageQuota>  create(const QByteArray& data);
            static std::unique_ptr<StorageQuota>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<StorageQuota> EXAMPLE();
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The usage limit, if applicable. This will not be present if the
                user has unlimited storage.
            */
        int m_limit = {0};

            /**
                The total usage across all services.
            */
        int m_usage = {0};

            /**
                The usage by all files in Google Drive.
            */
        int m_usageInDrive = {0};

            /**
                The usage by trashed files in Google Drive.
            */
        int m_usageInDriveTrash = {0};

            /**
                The maximum upload size in bytes.
            */
        int m_maxUploadSize = {0};

            /**
                Whether the user has installed the requesting app.
            */
        bool m_appInstalled;

    };//StorageQuota

}//about
}//googleQt
