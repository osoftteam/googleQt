#pragma once
#include <QSqlQuery>

#include "google/endpoint/ApiUtil.h"

/**
    GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD - lib will store structures as records in detail table, one row per structure
    One table will be used to store all kind of record parts, mutliple groups supported as well.
    Not used by default compilation, instead contact info is stored as two xml strings - original and modified
    On server update modified xml is merged with original xml.
*/
// #define GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD

namespace googleQt {
    class GcontactRoutes;

    namespace mail_cache {
        class GMailSQLiteStorage;
    };

    namespace gcontact {
        class GContactCache;

        using contact_cache_ptr = std::shared_ptr<GContactCache>;

        /**
           basic xml parsing product, can be invalid
         */
        class NullablePart 
        {
        public:
            bool isNull()const {return m_is_null;}

        protected:
            bool    m_is_null{ false };            
        };

        /**
            object can be stored as one record and dbID is primary key
        */
        class DbPersistant: public NullablePart
        {
        public:
            bool    isDbIdNull()const { return (m_db_id == -1); }
            int     dbID()const { return m_db_id; }
            void    setDbID(int v) { m_db_id = v; }
        protected:
            int     m_db_id{ -1 };
        };

        class ContactXmlPersistant : public DbPersistant 
        {
        public:
            enum EStatus
                {
                    localCopy = 1,
                    localModified,
                    localRemoved,
                    localRetired
                };

            ContactXmlPersistant();

            QString etag()const { return m_etag; }
            QString id()const { return m_id; }
            QString title()const { return m_title; }
            QString content()const { return m_content; }
            const QDateTime& updated()const { return m_updated; }

            EStatus status()const{return m_status;}
            void markAsClean() { m_status = localCopy; };
            void markAsModified() { m_status = localModified; };
            void markAsDeleted() { m_status = localRemoved; };

            QString  originalXml()const { return m_original_xml_string; }

            void updateDateTime();

            bool parseXml(const QByteArray & data);
            bool parseXml(const QString & xml);
            QString mergedXml(QString mergeOrigin)const;
            virtual bool parseEntryNode(QDomNode n) = 0;
            virtual void mergeEntryNode(QDomDocument& doc, QDomNode& entry_node) const = 0;

            /// int -> status enum, does some validation
            static EStatus validatedStatus(int val, bool* ok = nullptr);
        protected:
            QString             m_etag, m_id, m_title, m_content;
            QDateTime           m_updated;
            QString             m_original_xml_string;
            EStatus             m_status;
        };

#ifdef GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD
        /**
            object can be stored as series os records, each with it's own ID and label
        */
        class MRecordDbPersistant : public NullablePart 
        {
        public:
            using   ID2NAME = std::map<int, QString>;
            using   NAME2ID = std::map<QString, int>;
            /// one obj can have multiple records, they all will have same kind id
            virtual int  objKind()const = 0;
            virtual bool insertDb(QSqlQuery* q, std::function<void(QSqlQuery*)> header_binder, int group_idx) = 0;
        protected:
            void clearDbMaps();
            bool insertDbRecord(QSqlQuery* q, std::function<void (QSqlQuery*)> header_binder, int group_idx, QString recordName, QString recordValue);
            bool insertDbRecord(QSqlQuery* q, std::function<void(QSqlQuery*)> header_binder, int group_idx, QString recordName, bool recordValue);
        protected:
            ID2NAME m_id2name;
            NAME2ID m_name2id;
        };

        enum ContactPartKind 
        {
            pkindEmail = 1,
            pkindPhone = 2,
            pkindAddress = 3
        };
#else
        using MRecordDbPersistant = NullablePart;
#endif //GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD

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

#ifdef GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD
            int  objKind()const override { return pkindAddress; }
            bool insertDb(QSqlQuery* q, std::function<void(QSqlQuery*)> header_binder, int group_idx)override;
#endif
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

#ifdef GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD
            int  objKind()const override { return pkindPhone; }
            bool insertDb(QSqlQuery* q, std::function<void(QSqlQuery*)> header_binder, int group_idx)override;
#endif
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

#ifdef GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD
            int  objKind()const override { return pkindEmail; }
            bool insertDb(QSqlQuery* q, std::function<void(QSqlQuery*)> header_binder, int group_idx)override;
#endif

        protected:
            QString m_address;
            QString m_display_name;
            QString m_type_label;
            bool    m_is_primary{ false };
            friend class EmailInfoList;
        };

