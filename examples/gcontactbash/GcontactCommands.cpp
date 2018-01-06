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
            std::cout << arr[0]->toXmlString() << std::endl;   
        }        
        // m_c.printLastResponse();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void GcontactCommands::create_contact(QString email_first_last) 
{
    if (email_first_last.isEmpty()) {
        std::cout << "Invalid parameters, expected <Email> <First-Name> <Last-Name>" << std::endl;
        return;
    }

    QStringList arg_list = email_first_last.split(" ",
        QString::SkipEmptyParts);

    if (arg_list.size() < 3)
    {
        std::cout << "Invalid parameters, expected <Email> <First-Name> <Last-Name>" << std::endl;
        return;
    }

    QString email = arg_list[0];
    QString first = arg_list[1];
    QString last = arg_list[2];

    try
    {
        ContactInfo ci;
        NameInfo n;
        EmailInfo e;
        PhoneInfo p;
        OrganizationInfo o;
        PostalAddress a;

        n.setFamilyName(last).setGivenName(first).setFullName(first + " " + last);
        e.setAddress(email).setDisplayName(first + " " + last).setPrimary(true).setTypeLabel("home");
        p.setNumber("1-111-1111").setPrimary(true);
        o.setName("organization-name").setTitle("title-in-the-organization");
        a.setCity("Mountain View").setStreet("1600 Amphitheatre Pkwy").setRegion("CA").setPostcode("94043").setCountry("United States").setPrimary(true);

        ci.setName(n).setTitle("Title for " + first + " " + last)
            .addEmail(e)
            .addPhone(p)
            .setContent(QString("My notest on new contact for '%1'").arg(first))
            .setOrganizationInfo(o)
            .addAddress(a);

        CreateContactArg arg;
        arg.setData(ci);
        auto contacts_list = m_gt->getContacts()->create(arg);
        std::cout << contacts_list->toString() << std::endl;
        auto& arr = contacts_list->data()->contacts();
        if (arr.size() > 0) {
            std::cout << arr[0]->toXmlString() << std::endl;
        }

        //m_c.printLastResponse();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GcontactCommands::test_contact_xml() 
{
    QString email = "me@gmail.com";
    QString first = "First-Name";
    QString last = "Last-Name";

    ContactInfo ci;
    NameInfo n;
    EmailInfo e1, e2;
    PhoneInfo p;
    OrganizationInfo o;
    PostalAddress a;

    n.setFamilyName(last).setGivenName(first).setFullName(first + " " + last);
    e1.setAddress(email).setDisplayName(first + " " + last).setPrimary(true).setTypeLabel("home");
    e2.setAddress(QString("2") + email).setDisplayName(first + " " + last).setPrimary(false).setTypeLabel("home");
    p.setNumber("1-111-1111").setPrimary(true);
    o.setName("organization-name").setTitle("title-in-the-organization");
    a.setCity("Mountain View").setStreet("1600 Amphitheatre Pkwy").setRegion("CA").setPostcode("94043").setCountry("United States").setPrimary(true);

    ci.setName(n).setTitle("Title of " + first + " " + last)
        .addEmail(e1).addEmail(e2)
        .addPhone(p)
        .setContent(QString("My notest on new contact for '%1'").arg(first))
        .setOrganizationInfo(o)
        .addAddress(a);

    QString xml = ci.toXmlString();
    std::cout << "=== XML/begin ==== " << std::endl;
    std::cout << xml << std::endl;
    std::cout << "=== XML/end ==== " << std::endl;

    QByteArray data(xml.toStdString().c_str());

    QDomDocument doc;
    QString errorMsg = 0;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        std::cout << "Failed to export contacts XML document: " << errorMsg << " line=" << errorLine << " column=" << errorColumn << std::endl;
        return;
    }

    std::cout << ci.toString() << std::endl;
    std::cout << "======= " << std::endl;

    std::cout << "xml export - OK" << std::endl;

    ContactInfo ci2;
    if (!ci2.parseXml(data)) {
        std::cout << "Xml parse error" << std::endl;
        return;
    };
    std::cout << "xml import - OK" << std::endl;
    
    if (ci != ci2) {
        std::cout << "Exported != Imported obj." << std::endl;
        QString xml = ci2.toXmlString();
        std::cout << "=== XML#2/begin ==== " << std::endl;
        std::cout << xml << std::endl;
        std::cout << "=== XML#2/end ==== " << std::endl;
        return;
    }
    std::cout << "identity   - OK" << std::endl;
};

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

        for (auto& c : cl.contacts()) {
            std::cout << c->toXmlString() << std::endl << std::endl;
        }
    }
};
