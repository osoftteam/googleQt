#pragma once
#include "GcontactCacheUtil.h"

namespace googleQt {
    namespace gcontact {
        /**
           name - full, given, family
         */
        class NameInfo: public NullablePart
        {    
        public:

            NameInfo();

            QString fullName()const { return m_fullName; };
            QString givenName()const { return m_givenName; };
            QString familyName()const { return m_familyName; };

            /**
                set full name
            */
            NameInfo& setFullName(QString s) { m_fullName = s; return *this; }

            /**
                set given name
            */
            NameInfo& setGivenName(QString s) { m_givenName = s; return *this; }

            /**
            set family name
            */
            NameInfo& setFamilyName(QString s) { m_familyName = s; return *this; }

            bool isEmpty()const override;

            static NameInfo parse(QDomNode n);
            QString toString()const;
            void toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const;
            bool operator==(const NameInfo&) const;
            bool operator!=(const NameInfo&) const;

        protected:
            QString m_fullName;
            QString m_givenName;
            QString m_familyName;
        };

        /**
            organization details - name, title
        */
        class OrganizationInfo : public NullablePart
        {
        public:
            OrganizationInfo();

            QString name()const { return m_name; }
            QString title()const { return m_title; }
            QString typeLabel()const { return m_type_label; };

            OrganizationInfo& setName(QString s) { m_name = s; return *this; }
            OrganizationInfo& setTitle(QString s) { m_title = s; return *this; }
            OrganizationInfo& setTypeLabel(QString s) { m_type_label = s; return *this; }

            bool isEmpty()const override;

            static OrganizationInfo parse(QDomNode n);
            QString toString()const;
            QString toXmlString()const;
            void toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const;
            bool operator==(const OrganizationInfo&) const;
            bool operator!=(const OrganizationInfo&) const;

        protected:
            QString m_name;
            QString m_title;
            QString m_type_label;
        };

        /**
            PostalAddress - city, street etc.
        */
        class PostalAddress : public NullablePart
        {
        public:

            PostalAddress();

            QString city()const { return m_city; }
            QString street()const { return m_street; }
            QString region()const { return m_region; }
            QString postcode()const { return m_postcode; }
            QString country()const { return m_country; }
            QString formattedAddress()const { return m_formattedAddress; }
            QString typeLabel()const { return m_type_label; };
            bool isPrimary()const { return m_is_primary; }

            PostalAddress& setCity(QString s) { m_city = s; return *this; }
            PostalAddress& setStreet(QString s) { m_street = s; return *this; }
            PostalAddress& setRegion(QString s) { m_region = s; return *this; }
            PostalAddress& setPostcode(QString s) { m_postcode = s; return *this; }
            PostalAddress& setCountry(QString s) { m_country = s; return *this; }
            PostalAddress& setFormattedAddress(QString s) { m_formattedAddress = s; return *this; }            
            PostalAddress& setTypeLabel(QString s) { m_type_label = s; return *this; }
            PostalAddress& setPrimary(bool val){ m_is_primary = val; return *this;}

            QString toString()const;
            bool operator==(const PostalAddress&) const;
            bool operator!=(const PostalAddress&) const;

        protected:
            QString m_city, m_street, m_region, m_postcode, m_country;
            QString m_type_label;
            QString m_formattedAddress;
            bool    m_is_primary{ false };
            friend class PostalAddressList;
        };

        /**
            single phone details
        */
        class PhoneInfo : public NullablePart
        {
        public:

            PhoneInfo();

            QString number()const { return m_number; };
            QString uri()const { return m_uri; };
            QString typeLabel()const { return m_type_label; };
            bool isPrimary()const { return m_is_primary; }

            /**
                set phone number
            */
            PhoneInfo& setNumber(QString s) { m_number = s; return *this; };
            /**
                set phone uri
            */
            PhoneInfo& setUri(QString s) { m_uri = s; return *this; };
            /**
                set phone label - other, home, work
            */
            PhoneInfo& setTypeLabel(QString s) { m_type_label = s; return *this; };
            /**
                set primary property
            */
            PhoneInfo& setPrimary(bool val){ m_is_primary = val; return *this; }

