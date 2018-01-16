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
#include "Endpoint.h"

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
        arg.setMaxResults(100);
        arg.setOrderby("lastmodified");
        arg.setSortorder("descending");
        auto contacts_list = m_gt->getContacts()->list(arg);
        auto& arr = contacts_list->data()->contacts();
        int idx = 1;
        std::cout << "------------------------------------------------------------" << std::endl;
        std::cout << "  #      ID              etag                       updated " << std::endl;
        std::cout << "------------------------------------------------------------" << std::endl;
        for(auto& c : arr){
            QString info = c->title();
            if(info.isEmpty()){
                if(c->emails().size() > 0){
                    info = c->emails()[0].address();
                }
            }
            std::cout << std::setw(3) << idx++ << ". "
                      << c->id() << " "
                      << c->etag() << " "
                      << c->updated().toString("dd.MM.yyyy") << " "
                      << info << std::endl;
        }
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
            std::cout << arr[0]->toXml(m_c.userId()) << std::endl;
        }
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
            std::cout << arr[0]->toXml(m_c.userId()) << std::endl;
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
    e2.setAddress(QString("2") + email).setDisplayName(first + " " + last).setPrimary(false).setTypeLabel("work");
    p.setNumber("1-111-1111").setPrimary(true);
    o.setName("organization-name").setTitle("title-in-the-organization");
    a.setCity("Mountain View").setStreet("1600 Amphitheatre Pkwy").setRegion("CA").setPostcode("94043").setCountry("United States").setPrimary(true);

    ci.setName(n).setTitle("Title of " + first + " " + last)
        .addEmail(e1).addEmail(e2)
        .addPhone(p)
        .setContent(QString("My notest on new contact for '%1'").arg(first))
        .setOrganizationInfo(o)
        .addAddress(a);

    QString xml = ci.toXml(m_c.userId());
    std::cout << "=== XML/begin ==== " << std::endl;
    std::cout << xml << std::endl;
    std::cout << "=== XML/end ==== " << std::endl;

    bool checkXmlExport = true;

    if (checkXmlExport) {
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
            QString xml = ci2.toXml(m_c.userId());
            std::cout << "=== XML#2/begin ==== " << std::endl;
            std::cout << xml << std::endl;
            std::cout << "=== XML#2/end ==== " << std::endl;
            return;
        }
        std::cout << "identity   - OK" << std::endl;

        ContactInfo ci3;
        if (!ci3.parseXml(ci2.originalXml())) {
            std::cout << "Original Xml parse error" << std::endl;
            return;
        };

        if (ci != ci3) {
            std::cout << "Original Exported != Imported obj." << std::endl;
            QString xml = ci3.toXml(m_c.userId());
            std::cout << "=== Original XML#2/begin ==== " << std::endl;
            std::cout << xml << std::endl;
            std::cout << "=== Original XML#2/end ==== " << std::endl;
            return;
        }
        std::cout << "identity2  - OK" << std::endl;
    }
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

void GcontactCommands::parse_file(QString xmlFileName) 
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
            std::cout << c->toXml(m_c.userId()) << std::endl << std::endl;
        }
    }
};

