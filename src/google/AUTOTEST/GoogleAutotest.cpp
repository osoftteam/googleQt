#include "GoogleAutotest.h"
#include "gmail/AUTOTEST/GmailAutotest.h"
#include "gtask/AUTOTEST/GtaskAutotest.h"
#include "gdrive/AUTOTEST/GdriveAutotest.h"

using namespace googleQt;

#ifdef API_QT_AUTOTEST
GoogleAutotest::GoogleAutotest(std::shared_ptr<GoogleClient> cl):m_cl(cl)
{

};

GoogleAutotest::~GoogleAutotest()
{

};

void GoogleAutotest::generateCalls()
{
    if (m_cl)
    {
#define AUTOTEST_GENERATE_CALLS(T){T a(*(m_cl.get()));a.generateCalls();}
        AUTOTEST_GENERATE_CALLS(GmailAutotest);
        AUTOTEST_GENERATE_CALLS(GtaskAutotest);
        AUTOTEST_GENERATE_CALLS(GdriveAutotest);
#undef AUTOTEST_GENERATE_CALLS
    }
}


QString GoogleAutotest::userId()const
{
    QString rv = "";
    if (m_cl) {
        rv = m_cl->userId();
    }
    return rv;
}

void GoogleAutotest::setClient(std::shared_ptr<GoogleClient> cl) 
{
    m_cl = cl;
};

#endif//#ifdef API_QT_AUTOTEST