        template <class P>
        class PartList
        {
        public:
            QString toString()const 
            {
                QString s = "";
                for (auto& p : m_parts) {
                    s += QString(";") + p.toString();
                }
                return s;
            };

            bool operator==(const PartList<P>& o) const
            {
                if (m_parts.size() != o.m_parts.size())
                    return false;

                size_t Max = m_parts.size();
                for (size_t i = 0; i < Max; i++) {
                    if (m_parts[i] != o.m_parts[i])
                        return false;
                }

                return true;
            };

            bool operator!=(const PartList<P>& o) const
            {
                return !(*this == o);
            };

#ifdef GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD
            bool insertDb(QSqlQuery* q, std::function<void(QSqlQuery*)> header_binder) 
            {
                int idx = 1;
                for (auto& p : m_parts) {
                    if (!p.insertDb(q, header_binder, idx)) {
                        return false;
                    }
                    idx++;
                }
                return true;
            }
#endif

            size_t size()const{return m_parts.size();}
            const P& operator[](size_t idx)const{return m_parts[idx];}
            
        protected:
            std::vector<P> m_parts;
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

            bool parseEntryNode(QDomNode n)override;
            void mergeEntryNode(QDomDocument& doc, QDomNode& entry_node)const override;
            bool setFromDbRecord(QSqlQuery* q);

            QString toString()const;
            QString toXml(QString userEmail)const;
            
            bool operator==(const ContactInfo&) const;
            bool operator!=(const ContactInfo&) const;

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ContactInfo> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

#ifdef GOOGLE_QT_CONTACT_DB_STRUCT_AS_RECORD
            bool insertDbRecords(QSqlQuery* q, std::function<void(QSqlQuery*)> header_binder);
#endif

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

            QString toString()const;
            QString toXml(QString userEmail)const;
            bool parseEntryNode(QDomNode n)override;
            void mergeEntryNode(QDomDocument& doc, QDomNode& entry_node)const override;
            bool setFromDbRecord(QSqlQuery* q);

            bool operator==(const GroupInfo&) const;
            bool operator!=(const GroupInfo&) const;

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<GroupInfo> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST
        };

        template <class T>
        class InfoList 
        {
        public:
            void add(std::shared_ptr<T> c) {
                m_items.push_back(c);
            };

            void add(std::unique_ptr<T> c) {
                std::shared_ptr<T> c2(c.release());
                add(c2);
            };

            void clear() { m_items.clear(); };

            QString toString()const
            {
                QString rv;
                for (auto& c : items()) {
                    rv += c->toString();
                    rv += "\n";
                }
                return rv;
            };

            bool parseXml(const QByteArray & data)
            {
                QDomDocument doc;
                QString errorMsg = 0;
                int errorLine;
                int errorColumn;
                if (!doc.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
                    qWarning() << "Failed to parse contacts XML document: " << errorMsg << "line=" << errorLine << "column=" << errorColumn;
                    qWarning() << "-- begin data";
                    qWarning() << data;
                    qWarning() << "-- end data";
                    return false;
                }

                QDomNodeList entries = doc.elementsByTagName("entry");
                for (int i = 0; i < entries.size(); i++) {
                    QDomNode n = entries.item(i);
                    std::shared_ptr<T> ci(new T);
                    if (ci->parseEntryNode(n)) {
                        m_items.push_back(ci);
                    }
                }

                return true;
            };

            bool operator==(const InfoList<T>& o) const
            {
                if (m_items.size() != o.m_items.size())
                    return false;

                size_t Max = m_items.size();
                for (size_t i = 0; i < Max; i++) {
                    if (*(m_items[i]) != *(o.m_items[i]))
                        return false;
                }

                return true;
            };

            bool operator!=(const InfoList<T>& o) const
            {
                return !(*this == o);
            };

            const std::vector<std::shared_ptr<T>>& items()const { return m_items; }

        protected:
            std::vector<std::shared_ptr<T>> m_items;
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

        class GcontactCacheRoutes : public QObject
        {
            Q_OBJECT
        public:
            GcontactCacheRoutes(googleQt::Endpoint& endpoint, GcontactRoutes& gcontact_routes);

            contact_cache_ptr       cache() { return m_GContactsCache; }

            GoogleVoidTask* synchronizeContacts_Async();

#ifdef API_QT_AUTOTEST
            void runAutotest();
#endif
        protected:
            GoogleVoidTask* reloadCache_Async(GoogleVoidTask* rv);

        protected:
            Endpoint&           m_endpoint;
            contact_cache_ptr   m_GContactsCache;            
        };
    };//gcontact
};

