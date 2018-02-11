#include "GcontactParts.h"

using namespace googleQt;
using namespace gcontact;


/**
   NameInfo
*/
NameInfo::NameInfo() 
{
};

bool NameInfo::isEmpty()const 
{
    if (isNull())
        return false;
    bool rv = m_fullName.isEmpty() && m_givenName.isEmpty() && m_familyName.isEmpty();
    return rv;
}

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

void NameInfo::toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const
{
    xml_util::removeNodes(entry_node, "gd:name");
    QDomElement orga_node = xml_util::ensureNode(doc, entry_node, "gd:name");
    xml_util::updateNode(doc, orga_node, "gd:fullName", m_fullName);
    xml_util::updateNode(doc, orga_node, "gd:givenName", m_givenName);
    xml_util::updateNode(doc, orga_node, "gd:familyName", m_familyName);
};


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
OrganizationInfo::OrganizationInfo():m_type_label("other")
{
};

bool OrganizationInfo::isEmpty()const
{
    if (isNull())
        return false;
    bool rv = m_name.isEmpty() && m_title.isEmpty();
    return rv;
}


OrganizationInfo OrganizationInfo::parse(QDomNode n)
{
    OrganizationInfo rv;
    rv.m_is_null = true;
    QDomElement elem_organization = n.firstChildElement("gd:organization");
    if (!elem_organization.isNull()) {
        rv.m_is_null = false;
        rv.m_name = elem_organization.firstChildElement("gd:orgName").text().trimmed();
        rv.m_title = elem_organization.firstChildElement("gd:orgTitle").text().trimmed();


        QDomNamedNodeMap attr_names = elem_organization.attributes();
        if (attr_names.size() > 0) {
            for (int j = 0; j < attr_names.size(); j++) {
                QDomNode n2 = attr_names.item(j);
                if (n2.nodeType() == QDomNode::AttributeNode) {
                    if (n2.nodeName().compare("rel") == 0) {
                        QStringList slist = n2.nodeValue().trimmed().split("#");
                        if (slist.size() == 2) {
                            rv.m_type_label = slist[1];
                        }
                    }
                }
            }
        }
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
        s += QString("<gd:organization rel = \"http://schemas.google.com/g/2005#%1\">\n").arg(m_type_label);
        s += QString("    <gd:orgName>%1</gd:orgName>\n").arg(m_name);
        s += QString("    <gd:orgTitle>%1</gd:orgTitle>\n").arg(m_title);
        s += "</gd:organization>\n";
    }
    return s;
};

void OrganizationInfo::toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const 
{
    xml_util::removeNodes(entry_node, "gd:organization");
    QDomElement orga_node = xml_util::ensureNode(doc, entry_node, "gd:organization");
    xml_util::updateNode(doc, orga_node, "gd:orgName", m_name);
    xml_util::updateNode(doc, orga_node, "gd:orgTitle", m_title);

    orga_node.setAttribute("rel", QString("http://schemas.google.com/g/2005#%1").arg(m_type_label));
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

void EmailInfoList::toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const
{
    xml_util::removeNodes(entry_node, "gd:email");
    for (auto& p : m_parts) {
        if (!p.isNull()) {
            QDomElement phone_node = xml_util::addNode(doc, entry_node, "gd:email");
            if (p.m_is_primary) {
                phone_node.setAttribute("primary", "true");
            }
            if (!p.displayName().isEmpty()) {
                phone_node.setAttribute("displayName", p.displayName());
            }
            phone_node.setAttribute("address", p.address());
            phone_node.setAttribute("rel", QString("http://schemas.google.com/g/2005#%1").arg(p.m_type_label));
        }
    }
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

void PhoneInfoList::toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const
{
    xml_util::removeNodes(entry_node, "gd:phoneNumber");
    for (auto& p : m_parts) {
        if (!p.isNull()) {
            QDomElement phone_node = xml_util::addNode(doc, entry_node, "gd:phoneNumber");
            if (p.m_is_primary) {
                phone_node.setAttribute("primary", "true");
            }
            if (!p.uri().isEmpty()) {
                phone_node.setAttribute("uri", p.uri());
            }
            phone_node.setAttribute("rel", QString("http://schemas.google.com/g/2005#%1").arg(p.m_type_label));
            xml_util::addText(doc, phone_node, p.number());
            //QDomText tn = doc.createTextNode(QString(p.number()));
            //phone_node.appendChild(tn);
        }
    }
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

void PostalAddressList::toXmlDoc(QDomDocument& doc, QDomNode& entry_node)const
{
    xml_util::removeNodes(entry_node, "gd:structuredPostalAddress");
    for (auto& p : m_parts) {
        if (!p.isNull()) {
            QDomElement addr_node = xml_util::addNode(doc, entry_node, "gd:structuredPostalAddress");
            if (p.m_is_primary) {
                addr_node.setAttribute("primary", "true");
            }
            addr_node.setAttribute("rel", QString("http://schemas.google.com/g/2005#%1").arg(p.m_type_label));
            xml_util::updateNode(doc, addr_node, "gd:city", p.m_city);
            xml_util::updateNode(doc, addr_node, "gd:street", p.m_street);
            xml_util::updateNode(doc, addr_node, "gd:region", p.m_region);
            xml_util::updateNode(doc, addr_node, "gd:postcode", p.m_postcode);
            xml_util::updateNode(doc, addr_node, "gd:country", p.m_country);
            xml_util::updateNode(doc, addr_node, "gd:formattedAddress", p.m_formattedAddress);
        }
    }
};
