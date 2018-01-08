#include <QSqlError>
#include <QDebug>
#include <ctime>
#include <QDir>
#include <ostream>
#include "GcontactCache.h"
#include "GcontactRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailCache.h"

using namespace googleQt;
using namespace gcontact;

#define COMPARE_NO_CASE(N) if (N.compare(o.N, Qt::CaseInsensitive)){return false;}

/**
   NameInfo
*/
NameInfo::NameInfo() 
{
};

NameInfo NameInfo::parse(QDomNode n)
{
    NameInfo rv;        
    rv.m_is_null = true;
    QDomElement elem_name = n.firstChildElement("gd:name");
    if (!elem_name.isNull()) {
        rv.m_is_null = false;
        rv.m_fullName = elem_name.firstChildElement("gd:fullName").text().trimmed();
        rv.m_givenName = elem_name.firstChildElement("gd:givenName").text().trimmed();
        rv.m_familyName = elem_name.firstChildElement("gd:familyName").text().trimmed();
    }
    return rv;
}

QString NameInfo::toString()const
{
    QString s = "";
    if (!isNull()) {
        s = QString("name=%1,%2,%3").arg(fullName()).arg(givenName()).arg(familyName());
    }
    return s;
};

bool NameInfo::operator==(const NameInfo& o) const 
{
    COMPARE_NO_CASE(m_fullName);
    COMPARE_NO_CASE(m_givenName);
    COMPARE_NO_CASE(m_familyName);
    return true;
};

bool NameInfo::operator!=(const NameInfo& o) const 
{
    return !(*this == o);
};

/**
    OrganizationInfo
*/
OrganizationInfo::OrganizationInfo()
{
};

OrganizationInfo OrganizationInfo::parse(QDomNode n)
{
    OrganizationInfo rv;
    rv.m_is_null = true;
    QDomElement elem_organization = n.firstChildElement("gd:organization");
    if (!elem_organization.isNull()) {
        rv.m_is_null = false;
        rv.m_name = elem_organization.firstChildElement("gd:orgName").text().trimmed();
        rv.m_title = elem_organization.firstChildElement("gd:orgTitle").text().trimmed();
    }
    return rv;
}

QString OrganizationInfo::toString()const 
{
    QString s = "";
    if (!isNull()) {
        s = QString("organization=%1,%2").arg(name()).arg(title());
    }
    return s;
};

QString OrganizationInfo::toXmlString()const 
{
    QString s = "";
    if (!isNull()) {
        s += QString("<gd:organization rel = \"http://schemas.google.com/g/2005#%1\">\n").arg("other");
        s += QString("    <gd:orgName>%1</gd:orgName>\n").arg(m_name);
        s += QString("    <gd:orgTitle>%1</gd:orgTitle>\n").arg(m_title);
        s += "</gd:organization>\n";
    }
    return s;
};

bool OrganizationInfo::operator==(const OrganizationInfo& o) const
{
    COMPARE_NO_CASE(m_name);
    COMPARE_NO_CASE(m_title);
    return true;
};

bool OrganizationInfo::operator!=(const OrganizationInfo& o) const
{
    return !(*this == o);
};


/**
PostalAddress
*/
PostalAddress::PostalAddress() :m_type_label("home")
{
};

QString PostalAddress::toString()const
{
    QString s = "";
    if (!isNull()) {
        s = QString("postalAddress=%1 %2 %3 %4 %5")
            .arg(m_city)
            .arg(m_street)
            .arg(m_region)
            .arg(m_postcode)
            .arg(m_country);
    }
    return s;
};

bool PostalAddress::operator==(const PostalAddress& o) const
{
    COMPARE_NO_CASE(m_city);
    COMPARE_NO_CASE(m_street);
    COMPARE_NO_CASE(m_region);
    COMPARE_NO_CASE(m_postcode);
    COMPARE_NO_CASE(m_country);
    COMPARE_NO_CASE(m_type_label);
    COMPARE_NO_CASE(m_formattedAddress);
    if (m_is_primary != o.m_is_primary)
        return false;
    return true;
};

