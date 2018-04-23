#pragma once
#include "GcontactCacheUtil.h"
#include "GcontactParts.h"


namespace googleQt {
    class GcontactRoutes;

    namespace mail_cache {
        class GMailSQLiteStorage;
    };

    namespace gcontact {
        class GContactCache;
        class BatchRequestContactInfo;
        class BatchRequestGroupInfo;

        using contact_cache_ptr = std::shared_ptr<GContactCache>;


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
            const PhotoInfo&           photo()const { return m_photo; }
            const GroupMembershipInfoList& groups()const { return m_groups; }

            /// Ref-function return non-const references and can be used
            /// to modify objects. call ContactInfo::markAsModified to stage
            /// object ready for storing into database and storeContactsToDb to save
            EmailInfoList&       emailsRef(){ return m_emails; }
            PhoneInfoList&       phonesRef(){ return m_phones; }
            NameInfo&            nameRef(){ return m_name; }
            OrganizationInfo&    organizationRef(){ return m_organization; }
            PostalAddressList&   addressesRef(){ return m_address_list; }

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
            add group
            */
            ContactInfo& addGroup(const GroupMembershipInfo& p);

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

            std::unique_ptr<BatchRequestContactInfo> buildBatchRequest(googleQt::EBatchId batch_id);

            void markPhotoAsResolved();
            void markPhotoAsModified();
            bool isPhotoResolved()const;
            bool isPhotoModified()const;

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ContactInfo> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            virtual QString toXmlBegin()const;

        protected:
            NameInfo            m_name;
            OrganizationInfo    m_organization;
            EmailInfoList       m_emails;
            PhoneInfoList       m_phones;            
            PostalAddressList   m_address_list;            
            PhotoInfo           m_photo;
            GroupMembershipInfoList m_groups;
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

            std::unique_ptr<BatchRequestGroupInfo> buildBatchRequest(googleQt::EBatchId batch_id);

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<GroupInfo> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            virtual QString toXmlBegin()const;

        };

        class BatchRequestContactInfo : public ContactInfo,
                                        public BatchRequest
        {
        public:
            static std::unique_ptr<BatchRequestContactInfo> buildRequest(QString contact_id, googleQt::EBatchId batch_id);
        protected:
            QString toXmlBegin()const override;

        };

        class BatchRequestGroupInfo :   public GroupInfo,
                                        public BatchRequest
        {
        public:
            static std::unique_ptr<BatchRequestGroupInfo> buildRequest(QString group_id, googleQt::EBatchId batch_id);
        protected:
            QString toXmlBegin()const override;
        };

        class BatchResultContactInfo : public ContactInfo,
                                       public BatchResult
        {
        public:
            bool parseEntryNode(QDomNode n)override;
        };

        class BatchResultGroupInfo : public GroupInfo,
                                    public BatchResult
        {
        public:
            bool parseEntryNode(QDomNode n)override;
        };

        ///contacts batch request
        class BatchRequesContactList : public InfoList<BatchRequestContactInfo> {};

        ///group batch request
        class BatchRequesGroupList : public InfoList<BatchRequestGroupInfo> {};


        ///list of all contacts
        class ContactList : public BatchBuilderInfoList<ContactInfo, BatchRequesContactList>
        {
        public:
            /// this is special case of 'limbo' contact - we created it locally, it has DBID, we sent requiest
            /// to server and server assigned cloudID - we have to link cloudID to the contact and update local cache
            ContactInfo::ptr findNewCreatedContact(std::shared_ptr<BatchResultContactInfo> b);
            
            std::list<QString> buildUnresolvedPhotoIdList();
            std::list<QString> buildModifiedPhotoIdList();

            class factory {
            public:
                static std::unique_ptr<ContactList>  create(const QByteArray& data);
            };
#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ContactList> EXAMPLE(int context_index, int parent_context_index);
#endif //API_QT_AUTOTEST
        };
        
        ///list of all contact groups
        class GroupList : public BatchBuilderInfoList<GroupInfo, BatchRequesGroupList>
        {
        public:
            class factory {
            public:
                static std::unique_ptr<GroupList>  create(const QByteArray& data);
            };
#ifdef API_QT_AUTOTEST
            static std::unique_ptr<GroupList> EXAMPLE(int context_index, int parent_context_index);
#endif //API_QT_AUTOTEST
        };

