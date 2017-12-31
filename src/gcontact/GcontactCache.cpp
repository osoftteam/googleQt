#include <QSqlError>
#include <QDebug>
#include <ctime>
#include <QDir>
#include <ostream>
#include "GcontactCache.h"

using namespace googleQt;
using namespace gcontact;

/**
   NameInfo
*/
NameInfo NameInfo::parse(QDomNode n)
{
    NameInfo rv;        

    QDomElement elem_name = n.firstChildElement("gd:name");
    if (!elem_name.isNull()) {
        rv.m_is_null = false;
        rv.m_fullName = elem_name.firstChildElement("gd:fullName").text();
        rv.m_givenName = elem_name.firstChildElement("gd:givenName").text();
        rv.m_familyName = elem_name.firstChildElement("gd:familyName").text();
    }
    return rv;
}

NameInfo::operator QString ()const {
    return toString();
}

QString NameInfo::toString()const
{
    QString s = "";
    if (!isNull()) {
        s = QString("name=%1,%2,%3").arg(fullName()).arg(givenName()).arg(familyName());
    }
    return s;
};

/**
    OrganizationInfo
*/
OrganizationInfo OrganizationInfo::parse(QDomNode n)
{
    OrganizationInfo rv;

    QDomElement elem_organization = n.firstChildElement("gd:organization");
    if (!elem_organization.isNull()) {
        rv.m_is_null = false;
        rv.m_name = elem_organization.firstChildElement("gd:orgName").text();
        rv.m_title = elem_organization.firstChildElement("gd:orgTitle").text();
    }
    return rv;
}

OrganizationInfo::operator QString ()const {
    return toString();
}

QString OrganizationInfo::toString()const 
{
    QString s = "";
    if (!isNull()) {
        s = QString("organization=%1,%2").arg(name()).arg(title());
    }
    return s;
};

/**
    PhoneInfo
*/
PhoneInfo::operator QString ()const {
    return toString();
}

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

/**
    EmailInfo
*/
EmailInfo::operator QString ()const {
    return toString();
}

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

