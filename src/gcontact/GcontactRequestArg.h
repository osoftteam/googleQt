#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gcontact/GcontactCache.h"

namespace googleQt {
    namespace gcontact {

        class ContactList;

        /**
            argument class for contacts queries
        */
        class ContactsListArg : public QParamArg
        {
        public:
            ContactsListArg(QString contactId = "");
            void build(const QString& link_path, QUrl& url)const override;

            ///The type of feed to return, such as atom (the default), rss, or json.
            void    setAlt(QString val) { m_alt = val; }
            QString alt()const{return m_alt;}

            /**
               Fulltext query on contacts data fields. The API currently supports simple search queries such as q=term1 term2 term3 and exact search queries such as q="term1 term2 term3"
             */
            void    setQ(QString val) { m_q = val; }
            QString q()const{return m_q;}

            /**
                if contactId is defined only this ID entry will be returned in result 
            */
            void setContactId(QString val) { m_contactId = val; }
            QString contactId()const {return m_contactId;}

            /**
               The maximum number of entries to return. If you want to receive all of the contacts, rather than only the default maximum, you can specify a very large number for max-results.
             */
            void    setMaxResults(int val){m_max_results = val;}
            int     maxResults()const{return m_max_results;}

            /**
                The lower bound on entry update dates.
            */
            void setUpdatedMin(const QDateTime& val) { m_updated_min = val; }
            const QDateTime& updatedMin()const { return m_updated_min; }

            /**
               Sorting criterion. The only supported value is lastmodified.
             */
            QString orderby()const{return m_orderby;}
            void setOrderby(QString s){m_orderby = s;}

            /**
               Include deleted contacts in the returned contacts feed. Deleted contacts are shown as entries that contain nothing but an <atom:id> element and a <gd:deleted> element. (Google usually retains placeholders for deleted contacts for 30 days after deletion; during that time, you can request the placeholders using the showdeleted query parameter.) Valid values are true or false. When the server decides it cannot guarantee that it still has information about all deleted contacts pertinent to the query, then it's behavior depends on the value of the requirealldeleted query parameter.
             */
            bool   showdeleted()const{return m_showdeleted;}
            void   setShowdeleted(bool val){m_showdeleted = val;}

            /**
               Only relevant if showdeleted and updated-min are also provided. It dictates the behavior of the server in case it detects that placeholders of some entries deleted since the point in time specified as updated-min may have been lost. If requirealldeleted is false, the server simply returns all the placeholders it still knows about. If true, the server returns the 410 HTTP response code. The default value is false.
             */
            bool   requirealldeleted()const{return m_requirealldeleted;}
            void   setRequirealldeleted(bool val){m_requirealldeleted = val;}

            /**
               Sorting order direction. Can be either ascending or descending.
             */
            QString sortorder()const{return m_sortorder;}
            void setSortorder(QString s){m_sortorder = s;}

            /**
               Constrains the results to only the contacts belonging to the group specified. Value of this parameter specifies group ID
             */
            QString group()const{return m_group;}
            void setGroup(QString s){m_group = s;}

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ContactsListArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString m_alt { };
            QString m_q   { };
            QString m_contactId{ };
            int     m_max_results{ 0 };
            QDateTime m_updated_min;
            QString m_orderby;
            bool    m_showdeleted{ false };
            bool    m_requirealldeleted;
            QString m_sortorder;
            QString m_group;
        };

        /**
        argument class for creating contact
        */
        class CreateContactArg : public QParamArg
        {
        public:
            CreateContactArg();
            CreateContactArg(const ContactInfo& ci);

            void build(const QString& link_path, QUrl& url)const override;

            const ContactInfo& data()const { return m_contact_info; }
            void setData(const ContactInfo& c) { m_contact_info = c; };

            QString toXml(QString userId)const;

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<CreateContactArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            ContactInfo m_contact_info;
        };

        /**
            argument class for updating contact
        */
        class UpdateContactArg : public QParamArg
        {
        public:
            UpdateContactArg();
            UpdateContactArg(const ContactInfo& ci);
            void build(const QString& link_path, QUrl& url)const override;

            const ContactInfo& data()const { return m_contact_info; }
            void setData(const ContactInfo& c) { m_contact_info = c; };

            QString toXml()const;
            QString etag()const;

