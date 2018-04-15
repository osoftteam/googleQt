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

#define CONFIG_SYNC_TIME "sync-time"

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
ContactInfo* ContactInfo::createWithId(QString contact_id) 
{
    ContactInfo* rv = new ContactInfo();
    rv->m_id = contact_id;
    return rv;
};

ContactInfo& ContactInfo::setTitle(QString val)
{
    if(m_title != val){
        m_title = val;
        markAsModified();
    }
    return *this;
};

ContactInfo& ContactInfo::setContent(QString notes) 
{
    if (m_content != notes) {
        m_content = notes;
        markAsModified();
    }

    return *this;
};

ContactInfo& ContactInfo::setName(const NameInfo& n)
{
    if (m_name != n) {
        m_name = n;
        markAsModified();
    }    
    return *this;
};

ContactInfo& ContactInfo::addEmail(const EmailInfo& e)
{
    m_emails.m_parts.push_back(e);
    markAsModified();
    return *this;
};

ContactInfo& ContactInfo::addPhone(const PhoneInfo& p) 
{
    m_phones.m_parts.push_back(p);
    markAsModified();
    return *this;
};

ContactInfo& ContactInfo::replaceEmails(const std::list<EmailInfo>& lst)
{
    m_emails.m_parts.clear();
    markAsModified();
    for (auto& p : lst) {
        m_emails.m_parts.push_back(p);
    }
    return *this;
};

ContactInfo& ContactInfo::replacePhones(const std::list<PhoneInfo>& lst)
{
    m_phones.m_parts.clear();
    markAsModified();
    
    for (auto& p : lst) {
        m_phones.m_parts.push_back(p);
    }
    return *this;
};


ContactInfo& ContactInfo::addAddress(const PostalAddress& p)
{
    m_address_list.m_parts.push_back(p);
    markAsModified();
    return *this;
};

ContactInfo& ContactInfo::replaceAddressList(const std::list<PostalAddress>& lst)
{
    m_address_list.m_parts.clear();
    markAsModified();
    for (auto& p : lst) {
        m_address_list.m_parts.push_back(p);
    }
    return *this;
};


ContactInfo& ContactInfo::setOrganizationInfo(const OrganizationInfo& o) 
{
    m_organization = o;
    markAsModified();
    return *this;
};

QString ContactInfo::toString()const 
{
    QString s = "";
    s += "id=" + m_id + ";etag=" + m_etag + ";updated=" + m_updated.toString(Qt::ISODate)
        + ";title=" + m_title + "content=" + m_content + ";"
        + m_name.toString() + ";" + m_emails.toString() + ";" + m_phones.toString() + ";" + m_organization.toString() + ";" + m_address_list.toString();
    return s;
};


bool ContactInfo::operator == (const ContactInfo& o) const
{
    ///do not check on m_original_xml_string - it's for cache storage

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

    if (m_photo != o.m_photo) {
        return false;
    }

    return true;
};

bool ContactInfo::operator!=(const ContactInfo& o) const 
{
    return !(*this == o);
};

QString ContactInfo::toXmlBegin()const 
{
    QString rv;
    if (m_etag.isEmpty()) {
        rv = QString("<entry>\n");
    }
    else {
        rv = QString("<entry gd:etag=\"%1\">\n")
            .arg(m_etag);
    }
    return rv;
};

QString ContactInfo::toXml(QString userEmail)const 
{
    QString rv = toXmlBegin();

    if(!m_id.isEmpty())rv += QString("<id>http://www.google.com/m8/feeds/contacts/%1/base/%2</id>\n").arg(userEmail).arg(m_id);
    if(!m_title.isEmpty())rv += QString("<title>%1</title>\n").arg(m_title.toHtmlEscaped());
    if (!m_title.isEmpty())rv += QString("<content type = \"text\">%1</content>\n").arg(m_content.toHtmlEscaped());
    if (!m_name.isEmpty()) {
        rv += "<gd:name>\n";
        rv += QString("    <gd:givenName>%1</gd:givenName>\n").arg(m_name.givenName().toHtmlEscaped());
        rv += QString("    <gd:familyName>%1</gd:familyName>\n").arg(m_name.familyName().toHtmlEscaped());
        rv += QString("    <gd:fullName>%1</gd:fullName>\n").arg(m_name.fullName().toHtmlEscaped());
        rv += "</gd:name>\n";
    }
    if(!m_organization.isEmpty())rv += m_organization.toXmlString();
    rv += m_address_list.toXmlString();
    rv += m_emails.toXmlString();
    rv += m_phones.toXmlString();
    rv += "</entry>";
    return rv;
};


void ContactInfo::mergeEntryNode(QDomDocument& doc, QDomNode& entry_node)const
{
    QDomElement entry_elem = entry_node.toElement();
    if (entry_elem.isNull()) {
        qWarning() << "Invalid entry element";
    }
    else {
        entry_elem.setAttribute("gd:etag", m_etag);
    }

    xml_util::updateNode(doc, entry_node, "title", m_title);
    xml_util::updateNode(doc, entry_node, "content", m_content);
    m_organization.toXmlDoc(doc, entry_node);
    m_name.toXmlDoc(doc, entry_node);
    m_emails.toXmlDoc(doc, entry_node);
    m_phones.toXmlDoc(doc, entry_node);
    m_address_list.toXmlDoc(doc, entry_node);
};


bool ContactInfo::parseEntryNode(QDomNode n)
{
    m_original_xml_string = "";
    QTextStream ss(&m_original_xml_string);
    n.save(ss, 4);
    ss.flush();

    if (!verifyXml(m_original_xml_string)) {
        qWarning() << "Failed to validate contact xml node";
        return false;
    }

    if (m_original_xml_string.isEmpty()) {
        qWarning() << "Failed to parse contact xml node";
        return false;
    }

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
    m_photo = PhotoInfo::parse(n);
    m_is_null = !m_etag.isEmpty() && m_id.isEmpty();
    return !m_is_null;
}

bool ContactInfo::setFromDbRecord(QSqlQuery* q)
{
    QString xml = q->value(1).toString();
    if (!parseXml(xml)) {
        qWarning() << "failed to parse contact entry db record size=" << xml.size() << "dbid=" << q->value(3).toInt();
        qWarning() << "content=" << q->value(5).toString();
        qWarning() << "original-xml=" << q->value(6).toString();
        qWarning() << "-------------";
        return false;
    }
    bool ok = false;
    m_status = ContactXmlPersistant::validatedStatus(q->value(0).toInt(), &ok);
    if (!ok) {
        qWarning() << "Invalid contact db record status" << q->value(0).toInt();
    }

    setRegisterModifications(false); 
    
    m_updated = QDateTime::fromMSecsSinceEpoch(q->value(2).toLongLong());
    m_db_id = q->value(3).toInt();
    m_title = q->value(4).toString();
    m_content = q->value(5).toString();

    m_original_xml_string = q->value(6).toString();

    NameInfo n;
    n.setFullName(q->value(7).toString())
        .setGivenName(q->value(8).toString())
        .setFamilyName(q->value(9).toString());


    OrganizationInfo o;
    o.setName(q->value(10).toString())
        .setTitle(q->value(11).toString())
        .setTypeLabel(q->value(12).toString());

    setName(n);
    setOrganizationInfo(o);

    QString photo_href = q->value(13).toString();
    QString photo_etag = q->value(14).toString();
    int st_tmp = q->value(15).toInt();
    if(st_tmp < 0 || st_tmp > 1){
        st_tmp = 0;
        qWarning() << "Invalid DB value for photo status" << st_tmp;
    }
    PhotoInfo::EStatus st = static_cast<PhotoInfo::EStatus>(st_tmp);
    
    m_photo.setupFromLocalDb(photo_href, photo_etag, st);

    setRegisterModifications(true); 
    return true;

};

