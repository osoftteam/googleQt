/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "contacts"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gcontact/GcontactRequestArg.h"
#include "gcontact/contacts/ContactsContactEntry.h"

namespace googleQt{
namespace contacts{
    class ContactCollectionRes{
        /**
            field: version: version
            field: entries: Collection of contacts.
        */

    public:
        ContactCollectionRes(){};

        ContactCollectionRes(const QString& arg){ m_version = arg; };

    public:
            /**
                version
            */
        QString version()const{return m_version;};
        ContactCollectionRes& setVersion(const QString& arg){m_version=arg;return *this;};

            /**
                Collection of contacts.
            */
        const std::list <ContactEntry>& entries()const{return m_entries;};
        ContactCollectionRes& setEntries(const std::list <ContactEntry>& arg){m_entries=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<ContactCollectionRes>  create(const QByteArray& data);
            static std::unique_ptr<ContactCollectionRes>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<ContactCollectionRes> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                version
            */
        QString m_version;

            /**
                Collection of contacts.
            */
        std::list <ContactEntry> m_entries;

    };//ContactCollectionRes

}//contacts
}//googleQt
