#include <iostream>
#include <functional>
#include <QBuffer>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <iomanip>
#include "GcontactCommands.h"
#include "google/demo/ApiTerminal.h"
#include "gcontact/GcontactRoutes.h"
#include "gcontact/GcontactCache.h"

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

void GcontactCommands::get_contact(QString contactid)
{
    if (contactid.isEmpty()) {
        std::cout << "contactid required" << std::endl;
        return;
    }

    try
    {
        ContactsListArg arg;
        arg.setContactId(contactid);
        auto contacts_list = m_gt->getContacts()->list(arg);
        std::cout << contacts_list->toString() << std::endl;
        auto& arr = contacts_list->data()->contacts();
        if(arr.size() > 0){
            std::cout << arr[0].toXmlString() << std::endl;   
        }        
        // m_c.printLastResponse();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}


void GcontactCommands::ls_as_json()
{
    try
    {
        ContactsListArg arg;
        arg.setAlt("json");
        auto contacts_list = m_gt->getContacts()->list(arg);
        m_c.printLastResponse();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }    
}


void GcontactCommands::export_last_result()
{
    QString dest_dir = QString("download");
    QDir att_dir;    
    if(!att_dir.mkpath(dest_dir)){
        std::cout << "Failed to create directory: " << dest_dir << std::endl;
        return;
    };

    QString fileName = dest_dir + "/result_export.txt";    
    m_c.exportLastResponse(fileName);
    std::cout << "saved:" << fileName << std::endl;
};

void GcontactCommands::parse_contacts_xml(QString xmlFileName) 
{
    if (xmlFileName.isEmpty()) {
        std::cout << "file name required" << std::endl;
        return;
    }

    QFile file(xmlFileName);
    if (!file.open(QIODevice::ReadOnly)) {
        std::cout << "failed to open file:" << xmlFileName << std::endl;
        return;
    }

    ContactList cl;
    if (cl.parseXml(file.readAll())) {
        std::cout << "=== total # of contacts " << cl.contacts().size() << " ===" << std::endl;
        std::cout << cl << std::endl;
        std::cout << "===============" << std::endl;

        //...ykh... test on data
        for (auto& c : cl.contacts()) {
            std::cout << c.toXmlString() << std::endl << std::endl;
        }
    }
};