            bool ignoreEtag()const { return m_ignore_etag; }
            void setIgnoreEtag(bool val) { m_ignore_etag = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<UpdateContactArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            ContactInfo m_contact_info;
            bool        m_ignore_etag{false};//will ignore etag and use '*' instead
        };

        /**
        argument class for deleting contact
        */
        class DeleteContactArg : public QParamArg
        {
        public:
            DeleteContactArg();
            DeleteContactArg(QString contact_id, QString etag);
            void build(const QString& link_path, QUrl& url)const override;

            QString contactId()const{return m_contact_id;}
            void setContactId(QString val) { m_contact_id = val; }
            QString etag()const;
            
            bool ignoreEtag()const { return m_ignore_etag; }
            void setIgnoreEtag(bool val) { m_ignore_etag = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<DeleteContactArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString m_contact_id;
            QString m_etag;
            bool        m_ignore_etag{ false };//will ignore etag and use '*' instead
        };        

        /**
            result of contacts queries, can contain one entry if ContactsListArg was used with contactId
        */
        class ContactsListResult 
        {
        public:
            ContactsListResult(const QByteArray& data);

            ContactList* data();
            bool    isNull()const {return m_is_null;}
            QString toString(bool multiline = true)const;

            class factory {
            public:
                static std::unique_ptr<ContactsListResult>  create(const QByteArray& data);
            };

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ContactsListResult> EXAMPLE(int context_index, int parent_context_index);
#endif //API_QT_AUTOTEST


        protected:
            std::unique_ptr<ContactList> m_data;
            bool m_is_null{true};
        };//ContactsListResult
     

        /// groups
        /**
            argument class for contact group queries
        */
        class ContactGroupListArg : public QParamArg
        {
        public:
            ContactGroupListArg();
            void build(const QString& link_path, QUrl& url)const override;

            /**
            if contactId is defined only this ID entry will be returned in result
            */
            void setGroupId(QString val) { m_groupId = val; }
            QString groupId()const { return m_groupId; }


            ///The type of feed to return, such as atom (the default), rss, or json.
            void    setAlt(QString val) { m_alt = val; }
            QString alt()const { return m_alt; }

            /**
            Fulltext query on contacts data fields. The API currently supports simple search queries such as q=term1 term2 term3 and exact search queries such as q="term1 term2 term3"
            */
            void    setQ(QString val) { m_q = val; }
            QString q()const { return m_q; }

            /**
            The maximum number of entries to return. If you want to receive all of the contacts, rather than only the default maximum, you can specify a very large number for max-results.
            */
            void    setMaxResults(int val) { m_max_results = val; }
            int     maxResults()const { return m_max_results; }

            /**
            The lower bound on entry update dates.
            */
            void setUpdatedMin(const QDateTime& val) { m_updated_min = val; }
            const QDateTime& updatedMin()const { return m_updated_min; }

            /**
            Sorting criterion. The only supported value is lastmodified.
            */
            QString orderby()const { return m_orderby; }
            void setOrderby(QString s) { m_orderby = s; }

            /**
            Include deleted contacts in the returned contacts feed. Deleted contacts are shown as entries that contain nothing but an <atom:id> element and a <gd:deleted> element. (Google usually retains placeholders for deleted contacts for 30 days after deletion; during that time, you can request the placeholders using the showdeleted query parameter.) Valid values are true or false. When the server decides it cannot guarantee that it still has information about all deleted contacts pertinent to the query, then it's behavior depends on the value of the requirealldeleted query parameter.
            */
            bool   showdeleted()const { return m_showdeleted; }
            void   setShowdeleted(bool val) { m_showdeleted = val; }

            /**
            Only relevant if showdeleted and updated-min are also provided. It dictates the behavior of the server in case it detects that placeholders of some entries deleted since the point in time specified as updated-min may have been lost. If requirealldeleted is false, the server simply returns all the placeholders it still knows about. If true, the server returns the 410 HTTP response code. The default value is false.
            */
            bool   requirealldeleted()const { return m_requirealldeleted; }
            void   setRequirealldeleted(bool val) { m_requirealldeleted = val; }

