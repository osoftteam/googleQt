#include "GoogleHost.h"

using namespace googleQt;

GoogleHost default_gmail_host;

GoogleHost::GoogleHost():
    m_api("googleapis.com"),
    m_auth("accounts.google.com")
{
};

GoogleHost& GoogleHost::DEFAULT()
{
    return default_gmail_host;
};

