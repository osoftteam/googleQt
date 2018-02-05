#pragma once
#include "GcontactCacheUtil.h"



namespace googleQt {
    class GcontactRoutes;

    namespace mail_cache {
        class GMailSQLiteStorage;
    };

    namespace gcontact {
        class GContactCache;

        using contact_cache_ptr = std::shared_ptr<GContactCache>;

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
        class PostalAddress : public MRecordDbPersistant
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
        class PhoneInfo : public MRecordDbPersistant
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
        class EmailInfo : public MRecordDbPersistant
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
            single contact entry
        */
        class ContactInfo : public ContactXmlPersistant
        {
        public:

            using ptr = std::shared_ptr<ContactInfo>;

            const EmailInfoList&       emails()const { return m_emails; }
            const PhoneInfoList&       phones()const { return m_phones; }
            const NameInfo&            name()const { return m_name; }
            const OrganizationInfo&    organization()const { return m_organization; }
            const PostalAddressList&   addresses()const { return m_address_list; }            

            /**
                set title
            */
            ContactInfo& setTitle(QString val);

            /**
                set notes
            */
            ContactInfo& setContent(QString notes);

            /**
                set name details
            */
            ContactInfo& setName(const NameInfo& n);

            /**
                add new email account
            */
            ContactInfo& addEmail(const EmailInfo& e);

            /**
                delete old emails and add new
            */
            ContactInfo& replaceEmails(const std::list<EmailInfo>& lst);

            /**
                add new phone info
            */
            ContactInfo& addPhone(const PhoneInfo& p);

            /**
                delete old phone and add new
            */
            ContactInfo& replacePhones(const std::list<PhoneInfo>& lst);

            /**
                set organization information
            */
            ContactInfo& setOrganizationInfo(const OrganizationInfo& o);

            /**
                add new address info
            */
            ContactInfo& addAddress(const PostalAddress& p);

            /**
                delete old address list and put a new one
            */
            ContactInfo& replaceAddressList(const std::list<PostalAddress>& lst);

            static ContactInfo* createWithId(QString contact_id);

            bool parseEntryNode(QDomNode n)override;
            void mergeEntryNode(QDomDocument& doc, QDomNode& entry_node)const override;
            bool setFromDbRecord(QSqlQuery* q);

            QString toString()const;
            QString toXml(QString userEmail)const;
            
            void assignContent(const ContactInfo& src);

            bool operator==(const ContactInfo&) const;
            bool operator!=(const ContactInfo&) const;

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ContactInfo> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            NameInfo            m_name;
            OrganizationInfo    m_organization;
            EmailInfoList       m_emails;
            PhoneInfoList       m_phones;            
            PostalAddressList   m_address_list;            
        };

        /**
            single contact group
        */
        class GroupInfo : public ContactXmlPersistant
        {
        public:

            using ptr = std::shared_ptr<GroupInfo>;

            /**
            set title
            */
            GroupInfo& setTitle(QString val);

            /**
            set notes
            */
            GroupInfo& setContent(QString notes);

            static GroupInfo* createWithId(QString group_id);

            QString toString()const;
            QString toXml(QString userEmail)const;
            bool parseEntryNode(QDomNode n)override;
            void mergeEntryNode(QDomDocument& doc, QDomNode& entry_node)const override;
            bool setFromDbRecord(QSqlQuery* q);

            bool operator==(const GroupInfo&) const;
            bool operator!=(const GroupInfo&) const;

            void assignContent(const GroupInfo& src);

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<GroupInfo> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST
        };

        /**
            list of all contacts
        */
        class ContactList : public InfoList<ContactInfo>{};
        /**
        list of all contact groups
        */
        class GroupList : public InfoList<GroupInfo> {};

        /**
            contacts DB storage
        */
        class GContactCache
        {
        public:
            GContactCache(ApiEndpoint& ept);

            ContactList& contacts() { return m_contacts; }
            const ContactList& contacts()const { return m_contacts; }

            GroupList& groups() { return m_groups; }
            const GroupList& groups()const { return m_groups; }

            const QDateTime& lastSyncTime()const { return m_sync_time; }

            void attachSQLStorage(std::shared_ptr<mail_cache::GMailSQLiteStorage> ss);

            bool storeContactsToDb();
            bool loadContactsFromDb();
            bool clearDbCache();
            bool mergeContactsAndStoreToDb(GroupList& server_glist, ContactList& server_clist);
        protected:
            bool ensureContactTables();
            bool storeContactGroups();
            bool storeContactEntries();
            bool storeConfig();

            bool loadContactGroupsFromDb();
            bool loadContactEntriesFromDb();
            bool loadContactConfigFromDb();

            void mergeEntries(ContactList& entry_list);
            void mergeGroups(GroupList& group_list);

        protected:
            std::shared_ptr<mail_cache::GMailSQLiteStorage> m_sql_storage;
            ContactList m_contacts;
            GroupList m_groups;
            std::map<QString, QString> m_configs;
            QDateTime m_sync_time;
            ApiEndpoint& m_endpoint;

            friend class mail_cache::GMailSQLiteStorage;
            friend class GcontactCacheRoutes;
        };

        class GcontactCacheQueryTask : public GoogleVoidTask 
        {
        public:
            contact_cache_ptr       cache() { return m_cache; }
        protected:
            GcontactCacheQueryTask(ApiEndpoint& ept, contact_cache_ptr c) :GoogleVoidTask(ept), m_cache(c) {}        
            contact_cache_ptr m_cache;
            std::unique_ptr<ContactList> m_result_contacts;
            std::unique_ptr<GroupList> m_result_groups;
            friend class GcontactCacheRoutes;
        };

        class GcontactCacheRoutes : public QObject
        {
            Q_OBJECT
        public:
            GcontactCacheRoutes(googleQt::Endpoint& endpoint, GcontactRoutes& gcontact_routes);

            contact_cache_ptr       cache() { return m_GContactsCache; }

            GcontactCacheQueryTask* synchronizeContacts_Async();

#ifdef API_QT_AUTOTEST
            void runAutotest();
#endif
        protected:
            GcontactCacheQueryTask* reloadCache_Async(GcontactCacheQueryTask* rv, QDateTime dtUpdatedMin);

        protected:
            Endpoint&           m_endpoint;
            contact_cache_ptr   m_GContactsCache;            
        };
    };//gcontact
};

