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
    class LocationData{
        /**
            field: latitude: latitude
            field: longitude: longitude
            field: altitude: altitude
        */

    public:
        LocationData(){};

        LocationData(const float& arg){ m_latitude = arg; };
        virtual ~LocationData(){};

    public:
            /**
                latitude
            */
        float latitude()const{return m_latitude;};
        LocationData& setLatitude(const float& arg){m_latitude=arg;return *this;};

            /**
                longitude
            */
        float longitude()const{return m_longitude;};
        LocationData& setLongitude(const float& arg){m_longitude=arg;return *this;};

            /**
                altitude
            */
        float altitude()const{return m_altitude;};
        LocationData& setAltitude(const float& arg){m_altitude=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<LocationData>  create(const QByteArray& data);
            static std::unique_ptr<LocationData>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<LocationData> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                latitude
            */
        float m_latitude;

            /**
                longitude
            */
        float m_longitude;

            /**
                altitude
            */
        float m_altitude;

    };//LocationData

}//files
}//googleQt