bool PostalAddress::operator!=(const PostalAddress& o) const
{
    return !(*this == o);
};

/**
    PhoneInfo
*/
PhoneInfo::PhoneInfo() :m_type_label("other")
{
};

QString PhoneInfo::toString()const 
{
    QString s = "";
    if (!isNull()) {
        s = QString("phone=%1,%2,%3").arg(number()).arg(uri()).arg(typeLabel());
        if (m_is_primary) {
            s += ", primary=\"true\"";
        }
    }
    return s;
};


bool PhoneInfo::operator==(const PhoneInfo& o) const 
{
    COMPARE_NO_CASE(m_number);
    COMPARE_NO_CASE(m_uri);
    COMPARE_NO_CASE(m_type_label);
    if (m_is_primary != o.m_is_primary)
        return false;
    return true;

};

bool PhoneInfo::operator!=(const PhoneInfo& o) const 
{
    return !(*this == o);
};


/**
    EmailInfo
*/
EmailInfo::EmailInfo() :m_type_label("other")
{

};

QString EmailInfo::toString()const
{
    QString s = "";
    if (!isNull()) {
        s = QString("email=%1,%2,%3").arg(address()).arg(displayName()).arg(typeLabel());
        if (m_is_primary) {
            s += ", primary=\"true\"";
        }
    }
    return s;
};

bool EmailInfo::operator==(const EmailInfo& o) const
{
    COMPARE_NO_CASE(m_address);
    COMPARE_NO_CASE(m_display_name);
    COMPARE_NO_CASE(m_type_label);
    if (m_is_primary != o.m_is_primary)
        return false;
    return true;
};

bool EmailInfo::operator!=(const EmailInfo& o) const
{
    return !(*this == o);
};


/**
    EmailInfoList
*/
EmailInfoList EmailInfoList::parse(QDomNode n)
{
    EmailInfoList rv;    
    QDomElement first_email_elem = n.firstChildElement("gd:email");
    QDomElement email_elem = first_email_elem;
    while (!email_elem.isNull()) {
        EmailInfo email_info;
        email_info.m_is_null = false;
        QDomNamedNodeMap attr_names = email_elem.attributes();
        if (attr_names.size() > 0) {
            for (int j = 0; j < attr_names.size(); j++) {
                QDomNode n2 = attr_names.item(j);
                if (n2.nodeType() == QDomNode::AttributeNode) {
                    if (n2.nodeName().compare("address") == 0) {
                        email_info.m_address = n2.nodeValue().trimmed();
                    }
                    else  if (n2.nodeName().compare("displayName") == 0) {
                        email_info.m_display_name = n2.nodeValue().trimmed();
                    }
                    else if (n2.nodeName().compare("primary") == 0) {
                        QString s = n2.nodeValue().trimmed();
                        email_info.m_is_primary = (s.indexOf("true") != -1);
                    }
                    else if (n2.nodeName().compare("rel") == 0) {
                        QStringList slist = n2.nodeValue().split("#");
                        if (slist.size() == 2) {
                            email_info.m_type_label = slist[1];
                        }
                    }
                }
            }
        }

        rv.m_parts.push_back(email_info);
        email_elem = email_elem.nextSiblingElement("gd:email");
    }

    return rv;
}

QString EmailInfoList::toXmlString()const
{
    QString s = "";
    for (auto& p : m_parts) {
        if (!p.isNull()) {
            QString s_is_primary = "";
            QString displayName = "";
            QString address = "";
            if (!p.displayName().isEmpty()) {
                displayName = QString(" displayName=\"%1\"").arg(p.displayName());
            }
            if (p.isPrimary()) {
                s_is_primary = " primary=\"true\"";
            }
            address = QString(" address=\"%1\"").arg(p.address());
            s += QString("<gd:email rel = \"http://schemas.google.com/g/2005#%1\"%2 %3%4/>\n")
                .arg(p.m_type_label)
                .arg(s_is_primary)
                .arg(address)
                .arg(displayName);
        }
    }
    return s;
};



