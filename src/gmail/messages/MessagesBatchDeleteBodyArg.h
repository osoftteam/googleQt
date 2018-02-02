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
    class BatchDeleteBodyArg{

    public:
        BatchDeleteBodyArg(){};

        BatchDeleteBodyArg(const std::list <QString>& arg){ m_ids = arg; };
        virtual ~BatchDeleteBodyArg(){};

    public:
        const std::list <QString>& ids()const{return m_ids;};
        BatchDeleteBodyArg& setIds(const std::list <QString>& arg){m_ids=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<BatchDeleteBodyArg>  create(const QByteArray& data);
            static std::unique_ptr<BatchDeleteBodyArg>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<BatchDeleteBodyArg> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
        std::list <QString> m_ids;

    };//BatchDeleteBodyArg

}//messages
}//googleQt
