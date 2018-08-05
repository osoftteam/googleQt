/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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

        VideoMediaMetadata(const quint64& arg){ m_width = arg; };
        virtual ~VideoMediaMetadata(){};

    public:
            /**
                The width of the video in pixels.
            */
        quint64 width()const{return m_width;};
        VideoMediaMetadata& setWidth(const quint64& arg){m_width=arg;return *this;};

            /**
                The height of the video in pixels.
            */
        quint64 height()const{return m_height;};
        VideoMediaMetadata& setHeight(const quint64& arg){m_height=arg;return *this;};

            /**
                The duration of the video in milliseconds.
            */
        quint64 durationmillis()const{return m_durationMillis;};
        VideoMediaMetadata& setDurationmillis(const quint64& arg){m_durationMillis=arg;return *this;};

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
        static std::unique_ptr<VideoMediaMetadata> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The width of the video in pixels.
            */
        quint64 m_width = {0};

            /**
                The height of the video in pixels.
            */
        quint64 m_height = {0};

            /**
                The duration of the video in milliseconds.
            */
        quint64 m_durationMillis = {0};

    };//VideoMediaMetadata

}//files
}//googleQt
