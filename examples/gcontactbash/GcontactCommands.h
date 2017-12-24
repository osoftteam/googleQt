#pragma once

#include "GoogleClient.h"

using namespace googleQt;

namespace googleQt {
    class GcontactRoutes;
};


class GcontactCommands
{
public:
    GcontactCommands(GoogleClient& c);
    
    /**
    * ls - list contacts
    */
    void ls_contacts();

    void export_last_result();
protected:
    GoogleClient& m_c;
    GcontactRoutes*  m_gt;
};
