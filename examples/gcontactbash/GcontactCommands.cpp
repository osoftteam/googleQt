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
        std::cout << contacts_list->toString() << std::endl;
        auto& arr = contacts_list->items();
        if(arr.size() > 0){
            std::cout << arr[0]->toXml(m_c.userId()) << std::endl;
            std::cout << "------------------" << std::endl;
            std::cout << arr[0]->originalXml() << std::endl;
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

    std::cout << c.mergedXml(c.originalXml()) << std::endl << std::endl;
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
    n.setGivenName(first_name).setFamilyName(last_name);
    
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
        if (!d.mkpath("uploads")) {
            std::cout << "Failed to create 'uploads' directory." << std::endl;
            return;
        }

        in.setFileName("uploads/" + file_name);
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
    auto& arr = lst->items();
    int idx = 1;
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "  #      ID              etag                       updated " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    for (auto& c : arr) {
        QString info = c->title();
        if (info.isEmpty()) {
            if (c->emails().size() > 0) {
                info = c->emails()[0].address();
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
    auto& arr = lst->items();
    int idx = 1;
    std::cout << "------------------------------------------------------------" << std::endl;
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

void GcontactCommands::print_batch_contact_result(gcontact::BatchContactList* lst)
{
    auto& arr = lst->items();
    int idx = 1;
    std::cout << "------------------------------------------------------------" << std::endl;
    std::cout << "  #      ID              status                     updated " << std::endl;
    std::cout << "------------------------------------------------------------" << std::endl;
    for (auto& c : arr) {
        QString info = c->title();
        if (info.isEmpty()) {
            if (c->emails().size() > 0) {
                info = c->emails()[0].address();
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


void GcontactCommands::print_batch_group_result(gcontact::BatchGroupList* lst)
{
    auto& arr = lst->items();
    int idx = 1;
    std::cout << "------------------------------------------------------------" << std::endl;
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


std::unique_ptr<gcontact::ContactInfo> GcontactCommands::generateContactInfo(QString first, QString last, QString email)
{
    std::unique_ptr<ContactInfo> ci(new ContactInfo());
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
        m_gt->getContactGroup()->batch(arg);
        std::cout << "groups deleted" << std::endl;
    }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
};


void GcontactCommands::sync_contacts()
{
    try
        {
            auto r = m_gt->cacheRoutes();
            auto t = r->synchronizeContacts_Async();
            t->waitForResultAndRelease();
            auto c = r->cache();
            
            ContactList& cl = c->contacts();
            GroupList& gl = c->groups();

            std::cout << "entries: " << cl.items().size() << std::endl;
            std::cout << "groups: " << gl.items().size() << std::endl;            
        }
    catch (GoogleException& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }            
};
