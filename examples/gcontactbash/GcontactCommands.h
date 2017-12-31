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
    * ls_contacts - list contacts
    */
    void ls_contacts();

    /**
    * get_contact - get single contact entry
    */
    void get_contact(QString contactid);

    /**
    * ls_as_json - list contacts using json
    */
    void ls_as_json();

    ///parse xml contacts file
    void parse_contacts_xml(QString xmlFileName);

    
    void export_last_result();
protected:
    GoogleClient& m_c;
    GcontactRoutes*  m_gt;
};
