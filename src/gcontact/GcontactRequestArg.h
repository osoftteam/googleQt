#pragma once
#include "google/endpoint/ApiUtil.h"

namespace googleQt {
    namespace gcontact {
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
               The maximum number of entries to return. If you want to receive all of the contacts, rather than only the default maximum, you can specify a very large number for max-results.
             */
            void    setMaxResults(int val){m_max_results = val;}
            int     maxResults()const{return m_max_results;}
        protected:
            QString m_alt { };
            QString m_q   { };
            int     m_max_results{ 0 };
        };
    }
};
