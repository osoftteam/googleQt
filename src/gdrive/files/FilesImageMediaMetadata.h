/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "gdrive/files/FilesLocationData.h"

namespace googleQt{
namespace files{
    class ImageMediaMetadata{
        /**
            field: width: The width of the image in pixels.
            field: height: The height of the image in pixels.
            field: rotation: The rotation in clockwise degrees from the image's
                original orientation.
            field: location: Geographic location information stored in the
                image.
            field: time: The date and time the photo was taken (EXIF DateTime).
            field: camera_make: The make of the camera used to create the photo.
            field: camera_model: The model of the camera used to create the
                photo.
            field: exposure_time: The length of the exposure, in seconds.
            field: flash_used: Whether a flash was used to create the photo.
            field: focal_length: The focal length used to create the photo, in
                millimeters.
            field: iso_speed: The ISO speed used to create the photo.
            field: metering_mode: The metering mode used to create the photo.
            field: sensor: The type of sensor used to create the photo.
            field: exposure_mode: The exposure mode used to create the photo.
            field: color_space: The color space of the photo.
            field: white_balance: The white balance mode used to create the
                photo.
            field: exposure_bias: The exposure bias of the photo (APEX value).
            field: max_aperture_value: The smallest f-number of the lens at the
                focal length used to create the photo (APEX value).
            field: subject_distance: The distance to the subject of the photo,
                in meters.
            field: lens: The lens used to create the photo.
        */

    public:
        ImageMediaMetadata(){};

        ImageMediaMetadata(const quint64& arg){ m_width = arg; };

    public:
            /**
                The width of the image in pixels.
            */
        quint64 width()const{return m_width;};
        ImageMediaMetadata& setWidth(const quint64& arg){m_width=arg;return *this;};

            /**
                The height of the image in pixels.
            */
        quint64 height()const{return m_height;};
        ImageMediaMetadata& setHeight(const quint64& arg){m_height=arg;return *this;};

            /**
                The rotation in clockwise degrees from the image's original
                orientation.
            */
        quint64 rotation()const{return m_rotation;};
        ImageMediaMetadata& setRotation(const quint64& arg){m_rotation=arg;return *this;};

            /**
                Geographic location information stored in the image.
            */
        const LocationData& location()const{return m_location;};
        ImageMediaMetadata& setLocation(const LocationData& arg){m_location=arg;return *this;};

            /**
                The date and time the photo was taken (EXIF DateTime).
            */
        QString time()const{return m_time;};
        ImageMediaMetadata& setTime(const QString& arg){m_time=arg;return *this;};

            /**
                The make of the camera used to create the photo.
            */
        QString cameramake()const{return m_cameraMake;};
        ImageMediaMetadata& setCameramake(const QString& arg){m_cameraMake=arg;return *this;};

            /**
                The model of the camera used to create the photo.
            */
        QString cameramodel()const{return m_cameraModel;};
        ImageMediaMetadata& setCameramodel(const QString& arg){m_cameraModel=arg;return *this;};

            /**
                The length of the exposure, in seconds.
            */
        float exposuretime()const{return m_exposureTime;};
        ImageMediaMetadata& setExposuretime(const float& arg){m_exposureTime=arg;return *this;};

            /**
                Whether a flash was used to create the photo.
            */
        bool flashused()const{return m_flashUsed;};
        ImageMediaMetadata& setFlashused(const bool& arg){m_flashUsed=arg;return *this;};

            /**
                The focal length used to create the photo, in millimeters.
            */
        float focallength()const{return m_focalLength;};
        ImageMediaMetadata& setFocallength(const float& arg){m_focalLength=arg;return *this;};

            /**
                The ISO speed used to create the photo.
            */
        quint64 isospeed()const{return m_isoSpeed;};
        ImageMediaMetadata& setIsospeed(const quint64& arg){m_isoSpeed=arg;return *this;};

