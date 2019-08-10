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
            const GroupMembershipInfoList& groups()const { return m_mgroups; }
            const UserDefinedFieldInfoList& userFields()const { return m_user_def_fields; }

            /// Ref-function return non-const references and can be used
            /// to modify objects. call ContactInfo::markAsModified to stage
            /// object ready for storing into database and storeContactsToDb to save
            EmailInfoList&       emailsRef(){ return m_emails; }
            PhoneInfoList&       phonesRef(){ return m_phones; }
            NameInfo&            nameRef(){ return m_name; }
            OrganizationInfo&    organizationRef(){ return m_organization; }
            PostalAddressList&   addressesRef(){ return m_address_list; }
            GroupMembershipInfoList& groupsRef() { return m_mgroups; }
            UserDefinedFieldInfoList& userFieldsRef() { return m_user_def_fields; }

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
            ContactInfo& replaceEmails(const std::vector<EmailInfo>& lst);

            /**
                add new phone info
            */
            ContactInfo& addPhone(const PhoneInfo& p);

            /**
                delete old phone and add new
            */
            ContactInfo& replacePhones(const std::vector<PhoneInfo>& lst);

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
            replace groups with new ids
            */
            ContactInfo& setGroups(QString userId, const STRING_LIST& groupIdlist);

            /**
                delete old address list and put a new one
            */
            ContactInfo& replaceAddressList(const std::vector<PostalAddress>& lst);

            //..
            /**
            add user field
            */
            ContactInfo& addUserField(const UserDefinedFieldInfo& f);
            //...

            static std::unique_ptr<ContactInfo> createWithId(QString contact_id);

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
            GroupMembershipInfoList  m_mgroups;
            UserDefinedFieldInfoList m_user_def_fields;
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

            /**
                in case of regular group it's just the title
                in case of system group we trim away system prefix
            */
            QString displayTitle()const;

            bool isSystemGroup()const {return m_issystem_group;};

            static std::unique_ptr<GroupInfo> createWithId(QString group_id);

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
            void            setupTitle(QString title);
        protected:
            bool    m_issystem_group{false};
            QString m_display_title;
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
            ContactInfo::ptr findNewCreated(std::shared_ptr<BatchResultContactInfo> b);
            
            STRING_LIST buildUnresolvedPhotoIdList();
            STRING_LIST buildModifiedPhotoIdList();

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
            /// this is special case of 'limbo' group - we created it locally, it has DBID, we sent requiest
            /// to server and server assigned cloudID - we have to link cloudID to the contact and update local cache
            GroupInfo::ptr findNewCreated(std::shared_ptr<BatchResultGroupInfo> b);

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
        class GContactCacheBase
        {
        public:
            GContactCacheBase(QString userid);

			virtual QString		metaPrefix()const = 0;
			virtual int			accountId()const = 0;
			virtual QSqlQuery*	prepareContactQuery(QString sql) = 0;
			virtual bool		execContactQuery(QString sql) = 0;
			virtual QSqlQuery*	selectContactQuery(QString sql) = 0;
			virtual QString		contactCacheDir()const = 0;

            ContactList& contacts() { return m_contacts; }
            const ContactList& contacts()const { return m_contacts; }

            GroupList& groups() { return m_groups; }
            const GroupList& groups()const { return m_groups; }

            const QDateTime& lastSyncTime()const { return m_sync_time; }

            bool storeContactsToDb();
            bool loadContactsFromDb();
            bool clearDbCache();

            bool mergeServerModifications(GroupList& server_glist, ContactList& server_clist);
            QString getPhotoMediaPath(QString contactId, bool ensurePath = false)const;
            bool addPhoto(ContactInfo::ptr c, QString photoFileName);

			void setEnableContactsConfigTable(bool val) { m_enable_contacts_config_table = val;}
			bool ensureContactTables();

        protected:
            bool storeContactGroups();
            bool storeContactEntries();
            bool storeConfig();

            bool loadContactGroupsFromDb();
            bool loadContactEntriesFromDb();
            bool loadContactConfigFromDb();
            bool storeContactList(std::vector<std::shared_ptr<ContactInfo>>& contact_list);
            bool storeGroupList(std::vector<std::shared_ptr<GroupInfo>>& group_list);            
        protected:
            ContactList m_contacts;
            GroupList m_groups;
            std::map<QString, QString> m_configs;
            QDateTime		m_sync_time;
			QString			m_userid;
			bool			m_enable_contacts_config_table{true};
        };

		class GContactCache: public GContactCacheBase
		{
		public:
			GContactCache(QString userid);
			virtual QString metaPrefix()const override;
			virtual int		accountId()const override;
			QSqlQuery*	prepareContactQuery(QString sql) override;
			bool		execContactQuery(QString sql)override;
			QSqlQuery*	selectContactQuery(QString sql)override;
			QString		contactCacheDir()const override;

			void attachSQLStorage(mail_cache::GMailSQLiteStorage* ss);
		protected:
			mail_cache::GMailSQLiteStorage* m_sql_storage{ nullptr };
			friend class GcontactCacheRoutes;
		};

        class GcontactCacheSyncTask : public GoogleVoidTask 
        {
        public:
            const ContactList*      loadedContacts()const{return m_loaded_contacts.get();}
            const GroupList*        loadedGroups()const{return m_loaded_groups.get();}
            const BatchContactList* updatedContacts()const{return m_updated_contacts.get();}
            const BatchGroupList*   updatedGroups()const{return m_updated_groups.get();}
            const std::map<QString, std::shared_ptr<BatchRequestGroupInfo>>& deleted_groups()const{return m_deleted_groups;}
        protected:
            GcontactCacheSyncTask(ApiEndpoint& ept) :GoogleVoidTask(ept){}
            std::unique_ptr<ContactList>      m_loaded_contacts;
            std::unique_ptr<GroupList>        m_loaded_groups;
            std::unique_ptr<BatchContactList> m_updated_contacts;
            std::unique_ptr<BatchGroupList>   m_updated_groups;
            std::map<QString, std::shared_ptr<BatchRequestGroupInfo>> m_deleted_groups;
            friend class GcontactCacheRoutes;
        };
        
        class PhotoSyncTask : public GoogleVoidTask
        {
        public:
            const STRING_LIST& downloaded()const { return m_downloaded_ids; }
            const STRING_LIST& uploaded()const { return m_uploaded_ids; }
        private:
            PhotoSyncTask(ApiEndpoint& ept) :GoogleVoidTask(ept) {}
            STRING_LIST m_downloaded_ids;
            STRING_LIST m_uploaded_ids;
            friend class GcontactCacheRoutes;
        };


        class GcontactCacheRoutes : public QObject
        {
            Q_OBJECT
        public:
            GcontactCacheRoutes(googleQt::Endpoint& endpoint, GcontactRoutes& gcontact_routes);

			GContactCache*			cache() { return m_GContactsCache.get(); }

            GcontactCacheSyncTask*  synchronizeContacts_Async();
            PhotoSyncTask*          synchronizePhotos_Async();
            GoogleTask<QString>*    getContactCachePhoto_Async(ContactInfo::ptr c);

#ifdef API_QT_AUTOTEST
            void runAutotest();
#endif
        protected:
            void reloadCache_Async(GcontactCacheSyncTask* rv, QDateTime dtUpdatedMin);
            void applyLocalContacEntriesModifications_Async(GcontactCacheSyncTask* rv);
            // void applyLocalGroupModifications_Async(GcontactCacheSyncTask* rv);
        protected:
            Endpoint&						m_endpoint;
            GcontactRoutes&					m_c_routes;
			std::unique_ptr<GContactCache>  m_GContactsCache;

        private:
            class PhotoListTask : public GoogleVoidTask
            {
            public:
                const STRING_LIST& completed()const { return m_completed_ids; }
            private:
                PhotoListTask(ApiEndpoint& ept) :GoogleVoidTask(ept) {}
                STRING_LIST m_completed_ids;
                friend class GcontactCacheRoutes;
            };

            template <class PROCESSOR>
            PhotoListTask*      transferPhotos_Async(const STRING_LIST& id_list);
            PhotoListTask*      downloadPhotos_Async();
            PhotoListTask*      uploadPhotos_Async();
        };

    };//gcontact
};

