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
            void setFullName(QString s) { m_fullName = s;}

            /**
                set given name
            */
            void setGivenName(QString s) { m_givenName = s;}

            /**
            set family name
            */
            void setFamilyName(QString s) { m_familyName = s;}

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

            void setName(QString s) { m_name = s; }
            void setTitle(QString s) { m_title = s; }
            void setTypeLabel(QString s) { m_type_label = s; }

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

            void setCity(QString s) { m_city = s; }
            void setStreet(QString s) { m_street = s; }
            void setRegion(QString s) { m_region = s; }
            void setPostcode(QString s) { m_postcode = s; }
            void setCountry(QString s) { m_country = s; }
            void setFormattedAddress(QString s) { m_formattedAddress = s; }
            void setTypeLabel(QString s) { m_type_label = s; }
            void setPrimary(bool val){ m_is_primary = val; }

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
            void setNumber(QString s) { m_number = s; };
            /**
                set phone uri
            */
            void setUri(QString s) { m_uri = s; };
            /**
                set phone label - other, home, work
            */
            void setTypeLabel(QString s) { m_type_label = s; };
            /**
                set primary property
            */
            void setPrimary(bool val){ m_is_primary = val; }

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
            void setAddress(QString val){ m_address = val;};
            /**
                set display name
            */
            void setDisplayName(QString val){ m_display_name = val;};

            /**
                set label type - other, home, work
            */
            void setTypeLabel(QString val){ m_type_label = val;};

            /**
                set primary type
            */
            void setPrimary(bool val) { m_is_primary = val;}
            
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
            GroupMembershipInfo(QString userId, QString groupId);

            QString userId()const { return m_userId; }
            void setUserId(QString uId) { m_userId = uId;}

            QString groupId()const{return m_groupId;}
            void setGroupId(QString gId){m_groupId = gId;}

            bool isDeleted()const{return m_is_deleted;}

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
            custom field/value pair
        */
        class UserDefinedFieldInfo : public NullablePart 
        {
        public:
            UserDefinedFieldInfo();
            UserDefinedFieldInfo(QString key, QString value);

            QString     key()const { return m_key; }            
            QString     value()const { return m_value; }

            void        setKey(QString s) { m_key = s; }
            void        setValue(QString s) { m_value = s; }

            bool operator==(const UserDefinedFieldInfo&) const;
            bool operator!=(const UserDefinedFieldInfo&) const;

            QString toString()const;
        protected:
            QString m_key;
            QString m_value;
            friend class UserDefinedFieldInfoList;
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

        /**
        list of user defiled fields
        */
        class UserDefinedFieldInfoList : public PartList<UserDefinedFieldInfo>
        {
        public:
            static UserDefinedFieldInfoList parse(QDomNode n);
            QString toXmlString()const;
            void toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const;

        protected:
            friend class ContactInfo;
        };
    };
};