/**
    PhoneInfoList
*/
PhoneInfoList PhoneInfoList::parse(QDomNode n)
{
    PhoneInfoList rv;    
    QDomElement first_phone_elem = n.firstChildElement("gd:phoneNumber");
    QDomElement phone_elem = first_phone_elem;
    while (!phone_elem.isNull()) {
        PhoneInfo phone_info;
        phone_info.m_number = phone_elem.text().trimmed();
        phone_info.m_is_null = false;
        QDomNamedNodeMap attr_names = phone_elem.attributes();
        if (attr_names.size() > 0) {
            for (int j = 0; j < attr_names.size(); j++) {
                QDomNode n2 = attr_names.item(j);
                if (n2.nodeType() == QDomNode::AttributeNode) {
                    if (n2.nodeName().compare("uri") == 0) {
                        phone_info.m_uri = n2.nodeValue().trimmed();
                    }
                    else if (n2.nodeName().compare("primary") == 0) {
                        QString s = n2.nodeValue().trimmed();
                        phone_info.m_is_primary = (s.indexOf("true") != -1);
                    }
                    else if (n2.nodeName().compare("rel") == 0) {
                        QStringList slist = n2.nodeValue().trimmed().split("#");
                        if (slist.size() == 2) {
                            phone_info.m_type_label = slist[1];
                        }
                    }
                }
            }
        }

        rv.m_parts.push_back(phone_info);
        phone_elem = phone_elem.nextSiblingElement("gd:phoneNumber");
    }

    return rv;
}

QString PhoneInfoList::toXmlString()const 
{
    QString s = "";
    for (auto& p : m_parts) {
        if (!p.isNull()) {
            QString s_is_primary = "";
            if (p.m_is_primary) {
                s_is_primary = " primary=\"true\"";
            }
            s += QString("<gd:phoneNumber rel = \"http://schemas.google.com/g/2005#%1\"%2>\n").arg(p.m_type_label).arg(s_is_primary);
            s += QString("    %1\n").arg(p.m_number);
            s += "</gd:phoneNumber>\n";
        }
    }
    return s;
};

/**
PostalAddressList
*/
PostalAddressList PostalAddressList::parse(QDomNode n)
{
    PostalAddressList rv;
    
    QDomElement first_address_elem = n.firstChildElement("gd:structuredPostalAddress");
    QDomElement address_elem = first_address_elem;
    while (!address_elem.isNull()) {
        PostalAddress address;
        address.m_is_null = true;

        QDomNamedNodeMap attr_names = address_elem.attributes();
        if (attr_names.size() > 0) {
            for (int j = 0; j < attr_names.size(); j++) {
                QDomNode n2 = attr_names.item(j);
                if (n2.nodeType() == QDomNode::AttributeNode) {
                    if (n2.nodeName().compare("rel") == 0) {
                        QStringList slist = n2.nodeValue().trimmed().split("#");
                        if (slist.size() == 2) {
                            address.m_type_label = slist[1];
                        }
                    }
                    else if (n2.nodeName().compare("primary") == 0) {
                        QString s = n2.nodeValue().trimmed();
                        address.m_is_primary = (s.indexOf("true") != -1);
                    }
                }
            }
        }

        address.m_is_null = false;
        address.m_city = address_elem.firstChildElement("gd:city").text().trimmed();
        address.m_street = address_elem.firstChildElement("gd:street").text().trimmed();
        address.m_region = address_elem.firstChildElement("gd:region").text().trimmed();
        address.m_postcode = address_elem.firstChildElement("gd:postcode").text().trimmed();
        address.m_country = address_elem.firstChildElement("gd:country").text().trimmed();
        address.m_formattedAddress = address_elem.firstChildElement("gd:formattedAddress").text().trimmed();


        rv.m_parts.push_back(address);
        address_elem = address_elem.nextSiblingElement("gd:structuredPostalAddress");
    }

    return rv;
}

