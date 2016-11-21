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
    class VideoMediaMetadata{
        /**
            field: width: The width of the video in pixels.
            field: height: The height of the video in pixels.
            field: duration_millis: The duration of the video in milliseconds.
        */

    public:
        VideoMediaMetadata(){};

        VideoMediaMetadata(const int& arg){ m_width = arg; };

    public:
            /**
                The width of the video in pixels.
            */
        int width()const{return m_width;};
        VideoMediaMetadata& setWidth(const int& arg){m_width=arg;return *this;};

            /**
                The height of the video in pixels.
            */
        int height()const{return m_height;};
        VideoMediaMetadata& setHeight(const int& arg){m_height=arg;return *this;};

            /**
                The duration of the video in milliseconds.
            */
        int durationmillis()const{return m_durationMillis;};
        VideoMediaMetadata& setDurationmillis(const int& arg){m_durationMillis=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<VideoMediaMetadata>  create(const QByteArray& data);
            static std::unique_ptr<VideoMediaMetadata>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<VideoMediaMetadata> EXAMPLE();
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The width of the video in pixels.
            */
        int m_width = {0};

            /**
                The height of the video in pixels.
            */
        int m_height = {0};

            /**
                The duration of the video in milliseconds.
            */
        int m_durationMillis = {0};

    };//VideoMediaMetadata

}//files
}//googleQt