            QString toString()const;
            bool operator==(const PhoneInfo&) const;
            bool operator!=(const PhoneInfo&) const;            

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
        class EmailInfo : public NullablePart
        {
        public:
            EmailInfo();

            QString address()const { return m_address; };
            QString displayName()const { return m_display_name; };
            QString typeLabel()const { return m_type_label; };
            bool isPrimary()const { return m_is_primary; }

            /**
                set email address
            */
            EmailInfo& setAddress(QString val){ m_address = val; return *this; };
            /**
                set display name
            */
            EmailInfo& setDisplayName(QString val){ m_display_name = val; return *this; };

            /**
                set label type - other, home, work
            */
            EmailInfo& setTypeLabel(QString val){ m_type_label = val; return *this;};

            /**
                set primary type
            */
            EmailInfo& setPrimary(bool val) { m_is_primary = val;  return *this;}
            
            QString toString()const;
            bool operator==(const EmailInfo&) const;
            bool operator!=(const EmailInfo&) const;

        protected:
            QString m_address;
            QString m_display_name;
            QString m_type_label;
            bool    m_is_primary{ false };
            friend class EmailInfoList;
        };

        /**
        photo info
        */
        class PhotoInfo : public NullablePart
        {
        public:
            enum EStatus
                {
                    not_resolved = 0,
                    resolved = 1,
                    modified
                };
            static PhotoInfo parse(QDomNode n);

            QString href()const { return m_href; }
            QString etag()const { return m_etag; }
            EStatus status()const{return m_status;}

            bool operator==(const PhotoInfo&) const;
            bool operator!=(const PhotoInfo&) const;
            
        protected:
            void setupFromLocalDb(QString photo_href, QString photo_etag, EStatus st);

        protected:
            QString m_href;
            QString m_etag;
            EStatus m_status{not_resolved};
            friend class ContactInfo;
        };

        /**
           link against contact groups
         */
        class GroupMembershipInfo : public NullablePart
        {
        public:
            GroupMembershipInfo();

            GroupMembershipInfo(QString userId, QString groupId, bool isDeleted);

            QString userId()const { return m_userId; }
            GroupMembershipInfo& setUserId(QString uId) { m_userId = uId; return *this; }

            QString groupId()const{return m_groupId;}
            GroupMembershipInfo& setGroupId(QString gId){m_groupId = gId;return *this;}

            bool isDeleted()const{return m_is_deleted;}
            GroupMembershipInfo& setDeleted(bool val){m_is_deleted = val;return *this;}

            bool operator==(const GroupMembershipInfo&) const;
            bool operator!=(const GroupMembershipInfo&) const;

            QString toString()const;
        protected:
            QString m_userId;
            QString m_groupId;
            bool    m_is_deleted{false};
            friend class GroupMembershipInfoList;
        };
        
        /**
        list of emails
        */
        class EmailInfoList : public PartList<EmailInfo>
        {
        public:
            static EmailInfoList parse(QDomNode n);
            QString toXmlString()const;
            void toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const;

        protected:
            friend class ContactInfo;
        };

        /**
            list of phones
        */
        class PhoneInfoList : public PartList<PhoneInfo>
        {
        public:
            static PhoneInfoList parse(QDomNode n);
            QString toXmlString()const;
            void toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const;
        protected:
            friend class ContactInfo;
        };

        /**
        list of postal addresses
        */
        class PostalAddressList : public PartList<PostalAddress>
        {
        public:
            using ADDRESS_ARR = std::vector<PostalAddress>;

            static PostalAddressList parse(QDomNode n);
            QString toXmlString()const;
            void toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const;
        protected:
            friend class ContactInfo;
        };


        /**
        list of group links
        */
        class GroupMembershipInfoList : public PartList<GroupMembershipInfo>
        {
        public:
            static GroupMembershipInfoList parse(QDomNode n);
            QString toXmlString()const;
            void toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const;

        protected:
            friend class ContactInfo;
        };

    };
};