QString PostalAddressList::toXmlString()const 
{
    QString s = "";
    for (auto& p : m_parts) {
        if (!p.isNull()) {
            QString s_is_primary = "";
            if (p.isPrimary()) {
                s_is_primary = " primary=\"true\"";
            }
            s += QString("<gd:structuredPostalAddress rel = \"http://schemas.google.com/g/2005#%1\"%2>\n")
                .arg(p.m_type_label)
                .arg(s_is_primary);

            s += QString("    <gd:city>%1</gd:city>\n").arg(p.m_city);
            s += QString("    <gd:street>%1</gd:street>\n").arg(p.m_street);
            s += QString("    <gd:region>%1</gd:region>\n").arg(p.m_region);
            s += QString("    <gd:postcode>%1</gd:postcode>\n").arg(p.m_postcode);
            s += QString("    <gd:country>%1</gd:country>\n").arg(p.m_country);
            s += QString("    <gd:formattedAddress>%1</gd:formattedAddress>\n").arg(p.m_formattedAddress);

            s += "</gd:structuredPostalAddress>\n";
        }
    }
    return s;
};

static QString getAttribute(const QDomNode& n, QString name)
{
    QString rv = "";

    QDomNamedNodeMap attr_names = n.attributes();
    for (int j = 0; j < attr_names.size(); j++) {
        QDomNode n2 = attr_names.item(j);
        if (n2.nodeType() == QDomNode::AttributeNode &&
            n2.nodeName().compare(name) == 0) {
            rv = n2.nodeValue().trimmed();
            break;
        }
    }

    rv = rv.remove("\"");

    return rv;
}


/**
    ContactInfo
*/
ContactInfo& ContactInfo::setTitle(QString val)
{
    m_title = val;
    return *this;
};

ContactInfo& ContactInfo::setContent(QString notes) 
{
    m_content = notes;
    return *this;
};

ContactInfo& ContactInfo::setName(const NameInfo& n)
{
    m_name = n;
    return *this;
};

ContactInfo& ContactInfo::addEmail(const EmailInfo& e)
{
    m_emails.m_parts.push_back(e);
    return *this;
};

ContactInfo& ContactInfo::addPhone(const PhoneInfo& p) 
{
    m_phones.m_parts.push_back(p);
    return *this;
};

ContactInfo& ContactInfo::addAddress(const PostalAddress& p)
{
    m_address_list.m_parts.push_back(p);
    return *this;
};

ContactInfo& ContactInfo::setOrganizationInfo(const OrganizationInfo& o) 
{
    m_organization = o;
    return *this;
};

bool ContactInfo::parse(QDomNode n)
{
    m_is_null = false;
    m_etag = getAttribute(n, "gd:etag");
    m_id = "";
    QString sid = n.firstChildElement("id").text();
    int base_idx = sid.indexOf("/base/");
    if (base_idx != -1) {
        m_id = sid.right(sid.length() - base_idx - 6);
    }
    
    QString s = n.firstChildElement("updated").text().trimmed();
    m_updated = QDateTime::fromString(s, Qt::ISODate);
    m_title = n.firstChildElement("title").text().trimmed();
    m_content = n.firstChildElement("content").text().trimmed();
    m_emails = EmailInfoList::parse(n);
    m_phones = PhoneInfoList::parse(n);
    m_name = NameInfo::parse(n);
    m_organization = OrganizationInfo::parse(n);
    m_address_list = PostalAddressList::parse(n);
    m_is_null = !m_etag.isEmpty() && m_id.isEmpty();
    return !m_is_null;
}

QString ContactInfo::toString()const 
{
    QString s = "";
    s += "id=" + m_id + ";etag=" + m_etag + ";updated=" + m_updated.toString(Qt::ISODate)
        + ";title=" + m_title + "content=" + m_content + ";"
        + m_name.toString() + ";" + m_emails.toString() + ";" + m_phones.toString() + ";" + m_organization.toString() + ";" + m_address_list.toString();
    return s;
};

