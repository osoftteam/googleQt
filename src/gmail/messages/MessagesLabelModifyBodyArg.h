/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"

namespace googleQt{
namespace messages{
    class LabelModifyBodyArg{

    public:
        LabelModifyBodyArg(){};

        LabelModifyBodyArg(const std::list <QString>& arg){ m_addLabels = arg; };

    public:
        const std::list <QString>& addlabels()const{return m_addLabels;};
        LabelModifyBodyArg& setAddlabels(const std::list <QString>& arg){m_addLabels=arg;return *this;};

        const std::list <QString>& removelabels()const{return m_removeLabels;};
        LabelModifyBodyArg& setRemovelabels(const std::list <QString>& arg){m_removeLabels=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<LabelModifyBodyArg>  create(const QByteArray& data);
            static std::unique_ptr<LabelModifyBodyArg>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<LabelModifyBodyArg> EXAMPLE();
        #endif //API_QT_AUTOTEST


    protected:
        std::list <QString> m_addLabels;

        std::list <QString> m_removeLabels;

    };//LabelModifyBodyArg

}//messages
}//googleQt
