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
            ContactsListArg();
            void build(const QString& link_path, QUrl& url)const override;

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<ContactsListArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST
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

        protected:
            QString m_alt { };
            QString m_q   { };
            QString m_contactId{ };
            int     m_max_results{ 0 };
            QDateTime m_updated_min;
        };

        /**
        argument class for creating contact
        */
        class CreateContactArg : public QParamArg
        {
        public:

            void build(const QString& link_path, QUrl& url)const override;

            QString toXmlString()const;

#ifdef API_QT_AUTOTEST
            static std::unique_ptr<CreateContactArg> EXAMPLE(int context_index, int parent_content_index);
#endif //API_QT_AUTOTEST

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
    }
};