void ContactInfo::updateDateTime()
{
    m_updated = QDateTime::currentDateTime();
};

bool ContactInfo::operator == (const ContactInfo& o) const
{
    COMPARE_NO_CASE(m_etag);
    COMPARE_NO_CASE(m_id);
    COMPARE_NO_CASE(m_title);
    COMPARE_NO_CASE(m_content);

    if (m_updated != o.m_updated) {
        return false;
    }

    if (m_name != o.m_name) {
        return false;
    }

    if (m_organization != o.m_organization) {
        return false;
    }

    if (m_emails != o.m_emails) {
        return false;
    }

    if (m_phones != o.m_phones) {
        return false;
    }

    if (m_address_list != o.m_address_list) {
        return false;
    }

    return true;
};

bool ContactInfo::operator!=(const ContactInfo& o) const 
{
    return !(*this == o);
};

QString ContactInfo::toXmlString()const 
{
    QString rv;
    rv = "<entry xmlns:atom=\"http://www.w3.org/2005/Atom\" xmlns:gd = \"http://schemas.google.com/g/2005\"> <atom:category scheme = \"http://schemas.google.com/g/2005#kind\" term = \"http://schemas.google.com/contact/2008#contact\"/>\n";
    rv += QString("<title>%1</title>\n").arg(m_title);
    rv += QString("<content type = \"text\">%1</content>\n").arg(m_content);    
    rv += "<gd:name>\n";
    rv += QString("    <gd:givenName>%1</gd:givenName>\n").arg(m_name.givenName());
    rv += QString("    <gd:familyName>%1</gd:familyName>\n").arg(m_name.familyName());
    rv += QString("    <gd:fullName>%1</gd:fullName>\n").arg(m_name.fullName());
    rv += "</gd:name>\n";

    rv += m_organization.toXmlString();
    rv += m_address_list.toXmlString();
    rv += m_emails.toXmlString();
    rv += m_phones.toXmlString();
    rv += "</entry>";
    return rv;
};

bool ContactInfo::parseXml(const QByteArray & data) 
{
    m_is_null = true;

    QDomDocument doc;
    QString errorMsg = 0;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qWarning() << "Failed to parse contacts XML document: " << errorMsg << "line=" << errorLine << "column=" << errorColumn;
        qWarning() << "-- begin data";
        qWarning() << data;
        qWarning() << "-- end data";
        return false;
    }

    QDomNodeList entries = doc.elementsByTagName("entry");
    if (entries.size() > 0) {
        QDomNode n = entries.item(0);
        if (!parse(n)) {
            return false;
        }
    }

    m_is_null = false;

    return true;
};

/**
    ContactList
*/
bool ContactList::parseXml(const QByteArray & data)
{
    QDomDocument doc;
    QString errorMsg = 0; 
    int errorLine; 
    int errorColumn;
    if (!doc.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qWarning() << "Failed to parse contacts XML document: " << errorMsg << "line=" << errorLine << "column=" << errorColumn;
        qWarning() << "-- begin data";
        qWarning() << data;
        qWarning() << "-- end data";
        return false;
    }

    QDomNodeList entries = doc.elementsByTagName("entry");
    for (int i = 0; i < entries.size(); i++) {
        QDomNode n = entries.item(i);
        ContactInfo::ptr ci(new ContactInfo);
        if (ci->parse(n)) {
            m_contacts.push_back(ci);
        }
    }

    return true;
};

QString ContactList::toString(bool /*multiline = true*/)const 
{
    QString rv;
    for (auto& c : contacts()) {
        rv += c->toString();
        rv += "\n";
    }
    return rv;
};

ContactList& ContactList::add(ContactInfo::ptr c) 
{
    m_contacts.push_back(c);
    return *this;
};