            /**
                The metering mode used to create the photo.
            */
        QString meteringmode()const{return m_meteringMode;};
        ImageMediaMetadata& setMeteringmode(const QString& arg){m_meteringMode=arg;return *this;};

            /**
                The type of sensor used to create the photo.
            */
        QString sensor()const{return m_sensor;};
        ImageMediaMetadata& setSensor(const QString& arg){m_sensor=arg;return *this;};

            /**
                The exposure mode used to create the photo.
            */
        QString exposuremode()const{return m_exposureMode;};
        ImageMediaMetadata& setExposuremode(const QString& arg){m_exposureMode=arg;return *this;};

            /**
                The color space of the photo.
            */
        QString colorspace()const{return m_colorSpace;};
        ImageMediaMetadata& setColorspace(const QString& arg){m_colorSpace=arg;return *this;};

            /**
                The white balance mode used to create the photo.
            */
        QString whitebalance()const{return m_whiteBalance;};
        ImageMediaMetadata& setWhitebalance(const QString& arg){m_whiteBalance=arg;return *this;};

            /**
                The exposure bias of the photo (APEX value).
            */
        float exposurebias()const{return m_exposureBias;};
        ImageMediaMetadata& setExposurebias(const float& arg){m_exposureBias=arg;return *this;};

            /**
                The smallest f-number of the lens at the focal length used to
                create the photo (APEX value).
            */
        float maxaperturevalue()const{return m_maxApertureValue;};
        ImageMediaMetadata& setMaxaperturevalue(const float& arg){m_maxApertureValue=arg;return *this;};

            /**
                The distance to the subject of the photo, in meters.
            */
        quint64 subjectdistance()const{return m_subjectDistance;};
        ImageMediaMetadata& setSubjectdistance(const quint64& arg){m_subjectDistance=arg;return *this;};

            /**
                The lens used to create the photo.
            */
        QString lens()const{return m_lens;};
        ImageMediaMetadata& setLens(const QString& arg){m_lens=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<ImageMediaMetadata>  create(const QByteArray& data);
            static std::unique_ptr<ImageMediaMetadata>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<ImageMediaMetadata> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The width of the image in pixels.
            */
        quint64 m_width = {0};

            /**
                The height of the image in pixels.
            */
        quint64 m_height = {0};

            /**
                The rotation in clockwise degrees from the image's original
                orientation.
            */
        quint64 m_rotation = {0};

            /**
                Geographic location information stored in the image.
            */
        LocationData m_location;

            /**
                The date and time the photo was taken (EXIF DateTime).
            */
        QString m_time;

            /**
                The make of the camera used to create the photo.
            */
        QString m_cameraMake;

            /**
                The model of the camera used to create the photo.
            */
        QString m_cameraModel;

            /**
                The length of the exposure, in seconds.
            */
        float m_exposureTime;

            /**
                Whether a flash was used to create the photo.
            */
        bool m_flashUsed;

            /**
                The focal length used to create the photo, in millimeters.
            */
        float m_focalLength;

            /**
                The ISO speed used to create the photo.
            */
        quint64 m_isoSpeed = {0};

            /**
                The metering mode used to create the photo.
            */
        QString m_meteringMode;

            /**
                The type of sensor used to create the photo.
            */
        QString m_sensor;

            /**
                The exposure mode used to create the photo.
            */
        QString m_exposureMode;

            /**
                The color space of the photo.
            */
        QString m_colorSpace;

            /**
                The white balance mode used to create the photo.
            */
        QString m_whiteBalance;

            /**
                The exposure bias of the photo (APEX value).
            */
        float m_exposureBias;

            /**
                The smallest f-number of the lens at the focal length used to
                create the photo (APEX value).
            */
        float m_maxApertureValue;

            /**
                The distance to the subject of the photo, in meters.
            */
        quint64 m_subjectDistance = {0};

            /**
                The lens used to create the photo.
            */
        QString m_lens;

    };//ImageMediaMetadata

}//files
}//googleQt