void ContactInfo::assignContent(const ContactInfo& src)
{
    ContactXmlPersistant::assignContent(src);
    m_name = src.m_name;
    m_organization = src.m_organization;
    m_emails = src.m_emails;
    m_phones = src.m_phones;
    m_address_list = src.m_address_list;   
    m_photo = src.m_photo;
};

std::unique_ptr<BatchRequestContactInfo> ContactInfo::buildBatchRequest(googleQt::EBatchId batch_id)
{
    std::unique_ptr<BatchRequestContactInfo> rv(new BatchRequestContactInfo);
    rv->assignContent(*this);
    rv->setBatchid(batch_id);
    return rv;
};

/**
    GroupInfo
*/
GroupInfo* GroupInfo::createWithId(QString group_id) 
{
    GroupInfo* rv = new GroupInfo();
    rv->m_id = group_id;
    return rv;
};

bool GroupInfo::parseEntryNode(QDomNode n)
{
    m_original_xml_string = "";
    QTextStream ss(&m_original_xml_string);
    n.save(ss, 4);
    ss.flush();

    if (m_original_xml_string.isEmpty()) {
        qWarning() << "Failed to parse contact xml node";
        return false;
    }
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

    m_is_null = !m_etag.isEmpty() && m_id.isEmpty();
    return !m_is_null;
}

void GroupInfo::mergeEntryNode(QDomDocument& doc, QDomNode& entry_node)const
{
    QDomElement entry_elem = entry_node.toElement();
    if (entry_elem.isNull()) {
        qWarning() << "Invalid entry element";
    }
    else {
        entry_elem.setAttribute("gd:etag", m_etag);
    }

    xml_util::updateNode(doc, entry_node, "title", m_title);
    xml_util::updateNode(doc, entry_node, "content", m_content);
};

QString GroupInfo::toXmlBegin()const
{
    QString rv;
    if (m_etag.isEmpty()) {
        rv = QString("<entry>\n");
    }
    else {
        rv = QString("<entry gd:etag=\"%1\">\n")
            .arg(m_etag);
    }
    return rv;
};

QString GroupInfo::toXml(QString userEmail)const 
{
    QString rv = toXmlBegin();        

    if (!m_id.isEmpty())rv += QString("<id>http://www.google.com/m8/feeds/groups/%1/base/%2</id>\n").arg(userEmail).arg(m_id);
    if(!m_title.isEmpty())rv += QString("<title>%1</title>\n").arg(m_title);
    if (!m_content.isEmpty())rv += QString("<content type = \"text\">%1</content>\n").arg(m_content);
    rv += "</entry>\n";
    return rv;
};

void GroupInfo::assignContent(const GroupInfo& src)
{
    ContactXmlPersistant::assignContent(src);
}

QString GroupInfo::toString()const
{
    QString s = "";
    s += "id=" + m_id + ";etag=" + m_etag + ";updated=" + m_updated.toString(Qt::ISODate)
        + ";title=" + m_title + "content=" + m_content + ";";
    return s;
};


GroupInfo& GroupInfo::setTitle(QString val)
{
    m_title = val;
    return *this;
};

GroupInfo& GroupInfo::setContent(QString notes)
{
    m_content = notes;
    return *this;
};

bool GroupInfo::operator == (const GroupInfo& o) const
{
    ///do not check on m_original_xml_string - it's for cache storage

    COMPARE_NO_CASE(m_etag);
    COMPARE_NO_CASE(m_id);
    COMPARE_NO_CASE(m_title);
    COMPARE_NO_CASE(m_content);

    return true;
}

bool GroupInfo::operator!=(const GroupInfo& o) const
{
    return !(*this == o);
};

bool GroupInfo::setFromDbRecord(QSqlQuery* q) 
{
    QString xml = q->value(1).toString();
    if (!parseXml(xml)) {
        qWarning() << "failed to parse contact group db record" << xml.size();
        return false;
    }
    bool ok = false;
    m_status = ContactXmlPersistant::validatedStatus(q->value(0).toInt(), &ok);
    if (!ok) {
        qWarning() << "Invalid contact db group record status" << q->value(0).toInt();
    }

    setRegisterModifications(false); 
    
    m_updated = QDateTime::fromMSecsSinceEpoch(q->value(2).toLongLong());
    m_db_id = q->value(3).toInt();
    m_title = q->value(4).toString();
    m_content = q->value(5).toString();
    setRegisterModifications(true);
    return true;
};

std::unique_ptr<BatchRequestGroupInfo> GroupInfo::buildBatchRequest(googleQt::EBatchId batch_id)
{
    std::unique_ptr<BatchRequestGroupInfo> rv(new BatchRequestGroupInfo);
    rv->assignContent(*this);
    rv->setBatchid(batch_id);
    return rv;
};


/**
BatchRequestContactInfo
*/
std::unique_ptr<BatchRequestContactInfo> BatchRequestContactInfo::buildRequest(QString contact_id, googleQt::EBatchId batch_id)
{
    std::unique_ptr<BatchRequestContactInfo> rv(new BatchRequestContactInfo);
    rv->m_id = contact_id;
    rv->m_batch_id = batch_id;
    return rv;
};

QString BatchRequestContactInfo::toXmlBegin()const
{
    QString rv;
    if (m_batch_id != googleQt::EBatchId::none) {
        rv = BatchRequest::toBatchXmlEntryBegin();
    }
    else {
        rv = ContactInfo::toXmlBegin();
    }
    return rv;
};

/**
    BatchRequestGroupInfo
*/
std::unique_ptr<BatchRequestGroupInfo> BatchRequestGroupInfo::buildRequest(QString contact_id, googleQt::EBatchId batch_id)
{
    std::unique_ptr<BatchRequestGroupInfo> rv(new BatchRequestGroupInfo);
    rv->m_id = contact_id;
    rv->m_batch_id = batch_id;
    return rv;
};

QString BatchRequestGroupInfo::toXmlBegin()const
{
    QString rv;
    if (m_batch_id != googleQt::EBatchId::none) {
        rv = BatchRequest::toBatchXmlEntryBegin();
    }
    else {
        rv = GroupInfo::toXmlBegin();
    }

    return rv;
};

/**
    GContactCache
*/
GContactCache::GContactCache(ApiEndpoint& a):m_endpoint(a)
{

};

void GContactCache::attachSQLStorage(std::shared_ptr<mail_cache::GMailSQLiteStorage> ss) 
{
    m_sql_storage = ss;
};

bool GContactCache::ensureContactTables()
{
    /// contacts ///
    QString sql_entries = QString("CREATE TABLE IF NOT EXISTS %1gcontact_entry(contact_db_id INTEGER PRIMARY KEY, acc_id INTEGER NOT NULL, entry_id TEXT, etag TEXT, title TEXT, content TEXT, "
                                  "full_name TEXT, given_name TEXT, family_name TEXT, orga_name TEXT, orga_title TEXT, orga_label TEXT, xml_original TEXT, xml_current TEXT, updated INTEGER, status INTEGER, "
                                  "photo_href TEXT, photo_etag TEXT, photo_status INTEGER)").arg(m_sql_storage->m_metaPrefix);
    if (!m_sql_storage->execQuery(sql_entries))
        return false;

    QString sql_groups = QString("CREATE TABLE IF NOT EXISTS %1gcontact_group(group_db_id INTEGER PRIMARY KEY, acc_id INTEGER NOT NULL, entry_id TEXT, etag TEXT, title TEXT, content TEXT,"
        "xml_original TEXT, updated INTEGER, status INTEGER)").arg(m_sql_storage->m_metaPrefix);
    if (!m_sql_storage->execQuery(sql_groups))
        return false;

    QString sql_config = QString("CREATE TABLE IF NOT EXISTS %1gcontact_config(acc_id INTEGER NOT NULL, config_name TEXT, config_value TEXT)").arg(m_sql_storage->m_metaPrefix);
    if (!m_sql_storage->execQuery(sql_config))
        return false;
    

    return true;
};