ContactList& ContactList::add(std::unique_ptr<ContactInfo> c) 
{
    ContactInfo::ptr c2(c.release());
    return add(c2);
};

QDebug operator << (QDebug d, const googleQt::gcontact::ContactList &lst) {
    for (auto& c : lst.contacts()) {
        d << c->toString();
    }
    return d;
}

std::ostream &operator<<(std::ostream &os, const googleQt::gcontact::ContactList& lst) 
{
    int idx = 1;
    for (auto& c : lst.contacts()) {
        os << idx << ". "
           << c->id().toStdString() << " "
           << c->title().toStdString() << std::endl;
        os << c->toString().toStdString() << std::endl << std::endl;
        idx++;
    }
    return os;
};

GContactCache::GContactCache(ApiEndpoint& )
{

};

void GContactCache::attachSQLStorage(std::shared_ptr<mail_cache::GMailSQLiteStorage> ss) 
{
    m_sql_storage = ss;
};

bool GContactCache::ensureContactTables()
{
    /// entries ///
    QString sql_entries = QString("CREATE TABLE IF NOT EXISTS %1gcontact_entry(contact_id INTEGER PRIMARY KEY, acc_id INTEGER NOT NULL, entry_id TEXT, etag TEXT, title TEXT, content TEXT,"
                                  "full_name TEXT, given_name TEXT, family_name TEXT, orga_name TEXT, orga_title TEXT, updated INTEGER)").arg(m_sql_storage->m_metaPrefix);
    if (!m_sql_storage->execQuery(sql_entries))
        return false;

    QString sql_fields = QString("CREATE TABLE IF NOT EXISTS %1gcontact_field(field_id INTEGER PRIMARY KEY, contact_id INTEGER NOT NULL, field_name TEXT NOT NULL, group_idx INTEGER NOT NULL, field_value TEXT)").arg(m_sql_storage->m_metaPrefix);
    if (!m_sql_storage->execQuery(sql_fields))
        return false;


    //field_id, contact_id, field_name, group_idx
    //cid

    //if (!execQuery(QString("CREATE UNIQUE INDEX IF NOT EXISTS %1entry_accid_idx ON %1gcontact_entry(acc_id, entry_id)").arg(m_metaPrefix)))
    //    return false;

    return true;
};

bool GContactCache::storeContacts() 
{
    using CLIST = std::list<ContactInfo::ptr>;        
    CLIST new_contacts;
    for (auto& c : m_contacts.contacts()) {
        if (c->isDbIdNull()) {
            c->updateDateTime();
            new_contacts.push_back(c);
        }
    }

    if (new_contacts.size() > 0) {
        QString sql_insert;
        sql_insert = QString("INSERT INTO  %1gcontact_entry(acc_id, title, content, full_name, given_name, family_name, orga_name, orga_title, updated)"
            " VALUES(%2, ?, ?, ?, ?, ?, ?, ?, ?)")
            .arg(m_sql_storage->m_metaPrefix)
            .arg(m_sql_storage->m_accId);
        QSqlQuery* q = m_sql_storage->prepareQuery(sql_insert);
        if (!q){
            qWarning() << "Failed to prepare contact insert-SQL" << sql_insert;
            return false;
        }

        for (auto c : new_contacts) {
            q->addBindValue(c->title());
            q->addBindValue(c->content());
            q->addBindValue(c->name().fullName());
            q->addBindValue(c->name().givenName());
            q->addBindValue(c->name().familyName());
            q->addBindValue(c->organization().name());
            q->addBindValue(c->organization().title());
            qlonglong upd_time = c->updated().toMSecsSinceEpoch();
            q->addBindValue(upd_time);
            if (q->exec()) {
                int eid = q->lastInsertId().toInt();
                c->setDbID(eid);
            }
            else {
                QString error = q->lastError().text();
                qWarning() << "ERROR. Failed to insert contact to DB" << error;
                continue;
            }
        }
    }

    return true;
};

