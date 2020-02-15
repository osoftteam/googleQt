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

static const QString date_format = "dd/MM/yyyy hh:mm";

GcontactCommands::GcontactCommands(GoogleClient& c):m_c(c)
{
    m_gt = m_c.gcontact();
};


void GcontactCommands::ls_contacts()
{
    try
    {
        ContactListArg arg;
        arg.setMaxResults(100);
        arg.setOrderby("lastmodified");
        arg.setSortorder("descending");
        auto contacts_list = m_gt->getContacts()->list(arg);
        print_contact_list(contacts_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }    
}

void GcontactCommands::ls_contacts_date(QString updatedMin)
{
    std::function<void(void)> print_usage = []()
    {
        std::cout << "date time parameter required" << std::endl;
        std::cout << "example: ls_contacts_date " << QDateTime::currentDateTime().toString(date_format) << std::endl;
    };

    if (updatedMin.isEmpty()) {
        print_usage();
        return;
    }

    QDateTime dt = QDateTime::fromString(updatedMin, date_format);
    if (!dt.isValid()) {
        print_usage();
        return;
    }

    std::cout << "using date time parameter " << dt.toString(date_format) << std::endl;

    try
    {
        ContactListArg arg;
        arg.setMaxResults(100);
        arg.setOrderby("lastmodified");
        arg.setSortorder("descending");
        arg.setUpdatedMin(dt);
        auto contacts_list = m_gt->getContacts()->list(arg);
        print_contact_list(contacts_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void GcontactCommands::update_contact_user_field(QString contactId_key_value) 
{
    QStringList arg_list = contactId_key_value.split(" ",
        QString::SkipEmptyParts);

    if (arg_list.size() < 3)
    {
        std::cout << "Invalid parameters, expected <contactId> <Key> <Value>" << std::endl;
        return;
    }

    QString contactid = arg_list[0];
    QString key = arg_list[1];
    QString value = arg_list[2];

    try
    {
        ContactListArg arg;
        arg.setContactId(contactid);
        auto contacts_list = m_gt->getContacts()->list(arg);
        auto& arr = contacts_list->items();
        if (arr.size() > 0) {
            auto c = arr[0];
            auto& lst = c->userFieldsRef();
            auto i = std::find_if(lst.items().begin(), lst.items().end(), [&](const UserDefinedFieldInfo& f) {return f.key() == key; });
            if (i != lst.items().end()) {
                UserDefinedFieldInfo& f = *i;
                f.setValue(value);
            }
            else {
                UserDefinedFieldInfo f(key, value);
                c->addUserField(f);
            }

            UpdateContactArg upd(*(c.get()));
            upd.setIgnoreEtag(true);
            auto c_list = m_gt->getContacts()->update(upd);
            auto& arr2 = c_list->items();
            if (arr2.size() > 0) {
                std::cout << "Updated.." << std::endl;
                std::cout << arr2[0]->toXml(m_c.userId()) << std::endl;
            }
        }
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GcontactCommands::get_contact(QString contactid)
{
    if (contactid.isEmpty()) {
        std::cout << "contactid required" << std::endl;
        return;
    }

    try
    {
        ContactListArg arg;
        arg.setContactId(contactid);
        auto contacts_list = m_gt->getContacts()->list(arg);
        auto& arr = contacts_list->items();
        if(arr.size() > 0){
            ContactInfo::ptr c = arr[0];
            std::cout << "------------------" << std::endl;
            std::cout << c->toString() << std::endl;
            std::cout << "------------------" << std::endl;
            std::cout << c->toXml(m_c.userId()) << std::endl;
            std::cout << "------------------" << std::endl;
            std::cout << c->parsedXml() << std::endl;
            std::cout << "------------------" << std::endl;            
            auto& lst = c->userFields();
            if (!lst.items().empty()) {
                std::cout << "----- user defined fields ----" << std::endl;
                for (auto& f : lst.items()) {
                    std::cout << f.key() << " = " << f.value() << std::endl;
                }
            }
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
        std::unique_ptr<ContactInfo> ci = generateContactInfo(first, last, email);

        CreateContactArg arg;
        arg.setData(*(ci.get()));
        auto contacts_list = m_gt->getContacts()->create(arg);
        std::cout << contacts_list->toString() << std::endl;
        auto& arr = contacts_list->items();
        if (arr.size() > 0) {
            std::cout << "Created.." << std::endl;
            std::cout << arr[0]->toXml(m_c.userId()) << std::endl;
        }
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GcontactCommands::delete_contact(QString contactid)
{
    if (contactid.isEmpty()) {
        std::cout << "contactid required" << std::endl;
        return;
    }

    try
    {
        DeleteContactArg arg;
        arg.setContactId(contactid);
        arg.setIgnoreEtag(true);
        m_gt->getContacts()->deleteContact(arg);
        std::cout << "Contact " << contactid << " deleted" << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}


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

    n.setFamilyName(last); 
    n.setGivenName(first); 
    n.setFullName(first + " " + last);
    e1.setAddress(email);
    e1.setDisplayName(first + " " + last);
    e1.setPrimary(true);
    e1.setTypeLabel("home");
    e2.setAddress(QString("2") + email); 
    e2.setDisplayName(first + " " + last);
    e2.setPrimary(false);
    e2.setTypeLabel("work");
    p.setNumber("1-111-1111");
    p.setPrimary(true);
    o.setName("organization-name"); 
    o.setTitle("title-in-the-organization");
    a.setCity("Mountain View"); 
    a.setStreet("1600 Amphitheatre Pkwy");
    a.setRegion("CA");
    a.setPostcode("94043");
    a.setCountry("United States");
    a.setPrimary(true);

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
        if (!ci3.parseXml(ci2.parsedXml())) {
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

void GcontactCommands::test_concurrent_req()
{    
    TaskAggregator* agg = m_c.endpoint()->produceAggregatorTask();

    ContactListArg c;
    c.setMaxResults(100);
    c.setOrderby("lastmodified");
    c.setSortorder("descending");
    
    ContactGroupListArg g;

    auto tc = m_gt->getContacts()->list_Async(c);
    auto tg = m_gt->getContactGroup()->list_Async(g);
    agg->add(tc);
    agg->add(tg);
    
    try
    {
        agg->waitForResultAndRelease();
        auto clst = tc->detachResult();
        auto glst = tg->detachResult();
        print_group_list(glst.get());
        print_contact_list(clst.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GcontactCommands::ls_as_json()
{
    try
    {
        ContactListArg arg;
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

void GcontactCommands::print_last_result()
{
    m_c.printLastResponse();
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
        print_contact_list(&cl);
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
    o.setName("=NEW-organization-name="); 
    o.setTitle("=NEW-title-in-the-organization=");
    c.setOrganizationInfo(o);
    NameInfo n;
    n.setFamilyName("=NEW-last="); 
    n.setGivenName("=NEW-first");
    n.setFullName("=NEW-first_and_last=");
    c.setName(n);

    PhoneInfo p1, p2;
    std::vector<PhoneInfo> lst;
    p1.setNumber("=NEW-1-111-1111=");
    p1.setPrimary(true);
    p2.setNumber("=NEW-2-222-2222=");
    p2.setPrimary(false);
    lst.push_back(p1);
    lst.push_back(p2);
    c.replacePhones(lst);

    EmailInfo e1, e2;
    e1.setAddress("=NEW-first-email="); 
    e1.setDisplayName("=NEW-first-last="); 
    e1.setPrimary(true); 
    e1.setTypeLabel("home");
    e2.setAddress("=NEW-second-email=");
    e2.setDisplayName("=NEW-second-first-last=");
    e2.setPrimary(false);
    e2.setTypeLabel("work");
    std::vector<EmailInfo> e_lst;
    e_lst.push_back(e1);
    e_lst.push_back(e2);
    c.replaceEmails(e_lst);

    PostalAddress a1, a2;
    a1.setCity("=NEW-ADDR-1="); 
    a1.setStreet("=NEW-STREET1="); 
    a1.setRegion("=NEW-REGION1="); 
    a1.setPostcode("=NEW-ZIP1="); 
    a1.setCountry("=NEW-COUNTRY1="); 
    a1.setPrimary(true);
    a2.setCity("=NEW-ADDR-2="); 
    a2.setStreet("=NEW-STREET2="); 
    a2.setRegion("=NEW-REGION2="); 
    a2.setPostcode("=NEW-ZIP2="); 
    a2.setCountry("=NEW-COUNTRY2="); 
    a2.setPrimary(false);
    std::vector<PostalAddress> a_lst;
    a_lst.push_back(a1);
    a_lst.push_back(a2);
    c.replaceAddressList(a_lst);

    std::cout << c.mergedXml(c.parsedXml()) << std::endl << std::endl;
};

void GcontactCommands::update_contact_name(QString contactId_name) 
{
    ///first we get contact, make changes, merge them and upload modified xml
    QStringList arg_list = contactId_name.split(" ",
        QString::SkipEmptyParts);

    if (arg_list.size() < 3)
    {
        std::cout << "Invalid parameters, expected <contactId> <First> <Last>" << std::endl;
        return;
    }

    QString contactid = arg_list[0];
    QString first_name = arg_list[1];
    QString last_name = arg_list[2];

    NameInfo n;
    n.setGivenName(first_name); 
    n.setFamilyName(last_name);
    
    try
    {
        ContactListArg arg;
        arg.setContactId(contactid);
        auto contacts_list = m_gt->getContacts()->list(arg);        
        auto& arr = contacts_list->items();
        if (arr.size() > 0) {
            auto c = arr[0];
            c->setName(n);
            UpdateContactArg upd(*(c.get()));
            upd.setIgnoreEtag(true);
            auto c_list = m_gt->getContacts()->update(upd);
            auto& arr2 = c_list->items();
            if (arr2.size() > 0) {
                std::cout << "Updated.." << std::endl;
                std::cout << arr2[0]->toXml(m_c.userId()) << std::endl;
            }                        
        }
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GcontactCommands::download_photo(QString contactid) 
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
        if (!d.mkpath("download")) {
            std::cout << "Failed to create 'download' directory." << std::endl;
            return;
        }

        out.setFileName("download/" + contactid);
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
        std::cout << "Exception: " << e.what() << e.errSummary() << std::endl;
    }

    out.close();
};

void GcontactCommands::upload_photo(QString contactid_space_file_name)
{
    QStringList arg_list = contactid_space_file_name.split(" ",
        QString::SkipEmptyParts);

    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <contactId> <File-Name>" << std::endl;
        return;
    }

    QString contactId = arg_list[0];
    QString file_name = arg_list[1];

    QFile in;
    try
    {
        QDir d;
        if (!d.mkpath("upload")) {
            std::cout << "Failed to create 'upload' directory." << std::endl;
            return;
        }

        in.setFileName("upload/" + file_name);
        if (!in.open(QFile::ReadOnly)) {
            std::cout << "Error opening file: " << in.fileName() << std::endl;
            return;
        }

        UploadPhotoArg arg(contactId);
        m_gt->getContacts()->uploadContactPhoto(arg, &in);
        
        std::cout << "=== photo uploaded ===" << std::endl;
        std::cout << size_human(in.size()) << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    in.close();
};


void GcontactCommands::delete_photo(QString contactid)
{
    if (contactid.isEmpty()) {
        std::cout << "contactid required" << std::endl;
        return;
    }

    try
    {
        DeletePhotoArg arg;
        arg.setContactId(contactid);
        arg.setIgnoreEtag(true);
        m_gt->getContacts()->deleteContactPhoto(arg);
        std::cout << "Photo " << contactid << " deleted" << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}


void GcontactCommands::ls_groups()
{
    try
    {
        ContactGroupListArg arg;
        auto g_list = m_gt->getContactGroup()->list(arg);
        print_group_list(g_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void GcontactCommands::ls_groups_date(QString updatedMin) 
{
    std::function<void(void)> print_usage = []()
    {
        std::cout << "date time parameter required" << std::endl;
        std::cout << "example: ls_groups_date " << QDateTime::currentDateTime().toString(date_format) << std::endl;
    };

    if (updatedMin.isEmpty()) {
        print_usage();
        return;
    }

    QDateTime dt = QDateTime::fromString(updatedMin, date_format);
    if (!dt.isValid()) {
        print_usage();
        return;
    }

    std::cout << "using date time parameter " << dt.toString(date_format) << std::endl;

    try
    {
        ContactGroupListArg arg;
        arg.setUpdatedMin(dt);
        auto g_list = m_gt->getContactGroup()->list(arg);
        print_group_list(g_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }

};

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
        auto g_list = m_gt->getContactGroup()->list(arg);
        
        std::cout << g_list->toString() << std::endl;
        auto& arr = g_list->items();
        if (arr.size() > 0) {
            std::cout << arr[0]->toXml(m_c.userId()) << std::endl;
        }        
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void GcontactCommands::create_group(QString title_content)
{
    QStringList arg_list = title_content.split(" ",
        QString::SkipEmptyParts);

    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <Title> <Content>" << std::endl;
        return;
    }

    QString title = arg_list[0];
    QString content = arg_list[1];


    try
    {
        GroupInfo g;
        g.setTitle(title).setContent(content);

        CreateContactGroupArg arg;
        arg.setData(g);
        auto contacts_list = m_gt->getContactGroup()->create(arg);        
        std::cout << contacts_list->toString() << std::endl;
        auto& arr = contacts_list->items();
        if (arr.size() > 0) {
            std::cout << arr[0]->toXml(m_c.userId()) << std::endl;
        }

    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void GcontactCommands::delete_group(QString groupid)
{
    if (groupid.isEmpty()) {
        std::cout << "groupid required" << std::endl;
        return;
    }

    try
    {
        DeleteContactGroupArg arg;
        arg.setGroupId(groupid);
        arg.setIgnoreEtag(true);
        m_gt->getContactGroup()->deleteContactGroup(arg);
        std::cout << "Group " << groupid << " deleted" << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void GcontactCommands::update_group_title(QString groupId_title)
{
    ///first we get contact, make changes, merge them and upload modified xml
    QStringList arg_list = groupId_title.split(" ",
        QString::SkipEmptyParts);

    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <groupid> <Title>" << std::endl;
        return;
    }

    QString groupid = arg_list[0];
    QString new_title = arg_list[1];

    try
    {
        ContactGroupListArg arg;
        arg.setGroupId(groupid);
        auto contacts_list = m_gt->getContactGroup()->list(arg);
        auto& arr = contacts_list->items();
        if (arr.size() > 0) {
            auto g = arr[0];
            g->setTitle(new_title);

            UpdateContactGroupArg upd(*(g.get()));
            upd.setIgnoreEtag(true);
            auto contacts_list = m_gt->getContactGroup()->update(upd);
            //std::cout << contacts_list->toString() << std::endl;
            std::cout << "Updated: " << groupid << std::endl;
        }
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void GcontactCommands::ls_group_contacts(QString groupId)
{
    if (groupId.isEmpty()) {
        std::cout << "groupId required" << std::endl;
        return;
    }
   
    try
    {
        ContactListArg arg;
        arg.setMaxResults(100);
        arg.setOrderby("lastmodified");
        arg.setSortorder("descending");
        arg.setGroup(GcontactRoutes::encodeGroupUri(m_c.userId(), groupId));
        auto contacts_list = m_gt->getContacts()->list(arg);
        print_contact_list(contacts_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }    
};

void GcontactCommands::set_contact_groups(QString contactId_space_groupId)
{
    ///first we get contact, make changes, merge them and upload modified xml
    QStringList arg_list = contactId_space_groupId.split(" ",
        QString::SkipEmptyParts);

    if (arg_list.size() < 1)
    {
        std::cout << "Invalid parameters, expected <contactId> <groupId1>..." << std::endl;
        return;
    }

    QString contactid = arg_list[0];

    try
    {
        ContactListArg arg;
        arg.setContactId(contactid);
        auto contacts_list = m_gt->getContacts()->list(arg);
        auto& arr = contacts_list->items();
        if (arr.size() > 0) {
            auto c = arr[0];
            GroupMembershipInfoList& gm = c->groupsRef();
            gm.items().clear();
            
            int i = 1, Max = arg_list.size();
            for(; i < Max; i++){
                QString groupid = arg_list[i];
                GroupMembershipInfo m(m_c.userId(), groupid);
                c->addGroup(m);
            }
            
            UpdateContactArg upd(*(c.get()));
            upd.setIgnoreEtag(true);
            auto c_list = m_gt->getContacts()->update(upd);
            auto& arr2 = c_list->items();
            if (arr2.size() > 0) {
                std::cout << "Updated.." << std::endl;
                std::cout << arr2[0]->toXml(m_c.userId()) << std::endl;
            }
        }
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}

void GcontactCommands::parse_group_file(QString xmlFileName)
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


    GroupList cl;
    if (cl.parseXml(file.readAll())) {
        print_group_list(&cl);
    }
};

void GcontactCommands::print_contact_list(gcontact::ContactList* lst) 
{
    if (!lst)
        return;

    auto& arr = lst->items();
    int idx = 1;
    std::cout << "--contacts--------------------------------------------------" << std::endl;
    std::cout << "  #      ID              etag                       updated " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    for (auto& c : arr) {
        QString info = c->title();
        if (info.isEmpty()) {
            if (c->emails().items().size() > 0) {
                info = c->emails().items()[0].address();
            }
        }
        std::cout << std::setw(3) << idx++ << ". "
            << c->id() << " "
            << c->etag() << " "
            << c->updated().toString(date_format) << " "
            << info << std::endl;
    }
};

void GcontactCommands::print_group_list(gcontact::GroupList* lst)
{
    if (!lst)
        return;

    auto& arr = lst->items();
    int idx = 1;
    std::cout << "--groups----------------------------------------------------" << std::endl;
    std::cout << "  #      ID              etag                       updated " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    for (auto& c : arr) {
        QString info = c->title();
        info += "/";
        info += c->content();

        std::cout << std::setw(3) << idx++ << ". "
            << std::setw(16) << c->id() << " "
            << std::setw(32) << c->etag() << " "
            << c->updated().toString(date_format) << " "
            << info << std::endl;
    }
};

void GcontactCommands::print_batch_contact_result(const gcontact::BatchContactList* lst)
{
    if (!lst)
        return;
  
    auto& arr = lst->items();
    int idx = 1;
    std::cout << "--contacts--------------------------------------------------" << std::endl;
    std::cout << "  #      ID              status                     updated " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    for (auto& c : arr) {
        QString info = c->title();
        if (info.isEmpty()) {
            if (c->emails().items().size() > 0) {
                info = c->emails().items()[0].address();
            }
        }
        std::cout << std::setw(3) << idx++ << ". "
            << std::setw(16) << c->id() << " "
            << (c->succeded() ? "OK" : "ERR") << " "
            << c->batchResultOperationType() << "/" << c->batchResultStatusReason() << " "
            << c->updated().toString(date_format) << " "
            << info << std::endl;
    }
};


void GcontactCommands::print_batch_group_result(const gcontact::BatchGroupList* lst)
{
    if (!lst)
        return;

    auto& arr = lst->items();
    int idx = 1;
    std::cout << "--groups----------------------------------------------------" << std::endl;
    std::cout << "  #      ID              status                     updated " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    for (auto& c : arr) {
        QString info = c->title();
        info += "/";
        info += c->content();

        std::cout << std::setw(3) << idx++ << ". "
            << std::setw(16) << c->id() << " "
            << (c->succeded() ? "OK" : "ERR") << " "
            << c->batchResultOperationType() << "/" << c->batchResultStatusReason() << " "
            << c->updated().toString(date_format) << " "
            << info << std::endl;
    }
};

static QString cacheStatus2Str(gcontact::ContactXmlPersistant::EStatus st)
{
    QString rv;
    switch(st)
        {
        case gcontact::ContactXmlPersistant::localCopy:
            {
                rv = "[-]";
            }break;
        case gcontact::ContactXmlPersistant::localModified:
            {
                rv = "[M]";
            }break;
        case gcontact::ContactXmlPersistant::localRemoved:
            {
                rv = "[D]";
            }break;
        case gcontact::ContactXmlPersistant::localRetired:
            {
                rv = "[R]";
            }break;
        case gcontact::ContactXmlPersistant::localIdLimbo:
            {
                rv = "[L]";
            }break;            
        };
    return rv;
}

void print_cache_contact_info(ContactInfo* c, int idx)
{
    QString info = c->title();
    if (info.isEmpty()) {
        if (c->emails().items().size() > 0) {
            info = c->emails().items()[0].address();
        }
    }

    QString dirty_mark = "[ ]";
    if(c->isDirty()){
        dirty_mark = "[*]";
    }

    if(idx > 0){
        std::cout << std::setw(3) << idx++ << ". ";
    }
    
    std::cout << cacheStatus2Str(c->status()) << " "
              << dirty_mark << " "
              << c->id() << " "
              << c->etag() << " "
              << c->updated().toString(date_format) << " "
              << info << std::endl;

};

void GcontactCommands::print_cache_contact_list(const gcontact::ContactList* lst) 
{
    if (!lst)
        return;

    auto& arr = lst->items();
    int idx = 1;
    std::cout << "--contacts--------------------------------------------------" << std::endl;
    std::cout << "  #      ID              etag                       updated " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    for (auto& c : arr) {
        print_cache_contact_info(c.get(), idx++);
    }
};

void GcontactCommands::print_cache_group_list(const gcontact::GroupList* lst)
{
    if (!lst)
        return;

    auto& arr = lst->items();
    int idx = 1;
    std::cout << "--groups----------------------------------------------------" << std::endl;
    std::cout << "  #      ID              etag                       updated " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    for (auto& c : arr) {
        QString info = c->title();
        info += "/";
        info += c->content();

        QString dirty_mark = "[ ]";
        if(c->isDirty()){
            dirty_mark = "[*]";
        }

        
        std::cout << std::setw(3) << idx++ << ". "
                  << cacheStatus2Str(c->status()) << " "
                  << dirty_mark << " "
                  << std::setw(16) << c->id() << " "
                  << std::setw(32) << c->etag() << " "
                  << c->updated().toString(date_format) << " "
                  << info << std::endl;
    }
};

std::unique_ptr<gcontact::ContactInfo> GcontactCommands::generateContactInfo(QString first, QString last, QString email)
{
    std::unique_ptr<ContactInfo> ci(new ContactInfo());
    NameInfo n;
    EmailInfo e;
    PhoneInfo p;
    OrganizationInfo o;
    PostalAddress a;

    n.setFamilyName(last); 
    n.setGivenName(first); 
    n.setFullName(first + " " + last);
    e.setAddress(email);
    e.setDisplayName(first + " " + last);
    e.setPrimary(true);
    e.setTypeLabel("home");
    p.setNumber("1-111-1111");
    p.setPrimary(true);
    o.setName("organization-name"); 
    o.setTitle("title-in-the-organization");
    a.setCity("Mountain View");
    a.setStreet("1600 Amphitheatre Pkwy");
    a.setRegion("CA");
    a.setPostcode("94043");
    a.setCountry("United States");
    a.setPrimary(true);

    ci->setName(n).setTitle("Title for " + first + " " + last)
        .addEmail(e)
        .addPhone(p)
        .setContent(QString("My notest on new contact for '%1'").arg(first))
        .setOrganizationInfo(o)
        .addAddress(a);

    return ci;
};

std::unique_ptr<gcontact::GroupInfo> GcontactCommands::generateGroupInfo(QString title, QString content)
{
    std::unique_ptr<GroupInfo> g(new GroupInfo());
    g->setTitle(title).setContent(content);
    return g;
};


void GcontactCommands::batch_list_contacts(QString id_space_id)
{
    if (id_space_id.isEmpty()) {
        std::cout << "Invalid parameters, expected <id> <id>" << std::endl;
        return;
    }

    QStringList arg_list = id_space_id.split(" ",
        QString::SkipEmptyParts);

    try
    {
        BatchRequesContactList batch_list;
        for (auto s : arg_list)
        {
            std::unique_ptr<BatchRequestContactInfo> ci(BatchRequestContactInfo::buildRequest(s, googleQt::EBatchId::retrieve));
            batch_list.add(std::move(ci));
        }
        BatchContactArg arg(batch_list);
        auto c_list = m_gt->getContacts()->batch(arg);
        print_last_result();
        print_batch_contact_result(c_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GcontactCommands::batch_create_contact(QString name_space_name)
{
    if (name_space_name.isEmpty()) {
        std::cout << "Invalid parameters, expected <First-Name> <First-Name>" << std::endl;
        std::cout << "Example: batch_create_contact john tom mike" << std::endl;
        return;
    }

    QStringList arg_list = name_space_name.split(" ",
        QString::SkipEmptyParts);

    try
    {
        BatchRequesContactList batch_list;
        for (auto s : arg_list)
        {
            QString first = s;
            QString last = "4batch";
            QString email = "me@gmail.com";

            std::unique_ptr<ContactInfo> ci = generateContactInfo(first, last, email);
            std::unique_ptr<BatchRequestContactInfo> b = ci->buildBatchRequest(googleQt::EBatchId::create);
            batch_list.add(std::move(b));
        }
        BatchContactArg arg(batch_list);
        auto c_list = m_gt->getContacts()->batch(arg);
        print_batch_contact_result(c_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};


void GcontactCommands::batch_update_contact(QString id_space_id)
{
    if (id_space_id.isEmpty()) {
        std::cout << "Invalid parameters, expected <id> <id>" << std::endl;
        return;
    }

    QStringList arg_list = id_space_id.split(" ",
        QString::SkipEmptyParts);


    try
    {
        BatchRequesContactList batch_list;
        for (auto s : arg_list)
        {
            std::unique_ptr<BatchRequestContactInfo> ci(BatchRequestContactInfo::buildRequest(s, googleQt::EBatchId::retrieve));
            batch_list.add(std::move(ci));
        }

        BatchRequesContactList batch_upd;

        BatchContactArg arg(batch_list);
        auto rlst = m_gt->getContacts()->batch(arg);
        auto result_list = rlst.get();
        for (auto& c : result_list->items()) {
            std::unique_ptr<BatchRequestContactInfo> b = c->buildBatchRequest(googleQt::EBatchId::update);

            auto n = b->name();
            n.setFullName(n.fullName() + "-b");
            b->setName(n);
            batch_upd.add(std::move(b));
        }

        BatchContactArg arg2(batch_upd);
        auto c_list = m_gt->getContacts()->batch(arg2);
        print_batch_contact_result(c_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};


void GcontactCommands::batch_delete_contact(QString id_space_id)
{
    if (id_space_id.isEmpty()) {
        std::cout << "Invalid parameters, expected <id> <id>" << std::endl;
        return;
    }

    QStringList arg_list = id_space_id.split(" ",
        QString::SkipEmptyParts);

    try
    {
        BatchRequesContactList batch_list;
        for (auto s : arg_list)
        {
            std::unique_ptr<BatchRequestContactInfo> ci(BatchRequestContactInfo::buildRequest(s, googleQt::EBatchId::delete_operation));
            batch_list.add(std::move(ci));
        }
        BatchContactArg arg(batch_list);
        auto c_list = m_gt->getContacts()->batch(arg);
        std::cout << "contact deleted" << std::endl;
        print_batch_contact_result(c_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};


void GcontactCommands::batch_list_groups(QString id_space_id)
{
    if (id_space_id.isEmpty()) {
        std::cout << "Invalid parameters, expected <id> <id>" << std::endl;
        return;
    }

    QStringList arg_list = id_space_id.split(" ",
        QString::SkipEmptyParts);

    try
    {
        BatchRequesGroupList batch_list;
        for (auto s : arg_list)
        {
            std::unique_ptr<BatchRequestGroupInfo> ci(BatchRequestGroupInfo::buildRequest(s, googleQt::EBatchId::retrieve));
            batch_list.add(std::move(ci));            
        }
        BatchGroupArg arg(batch_list);
        auto g_list = m_gt->getContactGroup()->batch(arg);
        print_batch_group_result(g_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};


void GcontactCommands::batch_create_group(QString name_space_name) 
{
    if (name_space_name.isEmpty()) {
        std::cout << "Invalid parameters, expected <Name> <Name>" << std::endl;
        std::cout << "Example: batch_create_group group1 group2 group3" << std::endl;
        return;
    }

    QStringList arg_list = name_space_name.split(" ",
        QString::SkipEmptyParts);

    try
    {
        BatchRequesGroupList batch_list;
        for (auto s : arg_list)
        {
            QString title = s;
            QString content = "4batch";

            std::unique_ptr<GroupInfo> ci = generateGroupInfo(title, content);
            std::unique_ptr<BatchRequestGroupInfo> b = ci->buildBatchRequest(googleQt::EBatchId::create);
            batch_list.add(std::move(b));
        }
        BatchGroupArg arg(batch_list);
        auto g_list = m_gt->getContactGroup()->batch(arg);
        print_batch_group_result(g_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GcontactCommands::batch_update_group(QString id_space_id)
{
    if (id_space_id.isEmpty()) {
        std::cout << "Invalid parameters, expected <id> <id>" << std::endl;
        return;
    }

    QStringList arg_list = id_space_id.split(" ",
        QString::SkipEmptyParts);


    try
    {
        BatchRequesGroupList batch_list;
        for (auto s : arg_list)
        {
            std::unique_ptr<BatchRequestGroupInfo> b(BatchRequestGroupInfo::buildRequest(s, googleQt::EBatchId::retrieve));
            //std::unique_ptr<GroupInfo> ci(GroupInfo::createWithId(s));
            //ci->setBatchid(googleQt::EBatchId::retrieve);
            batch_list.add(std::move(b));
        }

        BatchRequesGroupList batch_upd;

        BatchGroupArg arg(batch_list);
        auto rlst = m_gt->getContactGroup()->batch(arg);
        std::cout << "groups before update.." << std::endl;
        print_batch_group_result(rlst.get());
        
        //GroupList* result_list = rlst.get();
        for (auto& c : rlst->items()) {
            std::unique_ptr<BatchRequestGroupInfo> b = c->buildBatchRequest(googleQt::EBatchId::update);
            b->setTitle(b->title() + "-b");
            batch_upd.add(std::move(b));
        }
        
        BatchGroupArg arg2(batch_upd);
        auto c_list = m_gt->getContactGroup()->batch(arg2);
        std::cout << "data after update.." << std::endl;
        print_batch_group_result(c_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};



void GcontactCommands::batch_delete_group(QString id_space_id)
{
    if (id_space_id.isEmpty()) {
        std::cout << "Invalid parameters, expected <id> <id>" << std::endl;
        return;
    }

    QStringList arg_list = id_space_id.split(" ",
        QString::SkipEmptyParts);

    try
    {
        BatchRequesGroupList batch_list;
        for (auto s : arg_list)
        {
            std::unique_ptr<BatchRequestGroupInfo> ci(BatchRequestGroupInfo::buildRequest(s, googleQt::EBatchId::delete_operation));
            batch_list.add(std::move(ci));
        }
        BatchGroupArg arg(batch_list);
        auto g_list = m_gt->getContactGroup()->batch(arg);
        std::cout << "groups deleted" << std::endl;
        print_batch_group_result(g_list.get());
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void print_cache_contact_info(ContactInfo* ci, int idx);

void GcontactCommands::cache_ls()
{    
    auto c = m_c.contacts_cache();
    c->loadContactsFromDb();
    ContactList& cl = c->contacts();
    GroupList& gl = c->groups();

    print_cache_contact_list(&cl);
    print_cache_group_list(&gl);
};


void GcontactCommands::cache_get_contact(QString cid)
{
	auto c = m_c.contacts_cache();
    c->loadContactsFromDb();
    ContactList& cl = c->contacts();

    auto ct = cl.findById(cid);
    if(!ct){
        std::cout << "not found" << std::endl;
    }
    else{
        print_cache_contact_info(ct.get(), 0);
    }
};

void GcontactCommands::cache_update(QString id_space_id)
{
    QStringList arg_list = id_space_id.split(" ",
        QString::SkipEmptyParts);

    if (arg_list.empty())
    {
        std::cout << "Invalid parameters, expected <id1> <id2>" << std::endl;
        return;
    }
    
    std::set<QString> idset;
    for(auto s : arg_list){
        idset.insert(s);
    }

    int mod_idx = 0;
    
	auto c = m_c.contacts_cache();
    ContactList& cl = c->contacts();
    auto& arr = cl.items();
    for(auto& c : arr){
        if(idset.find(c->id()) != idset.end()){
            auto n = c->name();
            n.setFullName(n.fullName() + "-c");
            c->setName(n);

            std::cout << "updating " << c->id() << " " << n.fullName() << std::endl;            
            mod_idx++;
        }
    }

    c->storeContactsToDb();
    std::cout << "modified " << mod_idx << " contact(s)" << std::endl;
};


void GcontactCommands::cache_delete(QString id_space_id)
{
    QStringList arg_list = id_space_id.split(" ",
        QString::SkipEmptyParts);

    if (arg_list.empty())
    {
        std::cout << "Invalid parameters, expected <id1> <id2>" << std::endl;
        return;
    }
    
    std::set<QString> idset;
    for(auto s : arg_list){
        idset.insert(s);
    }

    int mod_idx = 0;
    
    auto c = m_c.contacts_cache();
    ContactList& cl = c->contacts();
    for(auto& cid : idset){
        auto ct = cl.findById(cid);
        if(ct){
            std::cout << "removing " << ct->id() << " " << ct->name().fullName() << std::endl;
            cl.remove(ct);
            mod_idx++;
        }
        else{
            std::cout << "not found " << ct->id() << " " << ct->name().fullName() << std::endl;
        }
    }

    c->storeContactsToDb();
    std::cout << "deleted " << mod_idx << " contact(s)" << std::endl;
};

void GcontactCommands::resolve_cache_photo(QString contactid)
{
    if (contactid.isEmpty()) {
        std::cout << "contactid required" << std::endl;
        return;
    }

    auto r = m_gt->cacheRoutes(); 
    auto cache = m_c.contacts_cache();
    auto c = cache->contacts().findById(contactid);
    if(!c){
        std::cout << "Contact not found: " << contactid << std::endl;
        return;
    }

    auto t = r->getContactCachePhoto_Async(c);
    try
    {
        auto s = t->waitForResultAndRelease();
        std::cout << "Photo File " << s.get() << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    } 
};

void GcontactCommands::sync_cache_photos()
{    
    auto cache = m_c.contacts_cache();
    STRING_LIST lst_unresolved = cache->contacts().buildUnresolvedPhotoIdList();
    std::cout << "===unresolved photos===" << std::endl;
    for(auto s : lst_unresolved){
        std::cout << s << std::endl;
    }

    STRING_LIST lst_modified = cache->contacts().buildModifiedPhotoIdList();
    std::cout << "===modified photos===" << std::endl;
    for(auto s : lst_modified){
        std::cout << s << std::endl;
    }
    
    std::cout << "=============" << std::endl;
    
    auto t = m_gt->cacheRoutes()->synchronizePhotos_Async();
    try
    {
        t->waitForResultAndRelease();
        std::cout << "Photos synchronized downloaded=" << t->downloaded().size() << " uploaded=" << t->uploaded().size() << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};

void GcontactCommands::add_cache_photo(QString id_space_file_name)
{
    QStringList arg_list = id_space_file_name.split(" ",
        QString::SkipEmptyParts);

    if (arg_list.size() < 2)
    {
        std::cout << "Invalid parameters, expected <contactId> <File-Name>" << std::endl;
        return;
    }

    QString contactId = arg_list[0];
    QString file_name = arg_list[1];
    QString full_file_name = "photo-templates/" + file_name;

    auto cache = m_c.contacts_cache();
    auto c = cache->contacts().findById(contactId);
    if (!c) {
        std::cout << "contact not found " << contactId << std::endl;
        return;
    }

    if (!QFile::exists(full_file_name)) {
        std::cout << "File not found " << full_file_name << std::endl;
        return;
    }

    if (cache->addPhoto(c, full_file_name)) {
        std::cout << "photo added " << full_file_name << std::endl;        
    }
    else {
        std::cout << "failed to add photo " << full_file_name << std::endl;
    } 

    cache->storeContactsToDb();
};

void GcontactCommands::cache_sync()
{
    try
        {
            auto r = m_gt->cacheRoutes();
            auto t = r->synchronizeContacts_Async();
            t->waitForResultAndRelease();
            auto c = m_c.contacts_cache();
            
            ContactList& cl = c->contacts();
            GroupList& gl = c->groups();

            if(t->loadedContacts()){
                std::cout << "=== loaded entries: " << t->loadedContacts()->items().size() << std::endl;
                if(t->loadedContacts()->items().size() > 0){
                    print_cache_contact_list(t->loadedContacts());
                }
            }

            if(t->loadedGroups()){
                std::cout << "=== loaded groups: " << t->loadedGroups()->items().size() << std::endl;
                if(t->loadedGroups()->items().size() > 0){
                    print_cache_group_list(t->loadedGroups());
                }
            }

            if(t->updatedContacts()){
                std::cout << "=== updated entries: " << t->updatedContacts()->items().size() << std::endl;
                if(t->updatedContacts()->items().size() > 0){
                    print_batch_contact_result(t->updatedContacts());
                }
            }

            if(t->updatedGroups()){
                std::cout << "=== updated groups: " << t->updatedGroups()->items().size() << std::endl;
                if(t->updatedGroups()->items().size() > 0){
                    print_batch_group_result(t->updatedGroups());
                }
            }
                        
            std::cout << "=== entries-in-cache: " << cl.items().size() << std::endl;
            std::cout << "=== groups-in-cache: " << gl.items().size() << std::endl;
        }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }            
};