            /**
            Sorting order direction. Can be either ascending or descending.
            */
            QString sortorder()const { return m_sortorder; }
            void setSortorder(QString s) { m_sortorder = s; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ContactGroupListArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST
        protected:
            QString m_groupId;
            QString m_alt{};
            QString m_q{};
            int     m_max_results{ 0 };
            QDateTime m_updated_min;
            QString m_orderby;
            bool    m_showdeleted{ false };
            bool    m_requirealldeleted;
            QString m_sortorder;
        };

        /**
        result of contacts queries, can contain one entry if ContactsListArg was used with contactId
        */
        class ContactGroupListResult
        {
        public:
            ContactGroupListResult(const QByteArray& data);
            
            GroupList* data();

            QString toString(bool multiline = true)const;
            
            class factory {
            public:
                static std::unique_ptr<ContactGroupListResult>  create(const QByteArray& data);
            };

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ContactGroupListResult> EXAMPLE(int context_index, int parent_context_index);
#endif //API_QT_AUTOTEST


        protected:
            std::unique_ptr<GroupList> m_data;
        };//ContactGroupListResult


          /**
            argument class for creating contacts group
          */
        class CreateContactGroupArg : public QParamArg
        {
        public:
            CreateContactGroupArg();
            CreateContactGroupArg(const GroupInfo& gi);

            void build(const QString& link_path, QUrl& url)const override;

            const GroupInfo& data()const { return m_group_info; }
            void setData(const GroupInfo& c) { m_group_info = c; };


            QString toXml(QString userId)const;

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<CreateContactGroupArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            GroupInfo m_group_info;
        };

        /**
        argument class for updating contact
        */
        class UpdateContactGroupArg : public QParamArg
        {
        public:
            UpdateContactGroupArg();
            UpdateContactGroupArg(const GroupInfo& g);
            void build(const QString& link_path, QUrl& url)const override;

            const GroupInfo& data()const { return m_group_info; }
            void setData(const GroupInfo& c) { m_group_info = c; };

            QString toXml()const;
            QString etag()const;

            bool ignoreEtag()const { return m_ignore_etag; }
            void setIgnoreEtag(bool val) { m_ignore_etag = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<UpdateContactGroupArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            bool        m_ignore_etag{ false };//will ignore etag and use '*' instead
            GroupInfo   m_group_info;
        };


        /**
            argument class for deleting contact group
        */
        class DeleteContactGroupArg : public QParamArg
        {
        public:
            DeleteContactGroupArg();
            DeleteContactGroupArg(QString group_id, QString etag);
            void build(const QString& link_path, QUrl& url)const override;

            QString groupId()const { return m_group_id; }
            void    setGroupId(QString val) { m_group_id = val; }

            QString etag()const;
            void    setEtag(QString val) { m_etag = val; }

            bool ignoreEtag()const { return m_ignore_etag; }
            void setIgnoreEtag(bool val) { m_ignore_etag = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<DeleteContactGroupArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString m_group_id;
            QString m_etag;
            bool    m_ignore_etag{ false };//will ignore etag and use '*' instead
        };

        class DownloadPhotoArg : public QParamArg
        {
        public:
            DownloadPhotoArg() {};
            DownloadPhotoArg(QString contactId);
            void build(const QString& link_path, QUrl& url)const override;

            void setContactId(QString val) { m_contactId = val; }
            QString contactId()const { return m_contactId; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<DownloadPhotoArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString m_contactId{};
        };

        class UploadPhotoArg : public QParamArg
        {
        public:
            UploadPhotoArg() {};
            UploadPhotoArg(QString contactId);
            void build(const QString& link_path, QUrl& url)const override;

            void setContactId(QString val) { m_contactId = val; }
            QString contactId()const { return m_contactId; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<UploadPhotoArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString m_contactId{};
        };

        /**
        argument class for deleting contact
        */
        class DeletePhotoArg : public QParamArg
        {
        public:
            DeletePhotoArg();
            DeletePhotoArg(QString contact_id, QString etag);
            void build(const QString& link_path, QUrl& url)const override;

            QString contactId()const { return m_contact_id; }
            void setContactId(QString val) { m_contact_id = val; }
            QString etag()const;

            bool ignoreEtag()const { return m_ignore_etag; }
            void setIgnoreEtag(bool val) { m_ignore_etag = val; }

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<DeletePhotoArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

        protected:
            QString m_contact_id;
            QString m_etag;
            bool    m_ignore_etag{ false };//will ignore etag and use '*' instead
        };
    }
};
