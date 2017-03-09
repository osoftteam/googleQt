/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"

namespace googleQt{
namespace files{
    class ThumbnailInfo{
        /**
            field: image: The thumbnail data encoded with URL-safe Base64 (RFC
                4648 section 5).
            field: mime_type: The MIME type of the thumbnail.
        */

    public:
        ThumbnailInfo(){};

        ThumbnailInfo(const QByteArray& arg){ m_image = arg; };

    public:
            /**
                The thumbnail data encoded with URL-safe Base64 (RFC 4648
                section 5).
            */
        QByteArray image()const{return m_image;};
        ThumbnailInfo& setImage(const QByteArray& arg){m_image=arg;return *this;};

            /**
                The MIME type of the thumbnail.
            */
        QString mimetype()const{return m_mimeType;};
        ThumbnailInfo& setMimetype(const QString& arg){m_mimeType=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<ThumbnailInfo>  create(const QByteArray& data);
            static std::unique_ptr<ThumbnailInfo>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<ThumbnailInfo> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The thumbnail data encoded with URL-safe Base64 (RFC 4648
                section 5).
            */
        QByteArray m_image;

            /**
                The MIME type of the thumbnail.
            */
        QString m_mimeType;

    };//ThumbnailInfo

}//files
}//googleQt
