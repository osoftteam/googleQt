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
    * create_contact - create single contact entry
    * requires email adddress, first name, last name - space separated
    */
    void create_contact(QString email_first_last);


    /**
    * ls_as_json - list contacts using json
    */
    void ls_as_json();

    /**
    * test_contact_xml - create contact obj, export to XML and import back
    */
    void test_contact_xml();

    ///parse xml contacts file
    void parse_contacts_xml(QString xmlFileName);

    
    void export_last_result();
protected:
    GoogleClient& m_c;
    GcontactRoutes*  m_gt;
};