void GcontactCommands::test_merge(QString xmlFileName)
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

    ContactInfo c;
    c.parseXml(file.readAll());
    std::cout << "=============== BEGIN " << xmlFileName << std::endl;
    std::cout << c.toXml(m_c.userId()) << std::endl << std::endl;
    std::cout << "=============== END " << xmlFileName << std::endl;
    c.setTitle(c.title() + "=NEW-TITLE=");
    c.setContent("=NEW-CONTENT=");

    OrganizationInfo o;
    o.setName("=NEW-organization-name=").setTitle("=NEW-title-in-the-organization=");
    c.setOrganizationInfo(o);
    NameInfo n;
    n.setFamilyName("=NEW-last=").setGivenName("=NEW-first").setFullName("=NEW-first_and_last=");
    c.setName(n);

    PhoneInfo p1, p2;
    std::list<PhoneInfo> lst;
    p1.setNumber("=NEW-1-111-1111=").setPrimary(true);
    p2.setNumber("=NEW-2-222-2222=").setPrimary(false);
    lst.push_back(p1);
    lst.push_back(p2);
    c.replacePhones(lst);

    EmailInfo e1, e2;
    e1.setAddress("=NEW-first-email=").setDisplayName("=NEW-first-last=").setPrimary(true).setTypeLabel("home");
    e2.setAddress("=NEW-second-email=").setDisplayName("=NEW-second-first-last=").setPrimary(false).setTypeLabel("work");
    std::list<EmailInfo> e_lst;
    e_lst.push_back(e1);
    e_lst.push_back(e2);
    c.replaceEmails(e_lst);

    PostalAddress a1, a2;
    a1.setCity("=NEW-ADDR-1=").setStreet("=NEW-STREET1=").setRegion("=NEW-REGION1=").setPostcode("=NEW-ZIP1=").setCountry("=NEW-COUNTRY1=").setPrimary(true);
    a2.setCity("=NEW-ADDR-2=").setStreet("=NEW-STREET2=").setRegion("=NEW-REGION2=").setPostcode("=NEW-ZIP2=").setCountry("=NEW-COUNTRY2=").setPrimary(false);
    std::list<PostalAddress> a_lst;
    a_lst.push_back(a1);
    a_lst.push_back(a2);
    c.replaceAddressList(a_lst);

    std::cout << c.mergedXml(m_c.userId(), c.originalXml()) << std::endl << std::endl;
};

void GcontactCommands::update_contact_title(QString contactId_title) 
{
    ///first we get contact, make changes, merge them and upload modified xml
    QStringList arg_list = contactId_title.split(" ",
        QString::SkipEmptyParts);

    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <contactId> <Title>" << std::endl;
        return;
    }

    QString contactid = arg_list[0];
    QString new_title = arg_list[1];

    try
    {
        ContactsListArg arg;
        arg.setContactId(contactid);
        auto contacts_list = m_gt->getContacts()->list(arg);        
        auto& arr = contacts_list->data()->contacts();
        if (arr.size() > 0) {
            auto c = arr[0];
            c->setTitle(new_title);
            UpdateContactArg upd(*(c.get()));
            auto contacts_list = m_gt->getContacts()->update(upd);
            std::cout << contacts_list->toString() << std::endl;
        }
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GcontactCommands::get_photo(QString contactid) 
{
    if (contactid.isEmpty())
    {
        std::cout << "contactid required" << std::endl;
        return;
    }

    QFile out;
    try
    {
        QDir d;
        if (!d.mkpath("downloads")) {
            std::cout << "Failed to create 'downloads' directory." << std::endl;
            return;
        }

        out.setFileName("downloads/" + contactid);
        if (!out.open(QFile::WriteOnly | QIODevice::Truncate)) {
            std::cout << "Error opening file: " << out.fileName() << std::endl;
            return;
        }

        DownloadPhotoArg arg(contactid);
        m_gt->getContacts()->getContactPhoto(arg, &out);
        out.flush();
        std::cout << "=== photo downloaded ===" << std::endl;
        std::cout << size_human(out.size()) << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << " " << e.errSummary() << std::endl;
    }

    out.close();
};

void GcontactCommands::ls_groups()
{
    try
    {
        ContactGroupListArg arg;
        auto group_list = m_gt->getContactGroup()->list(arg);
        m_c.printLastResponse();
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void GcontactCommands::get_group(QString groupid)
{
    if (groupid.isEmpty()) {
        std::cout << "groupid required" << std::endl;
        return;
    }

    try
    {
        ContactGroupListArg arg;
        arg.setGroupId(groupid);
        auto contacts_list = m_gt->getContactGroup()->list(arg);
        m_c.printLastResponse();
        /*
        std::cout << contacts_list->toString() << std::endl;
        auto& arr = contacts_list->data()->contacts();
        if (arr.size() > 0) {
            std::cout << arr[0]->toXml(m_c.userId()) << std::endl;
        }
        */
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

/*
void GcontactCommands::testPeopleContacts()
{
    m_c.endpoint()->listPeopleContactGroup();
    m_c.printLastResponse();
    };
*/
