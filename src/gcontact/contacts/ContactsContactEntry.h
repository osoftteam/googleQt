/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "contacts"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gcontact/GcontactRequestArg.h"

namespace googleQt{
namespace contacts{
    class ContactEntry{
        /**
            field: id: Contact identifier.
        */

    public:
        ContactEntry(){};

        ContactEntry(const QString& arg){ m_id = arg; };

    public:
            /**
                Contact identifier.
            */
        QString id()const{return m_id;};
        ContactEntry& setId(const QString& arg){m_id=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<ContactEntry>  create(const QByteArray& data);
            static std::unique_ptr<ContactEntry>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<ContactEntry> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                Contact identifier.
            */
        QString m_id;

    };//ContactEntry

}//contacts
}//googleQt
