#pragma once

#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiAutotest.h"

#ifdef API_QT_AUTOTEST
namespace googleQt{
	class GoogleClient;

    class GoogleAutotest: public ApiAutotest{
    public:
        GoogleAutotest(GoogleClient& cl);
        virtual ~GoogleAutotest();
        void generateCalls()override;
    protected:
        GoogleClient& m_cl;
    };
};//googleQt
#endif//#ifdef API_QT_AUTOTEST