GcontactCacheRoutes::GcontactCacheRoutes(googleQt::Endpoint& endpoint, GcontactRoutes& )
{
    m_GContactsCache.reset(new GContactCache(endpoint));
};


#ifdef API_QT_AUTOTEST

std::unique_ptr<ContactInfo> ContactInfo::EXAMPLE(int param, int )
{
    QString email = QString("me%1@gmail.com").arg(param);
    QString first = QString("First-Name%1").arg(param);
    QString last = QString("Last-Name%1").arg(param);

    std::unique_ptr<ContactInfo> ci(new ContactInfo);
    NameInfo n;
    EmailInfo e1, e2;
    PhoneInfo p1, p2;
    OrganizationInfo o;
    PostalAddress a1, a2;

    n.setFamilyName(last).setGivenName(first).setFullName(first + " " + last);
    e1.setAddress(email).setDisplayName(first + " " + last).setPrimary(true).setTypeLabel("home");
    e2.setAddress(QString("2") + email).setDisplayName(first + " " + last).setPrimary(false).setTypeLabel("work");
    p1.setNumber(QString("1-111-111%1").arg(param)).setPrimary(true);
    p2.setNumber(QString("2-222-222%1").arg(param)).setPrimary(false);
    o.setName("1-organization-name").setTitle("title-in-the-organization");
    a1.setCity("Mountain View-1").setTypeLabel("work")
        .setStreet(QString("1111-%1 Amphitheatre Pkwy").arg(param))
        .setRegion("CA").setPostcode(QString("11111-%1").arg(param))
        .setCountry("United States")
        .setPrimary(true);

    a2.setCity("Mountain View-2").setTypeLabel("home")
        .setStreet(QString("2222-%1 Amphitheatre Pkwy").arg(param))
        .setRegion("NY").setPostcode(QString("22222-%1").arg(param))
        .setCountry("United States")
        .setPrimary(false);


    ci->setName(n).setTitle("Title for " + first + " " + last)
        .addEmail(e1).addEmail(e2)
        .addPhone(p1).addPhone(p2)
        .setContent(QString("My notest on new contact for '%1'").arg(first))
        .setOrganizationInfo(o)
        .addAddress(a1).addAddress(a2);

    return ci;
}

bool autoTestContactInfoXml(ContactInfo* ci) 
{
    QString xml = ci->toXmlString();
    ApiAutotest::INSTANCE() << "=== contact/XML/begin ==== ";
    ApiAutotest::INSTANCE() << xml;
    ApiAutotest::INSTANCE() << "=== XML/end ==== ";

    QByteArray data(xml.toStdString().c_str());

    QDomDocument doc;
    QString errorMsg = 0;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        ApiAutotest::INSTANCE() << "Failed to export contacts XML document: " << errorMsg << " line=" << errorLine << " column=" << errorColumn;
        return false;
    }

    ContactInfo ci2;
    if (!ci2.parseXml(data)) {
        ApiAutotest::INSTANCE() << "Contacts Xml parse error";
        return false;
    };

    bool rv = (*ci == ci2);
    if (!rv) {
        rv = (*ci == ci2);
    }
    return rv;
}

void GcontactCacheRoutes::runAutotest() 
{
    ApiAutotest::INSTANCE() << "start-gcontact-test";
    ApiAutotest::INSTANCE() << "1";
    ApiAutotest::INSTANCE() << "2";
    ApiAutotest::INSTANCE() << "3";
    ApiAutotest::INSTANCE() << "4";

    ContactList& lst = m_GContactsCache->contacts();
    for (int i = 0; i < 10; i++) {
        auto c = ContactInfo::EXAMPLE(i, 0);
        if (!autoTestContactInfoXml(c.get())) {
            ApiAutotest::INSTANCE() << "contact Xml export/identity error";;
        }
        else {
            ApiAutotest::INSTANCE() << QString("contact-xml-identity - OK / %1").arg(i+1);
            lst.add(std::move(c));
        }
    }

    m_GContactsCache->storeContacts();
};
#endif