bool GContactCache::storeContactList(std::vector<std::shared_ptr<ContactInfo>>& contact_list)
{    
    using CLIST = std::list<ContactInfo::ptr>;
    CLIST cloud_created_contacts;
    CLIST updated_contacts;
    CLIST limbo_id_contacts;
    for (auto& c : contact_list) {
        if (c->isDbIdNull()) {
            cloud_created_contacts.push_back(c);
        }
        else {
            if(c->isDirty()){
                if (c->isIdLimbo()) {
                    c->markAsNormalCopy();
                    limbo_id_contacts.push_back(c);
                }
                else {
                    updated_contacts.push_back(c);
                }
                c->setDirty(false);
            }
        }
    }

    if (cloud_created_contacts.size() > 0) {
        qDebug() << "sql-contacts-store-cloud-created" << cloud_created_contacts.size();
        QString sql_insert;
        sql_insert = QString("INSERT INTO  %1gcontact_entry(acc_id, title, content, full_name, given_name, family_name, orga_name, orga_title, orga_label,"
                            "xml_original, xml_current, updated, status, entry_id, etag, photo_href, photo_etag, photo_status)"
            " VALUES(%2, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")
            .arg(m_sql_storage->m_metaPrefix)
            .arg(m_sql_storage->m_accId);
        QSqlQuery* q = m_sql_storage->prepareQuery(sql_insert);
        if (!q) {
            qWarning() << "Failed to prepare contact insert-SQL" << sql_insert;
            return false;
        }

        for (auto c : cloud_created_contacts) {
            q->addBindValue(c->title());
            q->addBindValue(c->content());
            q->addBindValue(c->name().fullName());
            q->addBindValue(c->name().givenName());
            q->addBindValue(c->name().familyName());
            q->addBindValue(c->organization().name());
            q->addBindValue(c->organization().title());
            q->addBindValue(c->organization().typeLabel());
            q->addBindValue(c->originalXml());
            q->addBindValue(c->toXml(m_endpoint.apiClient()->userId()));
            qlonglong upd_time = c->updated().toMSecsSinceEpoch();
            q->addBindValue(upd_time);
            q->addBindValue(c->status());
            q->addBindValue(c->id());
            q->addBindValue(c->etag());

            q->addBindValue(c->photo().href());
            q->addBindValue(c->photo().etag());
            q->addBindValue(c->photo().status());
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
    
    if (updated_contacts.size() > 0) {
        qDebug() << "sql-contacts-store-updated" << updated_contacts.size();
        QString sql_update;
        sql_update = QString("UPDATE  %1gcontact_entry SET title=?, content=?, full_name=?, given_name=?, family_name=?, orga_name=?, orga_title=?, "
            "orga_label=?, xml_original=?, xml_current=?, updated=?, status=?, photo_href=?, photo_etag=?, photo_status=? WHERE contact_db_id=? AND acc_id = %2")
            .arg(m_sql_storage->m_metaPrefix)
            .arg(m_sql_storage->m_accId);
        QSqlQuery* q = m_sql_storage->prepareQuery(sql_update);
        if (!q) {
            qWarning() << "Failed to prepare contact update-SQL" << sql_update;
            return false;
        }

        for (auto c : updated_contacts) {
            q->addBindValue(c->title());
            q->addBindValue(c->content());
            q->addBindValue(c->name().fullName());
            q->addBindValue(c->name().givenName());
            q->addBindValue(c->name().familyName());
            q->addBindValue(c->organization().name());
            q->addBindValue(c->organization().title());
            q->addBindValue(c->organization().typeLabel());
            q->addBindValue(c->originalXml());
            q->addBindValue(c->toXml(m_endpoint.apiClient()->userId()));
            qlonglong upd_time = c->updated().toMSecsSinceEpoch();
            q->addBindValue(upd_time);
            q->addBindValue(c->status());
            q->addBindValue(c->photo().href());
            q->addBindValue(c->photo().etag());
            q->addBindValue(c->photo().status());
            q->addBindValue(c->dbID());
            if (!q->exec()) {
                QString error = q->lastError().text();
                qWarning() << "ERROR. Failed to update contact entry" << error;
                continue;
            }
        }
    }

    if (limbo_id_contacts.size() > 0) {
        qDebug() << "sql-contacts-store-limbo" << limbo_id_contacts.size();
        QString sql_update;
        sql_update = QString("UPDATE  %1gcontact_entry SET title=?, content=?, full_name=?, given_name=?, family_name=?, orga_name=?, orga_title=?, "
            "orga_label=?, xml_original=?, xml_current=?, updated=?, status=?, entry_id=?, etag=?, photo_href=?, photo_etag=?, photo_status=? WHERE contact_db_id=? AND acc_id = %2")
            .arg(m_sql_storage->m_metaPrefix)
            .arg(m_sql_storage->m_accId);
        QSqlQuery* q = m_sql_storage->prepareQuery(sql_update);
        if (!q) {
            qWarning() << "Failed to prepare contact update-SQL" << sql_update;
            return false;
        }

        for (auto c : limbo_id_contacts) {
            q->addBindValue(c->title());
            q->addBindValue(c->content());
            q->addBindValue(c->name().fullName());
            q->addBindValue(c->name().givenName());
            q->addBindValue(c->name().familyName());
            q->addBindValue(c->organization().name());
            q->addBindValue(c->organization().title());
            q->addBindValue(c->organization().typeLabel());
            q->addBindValue(c->originalXml());
            q->addBindValue(c->toXml(m_endpoint.apiClient()->userId()));
            qlonglong upd_time = c->updated().toMSecsSinceEpoch();
            q->addBindValue(upd_time);
            q->addBindValue(c->status());
            q->addBindValue(c->id());
            q->addBindValue(c->etag());
            q->addBindValue(c->photo().href());
            q->addBindValue(c->photo().etag());
            q->addBindValue(c->photo().status());
            q->addBindValue(c->dbID());
            if (!q->exec()) {
                QString error = q->lastError().text();
                qWarning() << "ERROR. Failed to update contact entry" << error;
                continue;
            }
        }
    }

    return true;
};

bool GContactCache::storeGroupList(std::vector<std::shared_ptr<GroupInfo>>& group_list)
{
    using CLIST = std::list<GroupInfo::ptr>;
    CLIST new_groups;
    CLIST updated_groups;
    for (auto& g : group_list) {
        if (g->isDbIdNull()) {
            new_groups.push_back(g);
        }
        else {
            updated_groups.push_back(g);
        }
    }

    if (new_groups.size() > 0) {
        QString sql_insert;
        sql_insert = QString("INSERT INTO  %1gcontact_group(acc_id, title, content, xml_original, updated, status, entry_id, etag)"
            " VALUES(%2, ?, ?, ?, ?, ?, ?, ?)")
            .arg(m_sql_storage->m_metaPrefix)
            .arg(m_sql_storage->m_accId);

        QSqlQuery* q = m_sql_storage->prepareQuery(sql_insert);
        if (!q) {
            qWarning() << "Failed to prepare contact group insert-SQL" << sql_insert;
            return false;
        }

        for (auto c : new_groups) {
            q->addBindValue(c->title());
            q->addBindValue(c->content());
            q->addBindValue(c->originalXml());
            qlonglong upd_time = c->updated().toMSecsSinceEpoch();
            q->addBindValue(upd_time);
            q->addBindValue(c->status());
            q->addBindValue(c->id());
            q->addBindValue(c->etag());
            if (q->exec()) {
                int eid = q->lastInsertId().toInt();
                c->setDbID(eid);
            }
            else {
                QString error = q->lastError().text();
                qWarning() << "ERROR. Failed to insert contact group to DB" << error;
                continue;
            }
        }//for
    }
    else if (updated_groups.size() > 0) {
        QString sql_update;
        sql_update = QString("UPDATE  %1gcontact_group SET title=?, content=?, xml_original=?, updated=?, status=? WHERE group_db_id=? AND acc_id = %2")
            .arg(m_sql_storage->m_metaPrefix)
            .arg(m_sql_storage->m_accId);

        QSqlQuery* q = m_sql_storage->prepareQuery(sql_update);
        if (!q) {
            qWarning() << "Failed to prepare contact group update-SQL" << sql_update;
            return false;
        }

        for (auto c : updated_groups) {
            q->addBindValue(c->title());
            q->addBindValue(c->content());
            q->addBindValue(c->originalXml());
            qlonglong upd_time = c->updated().toMSecsSinceEpoch();
            q->addBindValue(upd_time);
            q->addBindValue(c->status());
            q->addBindValue(c->dbID());
            if (!q->exec()) {
                QString error = q->lastError().text();
                qWarning() << "ERROR. Failed to update contact group" << error;
                continue;
            }
        }//for
    }

    return true;
};

bool GContactCache::storeContactEntries() 
{
    if(!storeContactList(m_contacts.items())){
        return false;
    }
    
    if(!storeContactList(m_contacts.retired())){
        return false;
    }
    
    return true;
};

bool GContactCache::storeContactGroups()
{
    if(!storeGroupList(m_groups.items())){
        return false;
    }
    
    if(!storeGroupList(m_groups.retired())){
        return false;
    }
    
    return true;
};

bool GContactCache::storeConfig()
{
    if (m_sync_time.isValid()) {

        QString sql_del_old;
        sql_del_old = QString("DELETE FROM  %1gcontact_config WHERE acc_id=%2 AND config_name='%3'")
            .arg(m_sql_storage->m_metaPrefix)
            .arg(m_sql_storage->m_accId)
            .arg(CONFIG_SYNC_TIME);

        if (!m_sql_storage->execQuery(sql_del_old)) {
            qWarning() << "Failed to delete old contact config del-SQL" << sql_del_old;
            return false;
        };


        QString sql_insert;
        sql_insert = QString("INSERT INTO %1gcontact_config(acc_id, config_name, config_value)"
            " VALUES(%2, ?, ?)")
            .arg(m_sql_storage->m_metaPrefix)
            .arg(m_sql_storage->m_accId);

        QSqlQuery* q = m_sql_storage->prepareQuery(sql_insert);
        if (!q) {
            qWarning() << "Failed to prepare contact config insert-SQL" << sql_insert;
            return false;
        }
        q->addBindValue(CONFIG_SYNC_TIME);
        q->addBindValue(m_sync_time.toString());

        if (!q->exec()) {
            QString error = q->lastError().text();
            qWarning() << "ERROR. Failed to insert contact config to DB" << error;
            return false;
        }
    }

    return true;
};


bool GContactCache::storeContactsToDb() 
{
    if (!storeContactEntries()) {
        return false;
    }

    if (!storeContactGroups()) {
        return false;
    }

    if (!storeConfig()) {
        return false;
    }
    
    return true;
};

bool GContactCache::loadContactsFromDb() 
{
    if (!loadContactEntriesFromDb()) {
        return false;
    }

    if (!loadContactGroupsFromDb()) {
        return false;
    }

    if (!loadContactConfigFromDb()) {
        return false;
    }
    
    return true;
};

bool GContactCache::clearDbCache() 
{
    if (!m_sql_storage) {
        qWarning() << "Sql cache is not setup";
        return false;
    }

    m_contacts.clear();
    m_groups.clear();

    QDateTime invalid_time;
    m_sync_time.swap(invalid_time);

    QString sql = QString("DELETE FROM %1gcontact_group WHERE acc_id=%2")
        .arg(m_sql_storage->m_metaPrefix)
        .arg(m_sql_storage->m_accId);

    if (!m_sql_storage->execQuery(sql)) {
        return false;
    };

    sql = QString("DELETE FROM %1gcontact_entry WHERE acc_id=%2")
        .arg(m_sql_storage->m_metaPrefix)
        .arg(m_sql_storage->m_accId);

    if (!m_sql_storage->execQuery(sql)) {
        return false;
    };

    return true;
};

bool GContactCache::mergeServerModifications(GroupList& server_glist, ContactList& server_clist)
{
    m_contacts.mergeList(server_clist);
    m_groups.mergeList(server_glist);

    m_sync_time = QDateTime::currentDateTime();

    bool rv = storeContactsToDb();
    return rv;
};

bool GContactCache::loadContactGroupsFromDb()
{
    m_groups.clear();

    QString sql = QString("SELECT status, xml_original, updated, group_db_id, title, content FROM %1gcontact_group WHERE acc_id=%2 AND status IN(1,2,3) ORDER BY updated DESC")
        .arg(m_sql_storage->m_metaPrefix)
        .arg(m_sql_storage->m_accId);
    QSqlQuery* q = m_sql_storage->selectQuery(sql);
    if (!q)
        return false;

    while (q->next())
    {
        std::shared_ptr<GroupInfo> g(new GroupInfo);
        if (g->setFromDbRecord(q)) {
            m_groups.add(g);
        };
    }

    return true;
};

bool GContactCache::loadContactEntriesFromDb()
{
    m_contacts.clear();

    QString sql = QString("SELECT status, xml_current, updated, contact_db_id, title, content, xml_original, full_name, given_name, family_name,"
        "orga_name, orga_title, orga_label, photo_href, photo_etag, photo_status FROM %1gcontact_entry WHERE acc_id=%2 AND status IN(1,2,3) ORDER BY updated DESC")
        .arg(m_sql_storage->m_metaPrefix)
        .arg(m_sql_storage->m_accId);
    QSqlQuery* q = m_sql_storage->selectQuery(sql);
    if (!q)
        return false;

    while (q->next())
    {
        std::shared_ptr<ContactInfo> c(new ContactInfo);
        if (c->setFromDbRecord(q)) {
            m_contacts.add(c);
        };
    }

    return true;
};

bool GContactCache::loadContactConfigFromDb() 
{
    m_configs.clear();
    
    QDateTime invalid_time;
    m_sync_time.swap(invalid_time);
    
    QString sql = QString("SELECT MAX(updated) FROM %1gcontact_group WHERE acc_id=%2 AND status IN(1,2,3)")
        .arg(m_sql_storage->m_metaPrefix)
        .arg(m_sql_storage->m_accId);
    QSqlQuery* q = m_sql_storage->selectQuery(sql);
    if (!q){
        qWarning() << "failed to load group MAX(updated)";
        return false;
    }
    
    if(q->next() && q->value(0).toLongLong() > 0){
        m_sync_time = QDateTime::fromMSecsSinceEpoch(q->value(0).toLongLong());
    }

    
    sql = QString("SELECT MAX(updated) FROM %1gcontact_entry WHERE acc_id=%2 AND status IN(1,2,3)")
        .arg(m_sql_storage->m_metaPrefix)
        .arg(m_sql_storage->m_accId);
    q = m_sql_storage->selectQuery(sql);
    if (!q){
        qWarning() << "failed to load contacts MAX(updated)";
        return false;
    }
    if(q->next() && q->value(0).toLongLong() > 0){
        QDateTime dt = QDateTime::fromMSecsSinceEpoch(q->value(0).toLongLong());
        if(dt > m_sync_time || !m_sync_time.isValid()){
            m_sync_time = dt;
        }
    }


    sql = QString("SELECT config_name, config_value FROM %1gcontact_config WHERE acc_id=%2")
        .arg(m_sql_storage->m_metaPrefix)
        .arg(m_sql_storage->m_accId);
    q = m_sql_storage->selectQuery(sql);
    if (!q)
        return false;

    while (q->next())
    {
        QString name = q->value(0).toString();
        QString value = q->value(1).toString();
        m_configs[name] = value;

        if (name.compare(CONFIG_SYNC_TIME, Qt::CaseInsensitive) == 0) {
            QDateTime dt = QDateTime::fromString(value);
            if(dt > m_sync_time || !m_sync_time.isValid())
            m_sync_time = dt;
        }
    }

    
    return true;
};


QString GContactCache::getPhotoMediaPath(ContactInfo::ptr c)const
{
    if(!c){
        return "";
    }
    
    //    QString rv;
    //    const PhotoInfo& p = c->photo();
    //    if(p.status() == PhotoInfo::resolved && !p.etag().isEmpty())
        {
            QString cache_dir = m_sql_storage->contactCacheDir();
            cache_dir += "/contact-photos/";
            cache_dir += m_endpoint.apiClient()->userId();
            cache_dir += "/";
            QString img_file = cache_dir + QString("%1.jpg").arg(c->id());
            //qDebug() << "ykh-img-file" << img_file;
            return img_file;
            //        cache_dir += QString("/%1.jpg").arg(c->id());
        }
    
    return "";
};

GoogleVoidTask* GcontactCacheRoutes::getContactCachePhoto_Async(ContactInfo::ptr c){
    QString filePath = m_GContactsCache->getPhotoMediaPath(c);
    QFileInfo fi(filePath);
    if(fi.exists()){
        GoogleVoidTask* rv = m_endpoint.produceVoidTask();
        rv->completed_callback();
        return rv;
    }    

    QDir d;
    
    QString path = fi.path();
    if(!d.exists(path)){
        if(!d.mkpath(path)){
            GoogleVoidTask* rv = m_endpoint.produceVoidTask();
            QString errStr = QString("Failed to create directory '%1'").arg(path);
            qWarning() << "Failed to create directory" << path;
            std::unique_ptr<GoogleException> ex(new GoogleException(errStr.toStdString()));
            rv->failed_callback(std::move(ex));
            return rv;
        };
    }
    
    QFile out;
    out.setFileName(filePath);
    
    DownloadPhotoArg arg(c->id());
    return m_c_routes.getContacts()->getContactPhoto_Async(arg, &out);
};

GcontactCacheRoutes::GcontactCacheRoutes(googleQt::Endpoint& endpoint, GcontactRoutes& r):m_endpoint(endpoint), m_c_routes(r)
{
    m_GContactsCache.reset(new GContactCache(endpoint));
};

GcontactCacheQueryTask* GcontactCacheRoutes::synchronizeContacts_Async()
{
    GcontactCacheQueryTask* rv = new GcontactCacheQueryTask(m_endpoint, m_GContactsCache);

    if (!m_GContactsCache->m_sql_storage) {
        std::unique_ptr<GoogleException> ex(new GoogleException("Local cache DB is not setup. Call setupCache first"));
        rv->failed_callback(std::move(ex));
        return rv;
    }


    QDateTime dtUpdatedMin = m_GContactsCache->lastSyncTime();
    
    if (!dtUpdatedMin.isValid()) {
        ///first time sync, purge local cache just in case..
        if (!m_GContactsCache->clearDbCache()) {
            std::unique_ptr<GoogleException> ex(new GoogleException("Failed to clear local DB cache."));
            rv->failed_callback(std::move(ex));
            return rv;
        }
    }
    else{
        /// 1 sec - seems resonable for time diff on updated items
        /// we take all contacts from server newer than last sync + 1 sec
        dtUpdatedMin = dtUpdatedMin.addSecs(1).toUTC();
    }
    
    reloadCache_Async(rv, dtUpdatedMin);
    return rv;
};

void GcontactCacheRoutes::reloadCache_Async(GcontactCacheQueryTask* rv, QDateTime dtUpdatedMin)
{
    ContactListArg entries_arg;
    entries_arg.setOrderby("lastmodified");
    entries_arg.setSortorder("descending");
    entries_arg.setMaxResults(200);

    if (dtUpdatedMin.isValid()) {        
        entries_arg.setUpdatedMin(dtUpdatedMin);
    }

    ContactGroupListArg groups_arg;
    groups_arg.setMaxResults(200);

    auto entries_task = m_endpoint.client()->gcontact()->getContacts()->list_Async(entries_arg);
    auto groups_task = m_endpoint.client()->gcontact()->getContactGroup()->list_Async(groups_arg);

    TaskAggregator* agg = new TaskAggregator(m_endpoint);
    agg->add(entries_task);
    agg->add(groups_task);
    agg->then([=]() 
    {
        rv->m_loaded_contacts = entries_task->detachResult();
        rv->m_loaded_groups = groups_task->detachResult();
        
        if (m_GContactsCache->mergeServerModifications(*(rv->m_loaded_groups.get()), *(rv->m_loaded_contacts.get()))) {
            //at this point we should have loaded server changes
            //now we will try to apply our changes - modifications and deleted records

            applyLocalCacheModifications_Async(rv);
        }
        else {
            std::unique_ptr<GoogleException> ex(new GoogleException("Failed to merge/store DB cache."));
            rv->failed_callback(std::move(ex));
        }
    },
        [=](std::unique_ptr<GoogleException> ex) 
    {
        rv->failed_callback(std::move(ex));
    });
};

void GcontactCacheRoutes::applyLocalCacheModifications_Async(GcontactCacheQueryTask* rv) 
{
    BatchRequesContactList bc = m_GContactsCache->contacts().buildBatchRequestList();
    BatchRequesGroupList bg = m_GContactsCache->groups().buildBatchRequestList();
        
    BatchContactArg arg1(bc);
    BatchGroupArg arg2(bg);

    auto entries_btask = m_endpoint.client()->gcontact()->getContacts()->batch_Async(arg1);
    auto groups_btask = m_endpoint.client()->gcontact()->getContactGroup()->batch_Async(arg2);
    TaskAggregator* agg = new TaskAggregator(m_endpoint);
    agg->add(entries_btask);
    agg->add(groups_btask);
    agg->then(
        [=]() 
        {
            rv->m_updated_contacts = entries_btask->detachResult();
            rv->m_updated_groups = groups_btask->detachResult();

            auto& arr = rv->m_updated_contacts->items();
            for (auto& b : arr) {
                if(b->succeded()){
                    auto c = m_GContactsCache->contacts().findById(b->id());
                    if(c){
                        switch(b->batchResultId())
                            {
                            case EBatchId::delete_operation:
                                {
                                    m_GContactsCache->contacts().retire(c);
                                }break;
                            case EBatchId::update:
                                {
                                    c->markAsNormalCopy();
                                }break;
                            default:
                                {
                                    qWarning() << "Unexpected batch operation"
                                               << b->id()
                                               << b->batchResultOperationType()
                                               << b->batchResultStatusReason();                        
                                }break;
                            }
                    }
                    else{
                        if(b->batchResultId() == EBatchId::create){
                            auto c = m_GContactsCache->contacts().findNewCreatedContact(b);
                            if(c){
                                c->assignContent(*(b.get()));
                                c->markAsIdLimbo();
                            }
                            else{
                                qWarning() << "Cache/create obj locate on batch operation failed"
                                           << b->id()
                                           << b->batchResultOperationType()
                                           << b->batchResultStatusReason();                                
                            }
                        }
                        else{
                            qWarning() << "Cache obj locate on batch operation failed"
                                       << b->id()
                                       << b->batchResultOperationType()
                                       << b->batchResultStatusReason();
                        }
                    }
                }
                else{
                    auto c = m_GContactsCache->contacts().findById(b->id());
                    
                    if(c && b->batchResultStatusCode() == 404 && b->batchResultId() == EBatchId::delete_operation){
                        m_GContactsCache->contacts().retire(c);
                    }
                    else{
                        qWarning() << "Batch operation failed"
                                   << b->id()
                                   << b->batchResultOperationType()
                                   << b->batchResultStatusReason();
                    }
                }
            }
            
            bool ok = m_GContactsCache->storeContactsToDb();
            if(ok){
                rv->completed_callback();
            }
            else{
                std::unique_ptr<GoogleException> ex(new GoogleException("Failed to merge/batch/store DB cache."));
                rv->failed_callback(std::move(ex));
            }
        },
            [=](std::unique_ptr<GoogleException> ex)
        {
            rv->failed_callback(std::move(ex));
        }
    );
};

/**
    BatchResultContactInfo
*/
bool BatchResultContactInfo::parseEntryNode(QDomNode n)
{
    bool rv = ContactInfo::parseEntryNode(n);
    if (rv) {
        rv = parseBatchResult(n);
    }
    return rv;
};

/**
BatchResultGroupInfo
*/
bool BatchResultGroupInfo::parseEntryNode(QDomNode n)
{
    bool rv = GroupInfo::parseEntryNode(n);
    if (rv) {
        rv = parseBatchResult(n);
    }
    return rv;
};

std::unique_ptr<ContactList> ContactList::factory::create(const QByteArray& data)
{
    std::unique_ptr<ContactList> rv(new ContactList());
    rv->parseXml(data);
    return rv;
};

std::unique_ptr<GroupList> GroupList::factory::create(const QByteArray& data)
{
    std::unique_ptr<GroupList> rv(new GroupList());
    rv->parseXml(data);
    return rv;
};

std::unique_ptr<BatchContactList> BatchContactList::factory::create(const QByteArray& data)
{
    std::unique_ptr<BatchContactList> rv(new BatchContactList());
    rv->parseXml(data);
    return rv;
};

std::unique_ptr<BatchGroupList> BatchGroupList::factory::create(const QByteArray& data)
{
    std::unique_ptr<BatchGroupList> rv(new BatchGroupList());
    rv->parseXml(data);
    return rv;
};

std::shared_ptr<ContactInfo> ContactList::findNewCreatedContact(std::shared_ptr<BatchResultContactInfo> b)
{
    const NameInfo& n = b->name();
    QString name = n.givenName() + " " + n.familyName();
    
    for (auto& c : items()) {
        if(c->id().isEmpty()){
            const NameInfo& n2 = c->name();
            QString name2 = n2.givenName() + " " + n2.familyName();
            name2 = name2.trimmed();
            if(!name2.isEmpty()){
                if(name.compare(name2) == 0){
                    qDebug() << "located contact by name";
                    return c;
                }
            }
        }
    }

    std::shared_ptr<ContactInfo> rv;
    return rv;
};


#ifdef API_QT_AUTOTEST

std::unique_ptr<ContactInfo> ContactInfo::EXAMPLE(int param, int )
{
    QString email = QString("me%1@gmail.com").arg(param);
    QString first = QString("First-Name%1").arg(param);
    QString last = QString("Last-Name%1").arg(param);

    ContactInfo ci;
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


    ci.setName(n).setTitle("Title for " + first + " " + last)
        .addEmail(e1).addEmail(e2)
        .addPhone(p1).addPhone(p2)
        .setContent(QString("My notest on new contact for '%1'").arg(first))
        .setOrganizationInfo(o)
        .addAddress(a1).addAddress(a2);

    ci.m_etag = "my-contact-etag";
    ci.m_id = "my-contact-id";

    std::unique_ptr<ContactInfo> rv(new ContactInfo);
    QString xml = ci.toXml("me@gmail.com");
    rv->parseXml(xml);
    return rv;
}

std::unique_ptr<GroupInfo> GroupInfo::EXAMPLE(int context_index, int parent_content_index) 
{
    GroupInfo g;
    g.setContent(QString("cgroup-content %1-%2").arg(context_index).arg(parent_content_index));
    g.setTitle(QString("cgroup-title  %1-%2").arg(context_index).arg(parent_content_index));
    g.m_etag = "my-group-etag";
    g.m_id = "my-group-id";



    std::unique_ptr<GroupInfo> rv(new GroupInfo);
    QString xml = g.toXml("me@gmail.com");
    rv->parseXml(xml);
    return rv;
};

template <class T>
bool autoTestCheckImportExport(T* ci) 
{
    QString xml = ci->toXml("me@gmail.com");
    QByteArray data(xml.toStdString().c_str());

    QDomDocument doc;
    QString errorMsg = 0;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        ApiAutotest::INSTANCE() << QString("Failed to export contacts XML document: %1 line=%2 column=%2").arg(errorMsg).arg(errorLine).arg(errorColumn);
        ApiAutotest::INSTANCE() << xml;
        return false;
    }

    T ci2;
    if (!ci2.parseXml(data)) {
        ApiAutotest::INSTANCE() << "Contacts Xml parse error";
        return false;
    };

    bool rv = (*ci == ci2);
    if (!rv) {
        //this line is here to put breakpoint cause something is wrong
        //it's code can be compiled for debug anyway..
        rv = (*ci == ci2);
    }
    return rv;
}


std::unique_ptr<ContactInfo> createMergeContactParty(QString xmlOrigin) 
{
    std::unique_ptr<ContactInfo> c = std::unique_ptr<ContactInfo>(new ContactInfo);

    c->parseXml(xmlOrigin);
    c->setTitle(c->title() + "=NEW-TITLE=");
    c->setContent("=NEW-CONTENT=");

    OrganizationInfo o;
    o.setName("=NEW-organization-name=").setTitle("=NEW-title-in-the-organization=");
    c->setOrganizationInfo(o);
    NameInfo n;
    n.setFamilyName("=NEW-last=").setGivenName("=NEW-first").setFullName("=NEW-first_and_last=");
    c->setName(n);

    PhoneInfo p1, p2;
    std::list<PhoneInfo> lst;
    p1.setNumber("=NEW-1-111-1111=").setPrimary(true);
    p2.setNumber("=NEW-2-222-2222=").setPrimary(false);
    lst.push_back(p1);
    lst.push_back(p2);
    c->replacePhones(lst);

    EmailInfo e1, e2;
    e1.setAddress("=NEW-first-email=").setDisplayName("=NEW-first-last=").setPrimary(true).setTypeLabel("home");
    e2.setAddress("=NEW-second-email=").setDisplayName("=NEW-second-first-last=").setPrimary(false).setTypeLabel("work");
    std::list<EmailInfo> e_lst;
    e_lst.push_back(e1);
    e_lst.push_back(e2);
    c->replaceEmails(e_lst);

    PostalAddress a1, a2;
    a1.setCity("=NEW-ADDR-1=").setStreet("=NEW-STREET1=").setRegion("=NEW-REGION1=").setPostcode("=NEW-ZIP1=").setCountry("=NEW-COUNTRY1=").setPrimary(true);
    a2.setCity("=NEW-ADDR-2=").setStreet("=NEW-STREET2=").setRegion("=NEW-REGION2=").setPostcode("=NEW-ZIP2=").setCountry("=NEW-COUNTRY2=").setPrimary(false);
    std::list<PostalAddress> a_lst;
    a_lst.push_back(a1);
    a_lst.push_back(a2);
    c->replaceAddressList(a_lst);

    return c;
}

std::unique_ptr<GroupInfo> createMergeGroupParty(QString xmlOrigin)
{
    std::unique_ptr<GroupInfo> g = std::unique_ptr<GroupInfo>(new GroupInfo);
    g->parseXml(xmlOrigin);
    g->setTitle(g->title() + "=NEW-G-TITLE=");
    g->setContent("=NEW-G-CONTENT=");
    return g;
}

static bool autoTestCheckContactMerge(ContactInfo* ci)
{
    QString xml = ci->toXml("me@gmail.com");
    auto c = createMergeContactParty(xml);
    return autoTestCheckImportExport<ContactInfo>(c.get());
}

static bool autoTestCheckGroupMerge(GroupInfo* ci)
{
    QString xml = ci->toXml("me@gmail.com");
    auto c = createMergeGroupParty(xml);
    return autoTestCheckImportExport<GroupInfo>(c.get());
}

static void autoTestStoreAndCheckIdentityOnLoad(gcontact::contact_cache_ptr ccache)
{
    ContactList contact_list_copy = ccache->contacts();
    GroupList group_list_copy = ccache->groups();
    ccache->storeContactsToDb();
    ccache->loadContactsFromDb();
    if (contact_list_copy == ccache->contacts()) {
        ApiAutotest::INSTANCE() << QString("contacts-DB-identity - OK / %1").arg(contact_list_copy.items().size());
    }
    else {
        ApiAutotest::INSTANCE() << QString("contacts-DB-identity - ERROR / %1").arg(contact_list_copy.items().size());
    }

    if (group_list_copy == ccache->groups()) {
        ApiAutotest::INSTANCE() << QString("groups-DB-identity - OK / %1").arg(group_list_copy.items().size());
    }
    else {
        ApiAutotest::INSTANCE() << QString("groups-DB-identity - ERROR / %1").arg(group_list_copy.items().size());
    }
}

static void autoTestModifyRandomly(gcontact::contact_cache_ptr ccache)
{
    ContactList& contact_list_copy = ccache->contacts();
    GroupList& group_list_copy = ccache->groups();
    for (auto c : contact_list_copy.items()) {
        c->setTitle(c->title() + "-m");
        c->setContent(c->content() + "-m");

        NameInfo n = c->name();
        n.setFamilyName(n.familyName() + "-m");
        n.setGivenName(n.givenName() + "-m");
        n.setFullName(n.fullName() + "m");
        c->setName(n);

        OrganizationInfo o = c->organization();
        o.setName(o.name() + "-m");
        o.setTitle(o.title() += "-m");
        c->setOrganizationInfo(o);

        c->markAsModified();
    }

    for (auto g : group_list_copy.items()) {
        g->setTitle(g->title() + "-m");
        g->setContent(g->content() + "-m");

        g->markAsModified();
    }
}

void GcontactCacheRoutes::runAutotest() 
{
    ApiAutotest::INSTANCE() << "start-gcontact-test";
    ApiAutotest::INSTANCE() << "1";
    ApiAutotest::INSTANCE() << "2";
    ApiAutotest::INSTANCE() << "3";
    ApiAutotest::INSTANCE() << "4";

    /// generate entries
    ContactList& lst = m_GContactsCache->contacts();
    for (int i = 0; i < 10; i++) {
        auto c = ContactInfo::EXAMPLE(i, 0);
        if (!autoTestCheckImportExport<ContactInfo>(c.get())) {
            ApiAutotest::INSTANCE() << "contact Xml export/identity error";;
        }
        else {
            ApiAutotest::INSTANCE() << QString("contact-identity - OK / %1").arg(i+1);

            if (!autoTestCheckContactMerge(c.get())) {
                ApiAutotest::INSTANCE() << "contact Xml merge error";;
            }
            else {
                ApiAutotest::INSTANCE() << QString("contact-merge - OK / %1").arg(i + 1);
                lst.add(std::move(c));
            }
        }
    }

    /// generate groups
    GroupList& g_lst = m_GContactsCache->groups();
    for (int i = 0; i < 10; i++) {
        auto g = GroupInfo::EXAMPLE(i, 0);
        if (!autoTestCheckImportExport<GroupInfo>(g.get())) {
            ApiAutotest::INSTANCE() << "group Xml export/identity error";;
        }
        else {
            ApiAutotest::INSTANCE() << QString("group-identity - OK / %1").arg(i + 1);
            if (!autoTestCheckGroupMerge(g.get())) {
                ApiAutotest::INSTANCE() << "group Xml merge error";;
            }
            else {
                ApiAutotest::INSTANCE() << QString("group-merge - OK / %1").arg(i + 1);
                g_lst.add(std::move(g));
            }
        }
    }

    ApiAutotest::INSTANCE() << "test persistance identity";
    autoTestStoreAndCheckIdentityOnLoad(m_GContactsCache);
    autoTestModifyRandomly(m_GContactsCache);
    ApiAutotest::INSTANCE() << "test persistance after modification identity";
    autoTestStoreAndCheckIdentityOnLoad(m_GContactsCache);

    m_GContactsCache->clearDbCache();

    ApiAutotest::INSTANCE().enableRequestLog(false);

    std::function<void(QString)> auto_sync_step = [=](QString slabel) 
    {
        synchronizeContacts_Async()->waitForResultAndRelease();
        ApiAutotest::INSTANCE() << QString("%1 %2/%3")
            .arg(slabel)
            .arg(m_GContactsCache->groups().items().size())
            .arg(m_GContactsCache->contacts().items().size());
    };

    std::function<void(int, int)> auto_sync_reserveIds = [=](int start, int num) 
    {
        IDSET idset;
        for (int i = start; i < start + num; i++) {
            QString sid = QString("id-%1").arg(i);
            idset.insert(sid);
        }
        ApiAutotest::INSTANCE().addIdSet("gcontact::ContactsListResult", idset);
    };

    std::function<void()> auto_sync_modifyContacts = [=]() 
    {
        for (auto c : m_GContactsCache->contacts().items()) {
            c->setTitle("m-" + c->title());
            c->markAsModified();
        }
        for (auto g : m_GContactsCache->groups().items()) {
            g->setTitle("m-" + g->title());
            g->markAsModified();
        }
        m_GContactsCache->storeContactsToDb();
    };

    auto_sync_reserveIds(1, 10);
    auto_sync_step("Sync contacts-1/clone");
    auto_sync_reserveIds(1, 10);
    auto_sync_step("Sync contacts-2/copy-over");

    auto_sync_modifyContacts();

    auto_sync_reserveIds(1, 10);
    auto_sync_step("Sync contacts-3/copy-over-and-retire");

    auto_sync_modifyContacts();
    auto_sync_reserveIds(1, 2);
    auto_sync_step("Sync contacts-4/some-copy-over-and-retire-some-sync");


    ApiAutotest::INSTANCE().enableRequestLog(true);
    ApiAutotest::INSTANCE() << "";
};

static QString XmlContactsResponseSample(std::set<QString>& id_set)
{
    static const char* xml_entry_template =
        "<entry gd:etag = \"&quot;%1&quot;\">\n"
        "<id>http://www.google.com/m8/feeds/contacts/me%40gmail.com/base/%2</id>\n"
        "<updated>2017-12-27T11:33:51.728Z</updated>\n"
        "<app:edited xmlns:app = \"http://www.w3.org/2007/app\">2017-12-27T11:33:51.728Z</app:edited>\n"
        "<category scheme = \"http://schemas.google.com/g/2005#kind\" term=\"http://schemas.google.com/contact/2008#contact\"/>\n"
        "<title>test</title>\n"
        "<content>testNotes</content>\n"
        "<link rel = \"http://schemas.google.com/contacts/2008/rel#photo\" type=\"image/*\" href=\"https://www.googleapis.com/m8/feeds/photos/media/me%40gmail.com/asdfgh123\"/>\n"
        "<link rel = \"self\" type=\"application/atom+xml\" href=\"https://www.googleapis.com/m8/feeds/contacts/me%40gmail.com/full/asdfgh123\"/>\n"
        "<link rel = \"edit\" type=\"application/atom+xml\" href=\"https://www.googleapis.com/m8/feeds/contacts/me%40gmail.com/full/asdfgh123\"/>\n"
        "<gd:name>\n"
        "<gd:fullName>test</gd:fullName>\n"
        "<gd:givenName>%3</gd:givenName>\n"
        "</gd:name>\n"
        "<gd:organization rel = \"http://schemas.google.com/g/2005#other\">\n"
        "<gd:orgName>%4</gd:orgName>\n"
        "<gd:orgTitle>testJobTitle</gd:orgTitle>\n"
        "</gd:organization>\n"
        "<gd:email rel = \"http://schemas.google.com/g/2005#other\" address=\"%5\"/>\n"
        "<gd:phoneNumber rel = \"http://schemas.google.com/g/2005#home\">917 111-1111</gd:phoneNumber>\n"
        "<gd:phoneNumber rel = \"http://schemas.google.com/g/2005#work\" uri=\"tel:+1-917-222-2222\">917 222-2222</gd:phoneNumber>\n"
        "<gContact:groupMembershipInfo deleted = \"false\" href=\"http://www.google.com/m8/feeds/groups/me%40gmail.com/base/6\"/>\n"
        "</entry>\n";


    QString rv = "<?xml version = \"1.0\" encoding = \"UTF-8\"?>\n"
        "<feed gd:etag = \"&quot;qwertyfeeedvalue.&quot;\" xmlns = \"http://www.w3.org/2005/Atom\" xmlns:batch = \"http://schemas.google.com/gdata/batch\" xmlns:gContact = \"http://schemas.google.com/contact/2008\" xmlns:gd = \"http://schemas.google.com/g/2005\" xmlns:openSearch = \"http://a9.com/-/spec/opensearch/1.1/\">\n"
        "<id>me@gmail.com</id>\n";

    size_t entries_count = id_set.size();
    for (size_t i = 0; i < entries_count; i++) {
        QString etag = QString("etag%1").arg(i);
        auto it = id_set.begin();
        if (it == id_set.end()) {
            qWarning() << "internal autotest error/xml-generation";
            return "";
        }
        QString e_id = *it;
        id_set.erase(it);
        QString name = QString("name%1").arg(i);
        QString orga_name = QString("orga%1").arg(i);
        QString email = QString("email%1@me-site.org").arg(i);
        QString e = QString(xml_entry_template)
            .arg(etag)
            .arg(e_id)
            .arg(name)
            .arg(orga_name)
            .arg(email);

        rv += e;
    }

    rv += "</feed>\n";
    return rv;
}


static QString XmlContactsBatchResponseSample(BATCH_LIST& bid_lst)
{
    static const char* xml_batch_entry_template =
        "<entry gd:etag = \"&quot;%1&quot;\">\n"
        "%2\n"
        "<id>http://www.google.com/m8/feeds/contacts/me%40gmail.com/base/%3</id>\n"
        "<updated>2017-12-27T11:33:51.728Z</updated>\n"
        "<app:edited xmlns:app = \"http://www.w3.org/2007/app\">2017-12-27T11:33:51.728Z</app:edited>\n"
        "<category scheme = \"http://schemas.google.com/g/2005#kind\" term=\"http://schemas.google.com/contact/2008#contact\"/>\n"
        "<title>test</title>\n"
        "<content>testNotes</content>\n"
        "<link rel = \"http://schemas.google.com/contacts/2008/rel#photo\" type=\"image/*\" href=\"https://www.googleapis.com/m8/feeds/photos/media/me%40gmail.com/asdfgh123\"/>\n"
        "<link rel = \"self\" type=\"application/atom+xml\" href=\"https://www.googleapis.com/m8/feeds/contacts/me%40gmail.com/full/asdfgh123\"/>\n"
        "<link rel = \"edit\" type=\"application/atom+xml\" href=\"https://www.googleapis.com/m8/feeds/contacts/me%40gmail.com/full/asdfgh123\"/>\n"
        "<gd:name>\n"
        "<gd:fullName>test</gd:fullName>\n"
        "<gd:givenName>%4</gd:givenName>\n"
        "</gd:name>\n"
        "<gd:organization rel = \"http://schemas.google.com/g/2005#other\">\n"
        "<gd:orgName>%5</gd:orgName>\n"
        "<gd:orgTitle>testJobTitle</gd:orgTitle>\n"
        "</gd:organization>\n"
        "<gd:email rel = \"http://schemas.google.com/g/2005#other\" address=\"%6\"/>\n"
        "<gd:phoneNumber rel = \"http://schemas.google.com/g/2005#home\">917 111-1111</gd:phoneNumber>\n"
        "<gd:phoneNumber rel = \"http://schemas.google.com/g/2005#work\" uri=\"tel:+1-917-222-2222\">917 222-2222</gd:phoneNumber>\n"
        "<gContact:groupMembershipInfo deleted = \"false\" href=\"http://www.google.com/m8/feeds/groups/me%40gmail.com/base/6\"/>\n"
        "</entry>\n";


    QString rv = "<?xml version = \"1.0\" encoding = \"UTF-8\"?>\n"
        "<feed xmlns = \"http://www.w3.org/2005/Atom\" xmlns:batch = \"http://schemas.google.com/gdata/batch\" xmlns:gContact = \"http://schemas.google.com/contact/2008\" xmlns:gd = \"http://schemas.google.com/g/2005\">\n";

    
    size_t entries_count = bid_lst.size();
    for (size_t i = 0; i < entries_count; i++) {
        QString etag = QString("etag%1").arg(i);
        if (bid_lst.empty()) {
            qWarning() << "internal autotest error/xml-generation";
            return "";
        }
        auto p = bid_lst.front();
        bid_lst.pop_front();
        QString e_id = p.first;
        QString batch_xml = BatchResult::EXAMPLE(p.second);
        QString name = QString("name%1").arg(i);
        QString orga_name = QString("orga%1").arg(i);
        QString email = QString("email%1@me-site.org").arg(i);

        QString e = QString(xml_batch_entry_template)            
            .arg(etag)
            .arg(batch_xml)
            .arg(e_id)
            .arg(name)
            .arg(orga_name)
            .arg(email);

        rv += e;
    }    
    rv += "</feed>\n";
    return rv;
}


std::unique_ptr<ContactList> ContactList::EXAMPLE(int context_index, int parent_context_index)
{
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);

    IDSET idset = ApiAutotest::INSTANCE().getReservedIdSet("gcontact::ContactsListResult");
    if (idset.empty()) {
        idset.insert("c1id");
    }
    QByteArray d(XmlContactsResponseSample(idset).toStdString().c_str());
    std::unique_ptr<ContactList> rv(new ContactList());
    rv->parseXml(d);
    return rv;
};

