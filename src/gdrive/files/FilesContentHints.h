/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "gdrive/files/FilesThumbnailInfo.h"

namespace googleQt{
namespace files{
    class ContentHints{
        /**
            field: thumbnail: A thumbnail for a file.
            field: indexable_text: Text to be indexed for the file to improve
                fullText queries. This is limited to 128KB in length and may
                contain HTML elements.
        */

    public:
        ContentHints(){};

        ContentHints(const ThumbnailInfo& arg){ m_thumbnail = arg; };

    public:
            /**
                A thumbnail for a file.
            */
        const ThumbnailInfo& thumbnail()const{return m_thumbnail;};
        ContentHints& setThumbnail(const ThumbnailInfo& arg){m_thumbnail=arg;return *this;};

            /**
                Text to be indexed for the file to improve fullText queries.
                This is limited to 128KB in length and may contain HTML
                elements.
            */
        QString indexabletext()const{return m_indexableText;};
        ContentHints& setIndexabletext(const QString& arg){m_indexableText=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<ContentHints>  create(const QByteArray& data);
            static std::unique_ptr<ContentHints>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<ContentHints> EXAMPLE(int context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                A thumbnail for a file.
            */
        ThumbnailInfo m_thumbnail;

            /**
                Text to be indexed for the file to improve fullText queries.
                This is limited to 128KB in length and may contain HTML
                elements.
            */
        QString m_indexableText;

    };//ContentHints

}//files
}//googleQt