        ///contacts batch result
        class BatchContactList : public InfoList<BatchResultContactInfo> 
        {
        public:
            class factory {
            public:
                static std::unique_ptr<BatchContactList>  create(const QByteArray& data);
            };

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<BatchContactList> EXAMPLE(int context_index, int parent_context_index);
#endif //API_QT_AUTOTEST
        };


        ///groups batch result
        class BatchGroupList : public InfoList<BatchResultGroupInfo> 
        {
        public:
            class factory {
            public:
                static std::unique_ptr<BatchGroupList>  create(const QByteArray& data);
            };

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<BatchGroupList> EXAMPLE(int context_index, int parent_context_index);
#endif //API_QT_AUTOTEST
        };

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

            bool mergeServerModifications(GroupList& server_glist, ContactList& server_clist);
            QString getPhotoMediaPath(QString contactId, bool ensurePath = false)const;
            bool addPhoto(ContactInfo::ptr c, QString photoFileName);


        protected:
            bool ensureContactTables();
            bool storeContactGroups();
            bool storeContactEntries();
            bool storeConfig();

            bool loadContactGroupsFromDb();
            bool loadContactEntriesFromDb();
            bool loadContactConfigFromDb();
            bool storeContactList(std::vector<std::shared_ptr<ContactInfo>>& contact_list);
            bool storeGroupList(std::vector<std::shared_ptr<GroupInfo>>& group_list);            
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

        class GcontactCacheSyncTask : public GoogleVoidTask 
        {
        public:
            contact_cache_ptr       cache() { return m_cache; }
            const ContactList*      loadedContacts()const{return m_loaded_contacts.get();}
            const GroupList*        loadedGroups()const{return m_loaded_groups.get();}
            const BatchContactList* updatedContacts()const{return m_updated_contacts.get();}
            const BatchGroupList*   updatedGroups()const{return m_updated_groups.get();}            
        protected:
            GcontactCacheSyncTask(ApiEndpoint& ept, contact_cache_ptr c) :GoogleVoidTask(ept), m_cache(c) {}
            contact_cache_ptr m_cache;
            std::unique_ptr<ContactList> m_loaded_contacts;
            std::unique_ptr<GroupList> m_loaded_groups;
            std::unique_ptr<BatchContactList> m_updated_contacts;
            std::unique_ptr<BatchGroupList> m_updated_groups;
            friend class GcontactCacheRoutes;
        };
        
        class PhotoSyncTask : public GoogleVoidTask
        {
        public:
            const std::list<QString>& downloaded()const { return m_downloaded_ids; }
            const std::list<QString>& uploaded()const { return m_uploaded_ids; }
        private:
            PhotoSyncTask(ApiEndpoint& ept) :GoogleVoidTask(ept) {}
            std::list<QString> m_downloaded_ids;
            std::list<QString> m_uploaded_ids;
            friend class GcontactCacheRoutes;
        };


        class GcontactCacheRoutes : public QObject
        {
            Q_OBJECT
        public:
            GcontactCacheRoutes(googleQt::Endpoint& endpoint, GcontactRoutes& gcontact_routes);

            contact_cache_ptr       cache() { return m_GContactsCache; }

            GcontactCacheSyncTask*  synchronizeContacts_Async();
            PhotoSyncTask*          synchronizePhotos_Async();
            GoogleTask<QString>*    getContactCachePhoto_Async(ContactInfo::ptr c);

#ifdef API_QT_AUTOTEST
            void runAutotest();
#endif
        protected:
            void reloadCache_Async(GcontactCacheSyncTask* rv, QDateTime dtUpdatedMin);
            void applyLocalCacheModifications_Async(GcontactCacheSyncTask* rv);
        protected:
            Endpoint&           m_endpoint;
            GcontactRoutes&     m_c_routes;
            contact_cache_ptr   m_GContactsCache;

        private:
            class PhotoListTask : public GoogleVoidTask
            {
            public:
                const std::list<QString>& completed()const { return m_completed_ids; }
            private:
                PhotoListTask(ApiEndpoint& ept) :GoogleVoidTask(ept) {}
                std::list<QString> m_completed_ids;
                friend class GcontactCacheRoutes;
            };

            template <class PROCESSOR>
            PhotoListTask*      transferPhotos_Async(const std::list<QString>& id_list);
            PhotoListTask*      downloadPhotos_Async();
            PhotoListTask*      uploadPhotos_Async();
        };

    };//gcontact
};