std::unique_ptr<GroupList> GroupList::EXAMPLE(int, int)
{
    IDSET idset = ApiAutotest::INSTANCE().getReservedIdSet("gcontact::ContactGroupListResult");
    if (idset.empty()) {
        idset.insert("g1id");
    }
    QByteArray d(XmlContactsResponseSample(idset).toStdString().c_str());
    std::unique_ptr<GroupList> rv(new GroupList());
    rv->parseXml(d);
    return rv;
};

std::unique_ptr<BatchContactList> BatchContactList::EXAMPLE(int, int)
{
    auto bid_lst = ApiAutotest::INSTANCE().getReservedBatchList("gcontact::BatchContactList");
    if (bid_lst.empty()) {
        bid_lst.push_back(std::pair<QString, googleQt::EBatchId>("c1id", googleQt::EBatchId::retrieve));
    }
    QByteArray d(XmlContactsBatchResponseSample(bid_lst).toStdString().c_str());
    std::unique_ptr<BatchContactList> rv(new BatchContactList());
    rv->parseXml(d);
    return rv;
};

std::unique_ptr<BatchGroupList> BatchGroupList::EXAMPLE(int, int)
{
    auto bid_lst = ApiAutotest::INSTANCE().getReservedBatchList("gcontact::BatchGroupList");
    if (bid_lst.empty()) {
        bid_lst.push_back(std::pair<QString, googleQt::EBatchId>("g1id", googleQt::EBatchId::retrieve));
    }
    QByteArray d(XmlContactsBatchResponseSample(bid_lst).toStdString().c_str());
    std::unique_ptr<BatchGroupList> rv(new BatchGroupList());
    rv->parseXml(d);
    return rv;
};


#endif
