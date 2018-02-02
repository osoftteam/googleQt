/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "history"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/messages/MessagesMessageResource.h"

namespace googleQt{
namespace history{
    class LabelHistoryRecord{
        /**
            field: message: Message resource
            field: label_ids: Labels list
        */

    public:
        LabelHistoryRecord(){};

        LabelHistoryRecord(const messages::MessageResource& arg){ m_message = arg; };
        virtual ~LabelHistoryRecord(){};

    public:
            /**
                Message resource
            */
        const messages::MessageResource& message()const{return m_message;};
        LabelHistoryRecord& setMessage(const messages::MessageResource& arg){m_message=arg;return *this;};

            /**
                Labels list
            */
        const std::list <QString>& labelids()const{return m_labelIds;};
        LabelHistoryRecord& setLabelids(const std::list <QString>& arg){m_labelIds=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<LabelHistoryRecord>  create(const QByteArray& data);
            static std::unique_ptr<LabelHistoryRecord>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<LabelHistoryRecord> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                Message resource
            */
        messages::MessageResource m_message;

            /**
                Labels list
            */
        std::list <QString> m_labelIds;

    };//LabelHistoryRecord

}//history
}//googleQt
