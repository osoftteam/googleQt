#pragma once
#include <QDomDocument>
#include <QDomNodeList>

#include "google/endpoint/ApiUtil.h"

namespace googleQt {
    namespace gcontact {
        
        /**
           basic xml parsing product, can be invalid
         */
        class ContactInfoPart 
        {
        public:
            operator bool()const { return !m_is_null; }
            bool isNull()const {return m_is_null;}

        protected:
            bool    m_is_null{ true };
        };

        /**
           name - full, given, family
         */
        class NameInfo: public ContactInfoPart
        {    
        public:

            QString fullName()const { return m_fullName; };
            QString givenName()const { return m_givenName; };
            QString familyName()const { return m_familyName; };


            static NameInfo parse(QDomNode n);
            operator QString ()const;
            QString toString()const;

        protected:
            QString m_fullName;
            QString m_givenName;
            QString m_familyName;
        };

        /**
            organization details - name, title
        */
        class OrganizationInfo : public ContactInfoPart
        {
        public:
            QString name()const { return m_name; }
            QString title()const { return m_title; }

            static OrganizationInfo parse(QDomNode n);
            operator QString ()const;
            QString toString()const;

        protected:
            QString m_name;
            QString m_title;
        };

        /**
            single phone details
        */
        class PhoneInfo : public ContactInfoPart
        {
        public:

            QString number()const { return m_number; };
            QString uri()const { return m_uri; };
            QString typeLabel()const { return m_type_label; };
            bool isPrimary()const { return m_is_primary; }

            operator QString ()const;
            QString toString()const;

        protected:
            QString m_number;
            QString m_uri;
            QString m_type_label;
            bool    m_is_primary{false};
            friend class PhoneInfoList;
        };

        /**
        single email details
        */
        class EmailInfo : public ContactInfoPart
        {
        public:

            QString address()const { return m_address; };
            QString displayName()const { return m_display_name; };
            QString typeLabel()const { return m_type_label; };
            bool isPrimary()const { return m_is_primary; }

            operator QString ()const;
            QString toString()const;

        protected:
            QString m_address;
            QString m_display_name;
            QString m_type_label;
            bool    m_is_primary{ false };
            friend class EmailInfoList;
        };

        /**
        list of emails
        */
        class EmailInfoList : public ContactInfoPart
        {
        public:
            using EMAILS_ARR = std::vector<EmailInfo>;

            static EmailInfoList parse(QDomNode n);
            operator QString ()const;
            QString toString()const;
            QString toXmlString()const;
        protected:
            EMAILS_ARR m_emails;
        };

        /**
            list of phones
        */
        class PhoneInfoList : public ContactInfoPart
        {
        public:
            using PHONES_ARR = std::vector<PhoneInfo>;

            static PhoneInfoList parse(QDomNode n);
            operator QString ()const;
            QString toString()const;
            QString toXmlString()const;
        protected:
            PHONES_ARR m_phones;
        };

        /**
            single contact entry
        */
        class ContactInfo : public ContactInfoPart
        {
        public:

            QString etag()const { return m_etag; }
            QString id()const { return m_id; }
            QString title()const { return m_title; }
            QString content()const { return m_content; }
            const QDateTime& updated()const { return m_updated; }

            const PhoneInfoList&       phones()const { return m_phones; }
            const NameInfo&            name()const { return m_name; }
            const OrganizationInfo&    organization()const { return m_organization; }

            bool parse(QDomNode n);
            operator QString ()const;
            QString toString()const;
            QString toXmlString()const;
            bool parseXml(const QByteArray & data);

        protected:
            QString m_etag, m_id, m_title, m_content;
            QDateTime           m_updated;
            EmailInfoList       m_emails;
            PhoneInfoList       m_phones;
            NameInfo            m_name;
            OrganizationInfo    m_organization;
        };

        /**
            list of all contacts
        */
        class ContactList
        {
        public:
            using CONTACTS_ARR = std::vector<ContactInfo>;

            const CONTACTS_ARR& contacts()const { return m_contacts; }

            bool parseXml(const QByteArray & data);
            QString toString(bool multiline = true)const;

        protected:
            CONTACTS_ARR m_contacts;
        };        

    };//gcontact
};

QDebug operator << (QDebug d, const googleQt::gcontact::ContactList &lst);
std::ostream &operator<<(std::ostream &os, const googleQt::gcontact::ContactList& lst);