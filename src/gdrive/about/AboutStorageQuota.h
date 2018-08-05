/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "about"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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

        StorageQuota(const float& arg){ m_limit = arg; };
        virtual ~StorageQuota(){};

    public:
            /**
                The usage limit, if applicable. This will not be present if the
                user has unlimited storage.
            */
        float limit()const{return m_limit;};
        StorageQuota& setLimit(const float& arg){m_limit=arg;return *this;};

            /**
                The total usage across all services.
            */
        float usage()const{return m_usage;};
        StorageQuota& setUsage(const float& arg){m_usage=arg;return *this;};

            /**
                The usage by all files in Google Drive.
            */
        quint64 usageindrive()const{return m_usageInDrive;};
        StorageQuota& setUsageindrive(const quint64& arg){m_usageInDrive=arg;return *this;};

            /**
                The usage by trashed files in Google Drive.
            */
        quint64 usageindrivetrash()const{return m_usageInDriveTrash;};
        StorageQuota& setUsageindrivetrash(const quint64& arg){m_usageInDriveTrash=arg;return *this;};

            /**
                The maximum upload size in bytes.
            */
        quint64 maxuploadsize()const{return m_maxUploadSize;};
        StorageQuota& setMaxuploadsize(const quint64& arg){m_maxUploadSize=arg;return *this;};

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
        static std::unique_ptr<StorageQuota> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The usage limit, if applicable. This will not be present if the
                user has unlimited storage.
            */
        float m_limit;

            /**
                The total usage across all services.
            */
        float m_usage;

            /**
                The usage by all files in Google Drive.
            */
        quint64 m_usageInDrive = {0};

            /**
                The usage by trashed files in Google Drive.
            */
        quint64 m_usageInDriveTrash = {0};

            /**
                The maximum upload size in bytes.
            */
        quint64 m_maxUploadSize = {0};

            /**
                Whether the user has installed the requesting app.
            */
        bool m_appInstalled;

    };//StorageQuota

}//about
}//googleQt