/**
    EmailInfoList
*/
EmailInfoList EmailInfoList::parse(QDomNode n)
{
    EmailInfoList rv;
    EmailInfo email_info;
    QDomElement first_email_elem = n.firstChildElement("gd:email");
    QDomElement email_elem = first_email_elem;
    while (!email_elem.isNull()) {
        email_info.m_is_null = false;
        QDomNamedNodeMap attr_names = email_elem.attributes();
        if (attr_names.size() > 0) {
            for (int j = 0; j < attr_names.size(); j++) {
                QDomNode n2 = attr_names.item(j);
                if (n2.nodeType() == QDomNode::AttributeNode) {
                    if (n2.nodeName().compare("address") == 0) {
                        email_info.m_address = n2.nodeValue();
                    }
                    else  if (n2.nodeName().compare("displayName") == 0) {
                        email_info.m_display_name = n2.nodeValue();
                    }
                    else if (n2.nodeName().compare("primary") == 0) {
                        QString s = n2.nodeValue();
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

        rv.m_emails.push_back(email_info);
        email_elem = email_elem.nextSiblingElement("gd:email");
    }

    return rv;
}

EmailInfoList::operator QString ()const {
    return toString();
}

QString EmailInfoList::toString()const
{
    QString s = "";
    for (auto& p : m_emails) {
        s += QString(";") + p.toString();
    }
    return s;
};

QString EmailInfoList::toXmlString()const
{
    QString s = "";
    for (auto& p : m_emails) {
        QString s_is_primary = "";
        QString displayName = "";
        if (!p.displayName().isEmpty()) {
            displayName = QString(" displayName=\"%1\"").arg(p.displayName());
        }
        if (p.isPrimary()) {
            s_is_primary = " primary=\"true\"";
        }
        s += QString("<gd:email rel = \"http://schemas.google.com/g/2005#%1\"%2 %3%4/>\n")
            .arg(p.m_type_label)
            .arg(s_is_primary)
            .arg(p.address())
            .arg(displayName);
    }
    return s;
};


/**
    PhoneInfoList
*/
PhoneInfoList PhoneInfoList::parse(QDomNode n)
{
    PhoneInfoList rv;
    PhoneInfo phone_info;
    QDomElement first_phone_elem = n.firstChildElement("gd:phoneNumber");
    QDomElement phone_elem = first_phone_elem;
    while (!phone_elem.isNull()) {
        phone_info.m_number = phone_elem.text();
        phone_info.m_is_null = false;
        QDomNamedNodeMap attr_names = phone_elem.attributes();
        if (attr_names.size() > 0) {
            for (int j = 0; j < attr_names.size(); j++) {
                QDomNode n2 = attr_names.item(j);
                if (n2.nodeType() == QDomNode::AttributeNode) {
                    if (n2.nodeName().compare("uri") == 0) {
                        phone_info.m_uri = n2.nodeValue();
                    }
                    else if (n2.nodeName().compare("primary") == 0) {
                        QString s = n2.nodeValue();
                        phone_info.m_is_primary = (s.indexOf("true") != -1);
                    }
                    else if (n2.nodeName().compare("rel") == 0) {
                        QStringList slist = n2.nodeValue().split("#");
                        if (slist.size() == 2) {
                            phone_info.m_type_label = slist[1];
                        }
                    }
                }
            }
        }

        rv.m_phones.push_back(phone_info);
        phone_elem = phone_elem.nextSiblingElement("gd:phoneNumber");
    }

    return rv;
}

PhoneInfoList::operator QString ()const {
    return toString();
}

QString PhoneInfoList::toString()const 
{
    QString s = "";
    for (auto& p : m_phones) {
        s += ";" + p.toString();
    }
    return s;
};

QString PhoneInfoList::toXmlString()const 
{
    QString s = "";
    for (auto& p : m_phones) {
        QString s_is_primary = "";
        if (p.m_is_primary) {
            s_is_primary = " primary=\"true\"";
        }
        s += QString("<gd:phoneNumber rel = \"http://schemas.google.com/g/2005#%1\"%2\n").arg(p.m_type_label).arg(s_is_primary);
        s += QString("    %1\n").arg(p.m_number);
        s += "</gd:phoneNumber>\n";
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
bool ContactInfo::parse(QDomNode n)
{
    m_is_null = false;
    m_etag = getAttribute(n, "gd:etag");
    QString sid = n.firstChildElement("id").text();
    int base_idx = sid.indexOf("/base/");
    if (base_idx == -1) {
        return false;
    }
    m_id = sid.right(sid.length() - base_idx - 6);
    QString s = n.firstChildElement("updated").text();
    m_updated = QDateTime::fromString(s, Qt::ISODate);
   // QDomElement elem_email = n.firstChildElement("gd:email");
    //m_email = getAttribute(elem_email, "address");
    m_title = n.firstChildElement("title").text();
    m_content = n.firstChildElement("content").text();
    m_emails = EmailInfoList::parse(n);
    m_phones = PhoneInfoList::parse(n);
    m_name = NameInfo::parse(n);
    m_organization = OrganizationInfo::parse(n);
    m_is_null = !m_etag.isEmpty() && m_id.isEmpty();
    return !m_is_null;
}

ContactInfo::operator QString ()const {
    return toString();
}

QString ContactInfo::toString()const 
{
    QString s = "";
    s += "id=" + m_id + ";etag=" + m_etag + ";updated=" + m_updated.toString(Qt::ISODate)
        + ";title=" + m_title + "content=" + m_content + ";"
        + (QString)m_name + ";" + (QString)m_emails + ";" + (QString)m_phones + ";" + (QString)m_organization;
    return s;
};

QString ContactInfo::toXmlString()const 
{
    QString rv;
    rv = "<atom:entry xmlns:atom=\"http://www.w3.org/2005/Atom\" xmlns:gd = \"http://schemas.google.com/g/2005\"> <atom:category scheme = \"http://schemas.google.com/g/2005#kind\" term = \"http://schemas.google.com/contact/2008#contact\"/>\n";
    rv += "<gd:name>\n";
    rv += QString("    <gd:givenName>%1</gd:givenName>\n").arg(m_name.givenName());
    rv += QString("    <gd:familyName>%1</gd:familyName>\n").arg(m_name.familyName());
    rv += QString("    <gd:fullName>%1</gd:fullName>\n").arg(m_name.fullName());
    rv += "</gd:name>\n";

    rv += QString("<atom:content type = \"text\">%1</atom:content>\n").arg(m_content);
    rv += m_emails.toXmlString();
    rv += m_phones.toXmlString();    
    rv += "</atom:entry>";
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

    return !m_is_null;
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
        ContactInfo ci;
        if (ci.parse(n)) {
            m_contacts.push_back(ci);
        }
    }

    return true;
};

QString ContactList::toString(bool /*multiline = true*/)const 
{
    QString rv;
    for (auto& c : contacts()) {
        rv += (QString)c;
        rv += "\n";
    }
    return rv;
};

QDebug operator << (QDebug d, const googleQt::gcontact::ContactList &lst) {
    for (auto& c : lst.contacts()) {
        d << (QString)c;
    }
    return d;
}

std::ostream &operator<<(std::ostream &os, const googleQt::gcontact::ContactList& lst) 
{
    int idx = 1;
    for (auto& c : lst.contacts()) {
        os << idx << ". "
           << c.id().toStdString() << " "
           << c.title().toStdString() << std::endl;
        os << c.toString().toStdString() << std::endl << std::endl;
        idx++;
    }
    return os;
};
