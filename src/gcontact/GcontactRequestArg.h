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

        };
    }
};
