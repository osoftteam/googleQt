#include <iostream>
#include <functional>
#include <QBuffer>
#include <QFile>
#include <QFileInfo>
#include <iomanip>
#include "GcontactCommands.h"
#include "google/demo/ApiTerminal.h"
#include "gcontact/GcontactRoutes.h"

using namespace googleQt;
using namespace gcontact;
using namespace demo;

GcontactCommands::GcontactCommands(GoogleClient& c):m_c(c)
{
    m_gt = m_c.gcontact();
};


void GcontactCommands::ls_contacts()
{
    try
    {
        ContactsListArg arg;
        auto contacts_list = m_gt->getContacts()->list(arg);
        m_c.printLastResponse();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }    
}
