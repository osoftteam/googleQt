/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "labels"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/labels/LabelsLabelResource.h"

namespace googleQt{
namespace labels{
    class LabelsResultList{

    public:
        LabelsResultList(){};

        LabelsResultList(const std::list <LabelResource>& arg){ m_labels = arg; };

    public:
        const std::list <LabelResource>& labels()const{return m_labels;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<LabelsResultList>  create(const QByteArray& data);
            static std::unique_ptr<LabelsResultList>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<LabelsResultList> EXAMPLE();
        #endif //API_QT_AUTOTEST


    protected:
        std::list <LabelResource> m_labels;

    };//LabelsResultList

}//labels
}//googleQt
