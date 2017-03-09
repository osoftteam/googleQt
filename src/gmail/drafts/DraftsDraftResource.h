/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "drafts"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/messages/MessagesMessageResource.h"

namespace googleQt{
namespace drafts{
    class DraftResource{
        /**
            field: id: The immutable ID of the draft.
            field: message: The message content of the draft.
        */

    public:
        DraftResource(){};

        DraftResource(const QString& arg){ m_id = arg; };

    public:
            /**
                The immutable ID of the draft.
            */
        QString id()const{return m_id;};
        DraftResource& setId(const QString& arg){m_id=arg;return *this;};

            /**
                The message content of the draft.
            */
        const messages::MessageResource& message()const{return m_message;};
        DraftResource& setMessage(const messages::MessageResource& arg){m_message=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<DraftResource>  create(const QByteArray& data);
            static std::unique_ptr<DraftResource>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<DraftResource> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The immutable ID of the draft.
            */
        QString m_id;

            /**
                The message content of the draft.
            */
        messages::MessageResource m_message;

    };//DraftResource

}//drafts
}//googleQt
