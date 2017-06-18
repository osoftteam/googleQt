#include "GoogleAutotest.h"
#include "gmail/AUTOTEST/GmailAutotest.h"
#include "gtask/AUTOTEST/GtaskAutotest.h"
#include "gdrive/AUTOTEST/GdriveAutotest.h"

using namespace googleQt;

#ifdef API_QT_AUTOTEST
GoogleAutotest::GoogleAutotest(GoogleClient& cl):m_cl(cl)
{

};

GoogleAutotest::~GoogleAutotest()
{

};

void GoogleAutotest::generateCalls()
{
#define AUTOTEST_GENERATE_CALLS(T){T a(m_cl);a.generateCalls();}
    AUTOTEST_GENERATE_CALLS(GmailAutotest);
    AUTOTEST_GENERATE_CALLS(GtaskAutotest);
    AUTOTEST_GENERATE_CALLS(GdriveAutotest);
#undef AUTOTEST_GENERATE_CALLS
}

QString GoogleAutotest::userId()const
{
    return m_cl.userId();
}

#endif//#ifdef API_QT_AUTOTEST