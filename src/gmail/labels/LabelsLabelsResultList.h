/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "labels"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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

        LabelsResultList(const std::vector<LabelResource>& arg){ m_labels = arg; };
        virtual ~LabelsResultList(){};

    public:
        const std::vector<LabelResource>& labels()const{return m_labels;};

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
        static std::unique_ptr<LabelsResultList> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
        std::vector<LabelResource> m_labels;

    };//LabelsResultList

}//labels
}//googleQt
