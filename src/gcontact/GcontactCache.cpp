#include <QSqlError>
#include <QDebug>
#include <ctime>
#include <QDir>
#include <QImageReader>
#include <QImageWriter>
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
std::unique_ptr<ContactInfo> ContactInfo::createWithId(QString contact_id)
{
    std::unique_ptr<ContactInfo> rv(new ContactInfo());
    rv->m_id = contact_id;
    return rv;
};


std::unique_ptr<ContactInfo> ContactInfo::cloneWithId(QString contact_id)
{
	std::unique_ptr<ContactInfo> rv(new ContactInfo());
	rv->assignContent(*this);
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
    m_emails.rebuildLabelsMap();
    return *this;
};

ContactInfo& ContactInfo::addPhone(const PhoneInfo& p) 
{
    m_phones.m_parts.push_back(p);
    markAsModified();
    m_phones.rebuildLabelsMap();
    return *this;
};

ContactInfo& ContactInfo::replaceEmails(const std::vector<EmailInfo>& lst)
{
    m_emails.m_parts.clear();
    markAsModified();
    for (auto& p : lst) {
        m_emails.m_parts.push_back(p);
    }
    m_emails.rebuildLabelsMap();
    return *this;
};

ContactInfo& ContactInfo::replacePhones(const std::vector<PhoneInfo>& lst)
{
    m_phones.m_parts.clear();
    markAsModified();
    
    for (auto& p : lst) {
        m_phones.m_parts.push_back(p);
    }
    m_phones.rebuildLabelsMap();
    return *this;
};


ContactInfo& ContactInfo::addAddress(const PostalAddress& p)
{
    m_address_list.m_parts.push_back(p);
    markAsModified();
    m_address_list.rebuildLabelsMap();
    return *this;
};

ContactInfo& ContactInfo::addGroup(const GroupMembershipInfo& p) 
{
    m_mgroups.m_parts.push_back(p);
    markAsModified();
    return *this;
};

ContactInfo& ContactInfo::setGroups(QString userId, const STRING_LIST& groupIdlist)
{
    m_mgroups.items().clear();
    for (auto gid : groupIdlist) {
        GroupMembershipInfo m(userId, gid);
        m_mgroups.m_parts.push_back(m);
    }
    markAsModified();
    return *this;
};

ContactInfo& ContactInfo::replaceAddressList(const std::vector<PostalAddress>& lst)
{
    m_address_list.m_parts.clear();
    markAsModified();
    for (auto& p : lst) {
        m_address_list.m_parts.push_back(p);
    }
    m_address_list.rebuildLabelsMap();
    return *this;
};


ContactInfo& ContactInfo::setOrganizationInfo(const OrganizationInfo& o) 
{
    m_organization = o;
    markAsModified();
    return *this;
};

ContactInfo& ContactInfo::addUserField(const UserDefinedFieldInfo& f) 
{
    m_user_def_fields.m_parts.push_back(f);
    markAsModified();
    return *this;
};

QString ContactInfo::toString()const 
{
    QString s = "";
    s += "id=" + m_id + ";etag=" + m_etag + ";updated=" + m_updated.toString(Qt::ISODate)
        + ";title=" + m_title + "content=" + m_content + ";"
        + m_name.toString() + ";"
        + m_emails.toString() + ";"
        + m_phones.toString() + ";"
        + m_organization.toString() + ";"
        + m_address_list.toString() + ";"
        + m_mgroups.toString() + ";"
        + m_user_def_fields.toString();
    return s;
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

    if (m_photo != o.m_photo) {
        return false;
    }

    if (m_mgroups != o.m_mgroups) {
        return false;
    }

    if (m_user_def_fields != o.m_user_def_fields) {
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
    rv += m_mgroups.toXmlString();
    rv += m_user_def_fields.toXmlString();
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
    m_mgroups.toXmlDoc(doc, entry_node);
    m_user_def_fields.toXmlDoc(doc, entry_node);
};


bool ContactInfo::parseEntryNode(QDomNode n)
{
    m_parsed_xml = "";
    QTextStream ss(&m_parsed_xml);
    n.save(ss, 4);
    ss.flush();

    if (!verifyXml(m_parsed_xml)) {
        qWarning() << "Failed to validate contact xml node";
        return false;
    }

    if (m_parsed_xml.isEmpty()) {
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
    m_mgroups = GroupMembershipInfoList::parse(n);
    m_user_def_fields = UserDefinedFieldInfoList::parse(n);
    m_photo = PhotoInfo::parse(n);

    m_emails.rebuildLabelsMap();
    m_phones.rebuildLabelsMap();
    m_address_list.rebuildLabelsMap();

    m_is_null = !m_etag.isEmpty() && m_id.isEmpty();
    return !m_is_null;
}

bool ContactInfo::setFromDbRecord(QSqlQuery* q)
{
    QString original_xml_string = q->value(1).toString();    
    QString currentXml = q->value(2).toString();
    if (!parseXml(currentXml)) {
        qWarning() << "failed to parse contact entry db record size="
                   << currentXml.size()
                   << "dbid=" << q->value(4).toInt();
        qWarning() << "-------------";
        return false;
    }
    bool ok = false;
    m_status = ContactXmlPersistant::validatedStatus(q->value(0).toInt(), &ok);
    if (!ok) {
        qWarning() << "Invalid contact db record status" << q->value(0).toInt();
    }

    setRegisterModifications(false); 
    
    m_updated = QDateTime::fromMSecsSinceEpoch(q->value(3).toLongLong());
    m_db_id = q->value(4).toInt();
    m_title = q->value(5).toString();
        
    QString photo_href = q->value(6).toString();
    QString photo_etag = q->value(7).toString();
    int st_tmp = q->value(8).toInt();
    
    if(st_tmp < 0 || st_tmp > 2){
        qWarning() << "Invalid DB value for photo status" << st_tmp;
        st_tmp = 0;
    }
    PhotoInfo::EStatus st = static_cast<PhotoInfo::EStatus>(st_tmp);
    
    m_photo.setupFromLocalDb(photo_href, photo_etag, st);

#ifdef API_QT_AUTOTEST
    if (m_id.isEmpty()) {
        /// normaly ID should be comming from server (original XML)
        /// but for autotest we can pick up from DB field
        m_id = q->value(9).toString();
    }
#endif

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
    m_mgroups = src.m_mgroups;
    m_user_def_fields = src.m_user_def_fields;

    m_emails.rebuildLabelsMap();
    m_phones.rebuildLabelsMap();
    m_address_list.rebuildLabelsMap();
};

std::unique_ptr<ContactInfo> ContactInfo::clone()const
{
	std::unique_ptr<ContactInfo> c(new ContactInfo);
	c->assignContent(*this);
	return c;
};

std::unique_ptr<BatchRequestContactInfo> ContactInfo::buildBatchRequest(googleQt::EBatchId batch_id)
{
    std::unique_ptr<BatchRequestContactInfo> rv(new BatchRequestContactInfo);
    rv->assignContent(*this);
    rv->setBatchid(batch_id);
    return rv;
};

void ContactInfo::markPhotoAsResolved() 
{
    m_photo.m_status = PhotoInfo::resolved;
    setDirty();
};

void ContactInfo::markPhotoAsModified() 
{
    m_photo.m_status = PhotoInfo::modified;
    setDirty();
};

bool ContactInfo::isPhotoResolved()const 
{ 
    return (m_photo.m_status == PhotoInfo::resolved); 
};

bool ContactInfo::isPhotoModified()const 
{ 
    return (m_photo.m_status == PhotoInfo::modified); 
};


/**
    GroupInfo
*/
std::unique_ptr<GroupInfo> GroupInfo::createWithId(QString group_id)
{
    std::unique_ptr<GroupInfo> rv(new GroupInfo());
    rv->m_id = group_id;
    return rv;
};

void GroupInfo::setupTitle(QString title) 
{
    m_title = title;
    int sys_idx = m_title.indexOf("System Group:");
    m_issystem_group = (sys_idx == 0);
    if (m_issystem_group) {
        m_display_title = m_title.mid(13).trimmed();
    }
};

QString GroupInfo::displayTitle()const 
{
    return isSystemGroup() ? m_display_title : m_title;
};

bool GroupInfo::parseEntryNode(QDomNode n)
{
    m_parsed_xml = "";
    QTextStream ss(&m_parsed_xml);
    n.save(ss, 4);
    ss.flush();

    if (m_parsed_xml.isEmpty()) {
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
    setupTitle(n.firstChildElement("title").text().trimmed());
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

std::unique_ptr<GroupInfo> GroupInfo::clone()const
{
	std::unique_ptr<GroupInfo> c(new GroupInfo);
	c->assignContent(*this);
	return c;
};

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
    markAsModified();
    return *this;
};

GroupInfo& GroupInfo::setContent(QString notes)
{
    m_content = notes;
    markAsModified();
    return *this;
};

bool GroupInfo::operator == (const GroupInfo& o) const
{
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
    QString group_id = q->value(6).toString();
    bool new_group = xml.isEmpty() && group_id.isEmpty();

    if (!new_group) {
#ifndef API_QT_AUTOTEST
        if (!parseXml(xml)) {
            qWarning() << "failed to parse contact group db record xml-size=" << xml.size();
            return false;
        }
#endif
    }

    bool ok = false;
    m_status = ContactXmlPersistant::validatedStatus(q->value(0).toInt(), &ok);
    if (!ok) {
        qWarning() << "Invalid contact db group record status" << q->value(0).toInt();
    }

    setRegisterModifications(false); 
    
    m_updated = QDateTime::fromMSecsSinceEpoch(q->value(2).toLongLong());
    m_db_id = q->value(3).toInt();
    setupTitle(q->value(4).toString());
    m_content = q->value(5).toString();
    m_id = q->value(6).toString();
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
GContactCache::GContactCache(googleQt::GoogleClient& gc):m_gc(gc)
{
};


QString GContactCache::metaPrefix()const 
{
	return m_sql_storage->m_metaPrefix;
};

QSqlQuery* GContactCache::prepareContactQuery(QString sql)
{
	return m_sql_storage->prepareContactQuery(sql);
};

bool GContactCache::execContactQuery(QString sql) 
{
	return m_sql_storage->execContactQuery(sql);
};

QSqlQuery* GContactCache::selectContactQuery(QString sql)
{
	return m_sql_storage->selectContactQuery(sql);
};

QString GContactCache::contactCacheDir()const
{
	return m_sql_storage->contactCacheDir();
};

QString	GContactCache::accountEmail()const
{
	return m_gc.userId();
};

void GContactCache::attachSQLStorage(mail_cache::GMailSQLiteStorage* ss)
{
	m_sql_storage = ss;
};


/**
    GContactCache
*/
GContactCacheBase::GContactCacheBase()
{

};

bool GContactCacheBase::ensureContactTables()
{
    /// contacts ///
    QString sql_entries = QString("CREATE TABLE IF NOT EXISTS %1gcontact_entry(contact_db_id INTEGER PRIMARY KEY, acc_email TEXT NOT NULL, entry_id TEXT, etag TEXT, title TEXT, "
                                  "full_name TEXT, xml_original TEXT, xml_current TEXT, updated INTEGER, status INTEGER, "
                                  "photo_href TEXT, photo_etag TEXT, photo_status INTEGER)").arg(metaPrefix());
    if (!execContactQuery(sql_entries))
        return false;

    QString sql_groups = QString("CREATE TABLE IF NOT EXISTS %1gcontact_group(group_db_id INTEGER PRIMARY KEY, acc_email TEXT NOT NULL, group_id TEXT, etag TEXT, title TEXT, content TEXT,"
        "xml_original TEXT, updated INTEGER, status INTEGER)").arg(metaPrefix());
    if (!execContactQuery(sql_groups))
        return false;

	if (m_enable_contacts_config_table) {
		QString sql_config = QString("CREATE TABLE IF NOT EXISTS %1gcontact_config(acc_email TEXT NOT NULL, config_name TEXT, config_value TEXT)").arg(metaPrefix());
		if (!execContactQuery(sql_config))
			return false;
	}

    return true;
};

#define MAKE_DB_UPDATE_LISTS                        \
    CLIST cloud_created_contacts;                   \
    CLIST updated_contacts;                         \
    CLIST limbo_id_contacts;                        \
    for (auto& c : contact_list) {                  \
        if (c->isDbIdNull()) {                      \
            cloud_created_contacts.push_back(c);    \
        }                                           \
        else {                                      \
            if (c->isDirty()) {                     \
                if (c->isIdLimbo()) {               \
                    c->markAsNormalCopy();          \
                    limbo_id_contacts.push_back(c); \
                }                                   \
                else {                              \
                    updated_contacts.push_back(c);  \
                }                                   \
                c->setDirty(false);                 \
            }                                       \
        }                                           \
    }                                               \


bool GContactCacheBase::storeContactList(std::vector<std::shared_ptr<ContactInfo>>& contact_list)
{    
    using CLIST = std::vector<ContactInfo::ptr>;
    MAKE_DB_UPDATE_LISTS;

    if (cloud_created_contacts.size() > 0) {
        qDebug() << "sql-contacts-store-cloud-created" << cloud_created_contacts.size();
        QString sql_insert;
        sql_insert = QString("INSERT INTO  %1gcontact_entry(acc_email, title, full_name, "
                             "xml_original, xml_current, updated, status, entry_id, etag, photo_href, photo_etag, photo_status)"
            " VALUES('%2', ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")
            .arg(metaPrefix())
            .arg(accountEmail());
        QSqlQuery* q = prepareContactQuery(sql_insert);
        if (!q) {
            qWarning() << "Failed to prepare contact insert-SQL" << sql_insert;
            return false;
        }

        for (auto c : cloud_created_contacts) {
            q->addBindValue(c->title());
            q->addBindValue(c->name().fullName());
            q->addBindValue(c->parsedXml());
            q->addBindValue(c->toXml(accountEmail()));
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
        sql_update = QString("UPDATE  %1gcontact_entry SET title=?, full_name=?, "
                             "xml_original=?, xml_current=?, updated=?, status=?, photo_href=?, photo_etag=?, photo_status=? WHERE contact_db_id=? AND UPPER(acc_email) = UPPER('%2')")
            .arg(metaPrefix())
            .arg(accountEmail());
        QSqlQuery* q = prepareContactQuery(sql_update);
        if (!q) {
            qWarning() << "Failed to prepare contact update-SQL" << sql_update;
            return false;
        }

        for (auto c : updated_contacts) {
            QString xml_current = c->toXml(accountEmail());
            //qDebug() << "sql-updated dbid=" << c->dbID() << "title=" << c->title() << "fullname=" << c->name().fullName();
            //qDebug() << xml_current;
            //qDebug() << "================================";
            q->addBindValue(c->title());
            q->addBindValue(c->name().fullName());
            q->addBindValue(c->parsedXml());
            q->addBindValue(xml_current);
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
        sql_update = QString("UPDATE  %1gcontact_entry SET title=?, full_name=?, "
                             "xml_original=?, xml_current=?, updated=?, status=?, entry_id=?, etag=?, photo_href=?, photo_etag=?, photo_status=? WHERE contact_db_id=? AND UPPER(acc_email) = UPPER('%2')")
            .arg(metaPrefix())
            .arg(accountEmail());
        QSqlQuery* q = prepareContactQuery(sql_update);
        if (!q) {
            qWarning() << "Failed to prepare contact update-SQL" << sql_update;
            return false;
        }

        for (auto c : limbo_id_contacts) {
            q->addBindValue(c->title());
            q->addBindValue(c->name().fullName());
            q->addBindValue(c->parsedXml());
            q->addBindValue(c->toXml(accountEmail()));
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

bool GContactCacheBase::storeGroupList(std::vector<std::shared_ptr<GroupInfo>>& contact_list)
{
    using CLIST = std::vector<GroupInfo::ptr>;
    MAKE_DB_UPDATE_LISTS;

    if (cloud_created_contacts.size() > 0) {
        qDebug() << "sql-groups-store-cloud-created" << cloud_created_contacts.size() << (*cloud_created_contacts.begin())->id();
        QString sql_insert;
        sql_insert = QString("INSERT INTO  %1gcontact_group(acc_email, title, content, xml_original, updated, status, group_id, etag)"
            " VALUES('%2', ?, ?, ?, ?, ?, ?, ?)")
            .arg(metaPrefix())
            .arg(accountEmail());

        QSqlQuery* q = prepareContactQuery(sql_insert);
        if (!q) {
            qWarning() << "Failed to prepare contact group insert-SQL" << sql_insert;
            return false;
        }

        for (auto c : cloud_created_contacts) {
            q->addBindValue(c->title());
            q->addBindValue(c->content());
            q->addBindValue(c->parsedXml());
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
    
    if (updated_contacts.size() > 0) {
        qDebug() << "sql-groups-store-updated" << updated_contacts.size() << (*updated_contacts.begin())->id();
        QString sql_update;
        sql_update = QString("UPDATE  %1gcontact_group SET title=?, content=?, xml_original=?, updated=?, status=? WHERE group_db_id=? AND UPPER(acc_email) = UPPER('%2')")
            .arg(metaPrefix())
            .arg(accountEmail());

        QSqlQuery* q = prepareContactQuery(sql_update);
        if (!q) {
            qWarning() << "Failed to prepare contact group update-SQL" << sql_update;
            return false;
        }

        for (auto c : updated_contacts) {
            q->addBindValue(c->title());
            q->addBindValue(c->content());
            q->addBindValue(c->parsedXml());
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

    if (limbo_id_contacts.size() > 0) {
        qDebug() << "sql-groups-store-limbo" << limbo_id_contacts.size() << (*limbo_id_contacts.begin())->id();
        QString sql_update;

        sql_update = QString("UPDATE  %1gcontact_group SET title=?, content=?, xml_original=?, updated=?, status=?, group_id=?, etag=? WHERE group_db_id=? AND UPPER(acc_email) = UPPER('%2')")
            .arg(metaPrefix())
            .arg(accountEmail());

        QSqlQuery* q = prepareContactQuery(sql_update);
        if (!q) {
            qWarning() << "Failed to prepare contact group update-SQL" << sql_update;
            return false;
        }

        for (auto c : limbo_id_contacts) {
            q->addBindValue(c->title());
            q->addBindValue(c->content());
            q->addBindValue(c->parsedXml());
            qlonglong upd_time = c->updated().toMSecsSinceEpoch();
            q->addBindValue(upd_time);
            q->addBindValue(c->status());
            q->addBindValue(c->id());
            q->addBindValue(c->etag());
            q->addBindValue(c->dbID());
            if (!q->exec()) {
                QString error = q->lastError().text();
                qWarning() << "ERROR. Failed to update contact group" << error;
                continue;
            }
        }
    }

    return true;
};

bool GContactCacheBase::storeContactEntries()
{
    if(!storeContactList(m_contacts.items())){
        return false;
    }
    
    if(!storeContactList(m_contacts.retired())){
        return false;
    }
    
    return true;
};

bool GContactCacheBase::storeContactGroups()
{
    if(!storeGroupList(m_groups.items())){
        return false;
    }
    
    if(!storeGroupList(m_groups.retired())){
        return false;
    }
    
    return true;
};


bool GContactCacheBase::storeConfig()
{
	if (!m_enable_contacts_config_table) {
		qWarning() << "config table not supported for external DB";
		return false;
	}

    if (m_sync_time.isValid()) {
        QString sql_del_old;
        sql_del_old = QString("DELETE FROM  %1gcontact_config WHERE UPPER(acc_email)=UPPER('%2') AND config_name='%3'")
            .arg(metaPrefix())
            .arg(accountEmail())
            .arg(CONFIG_SYNC_TIME);

        if (!execContactQuery(sql_del_old)) {
            qWarning() << "Failed to delete old contact config del-SQL" << sql_del_old;
            return false;
        };


        QString sql_insert;
        sql_insert = QString("INSERT INTO %1gcontact_config(acc_email, config_name, config_value)"
            " VALUES('%2', ?, ?)")
            .arg(metaPrefix())
            .arg(accountEmail());

        QSqlQuery* q = prepareContactQuery(sql_insert);
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

bool GContactCacheBase::storeContactsToDb()
{
    if (!storeContactEntries()) {
        return false;
    }

    if (!storeContactGroups()) {
        return false;
    }

	if (m_enable_contacts_config_table) {
		if (!storeConfig()) {
			return false;
		}
	}
    
    return true;
};

bool GContactCacheBase::loadContactsFromDb()
{
    if (!loadContactEntriesFromDb()) {
        return false;
    }

    if (!loadContactGroupsFromDb()) {
        return false;
    }

    if (m_enable_contacts_config_table) {
      if (!loadContactConfigFromDb()) {
	return false;
      }
    }

    //qWarning() << "ykh/loadContactsFromDb entries=" << contacts().items().size() << "groups=" << groups().items().size();
    
    return true;
};

bool GContactCacheBase::clearDbCache()
{
    m_contacts.clear();
    m_groups.clear();

    QDateTime invalid_time;
    m_sync_time.swap(invalid_time);

    QString sql = QString("DELETE FROM %1gcontact_group WHERE UPPER(acc_email)=UPPER('%2')")
        .arg(metaPrefix())
        .arg(accountEmail());

    if (!execContactQuery(sql)) {
        return false;
    };

    sql = QString("DELETE FROM %1gcontact_entry WHERE UPPER(acc_email)=UPPER('%2')")
        .arg(metaPrefix())
        .arg(accountEmail());

    if (!execContactQuery(sql)) {
        return false;
    };

    return true;
};

bool GContactCacheBase::mergeServerModifications(GroupList& server_glist, ContactList& server_clist)
{
    m_contacts.mergeList(server_clist);
    m_groups.mergeList(server_glist);

    m_sync_time = QDateTime::currentDateTime();

    bool rv = storeContactsToDb();
    return rv;
};

bool GContactCacheBase::loadContactGroupsFromDb()
{
    m_groups.clear();

	QString email = accountEmail();
    QString sql = QString("SELECT status, xml_original, updated, group_db_id, title, content, group_id FROM %1gcontact_group WHERE UPPER(acc_email)=UPPER('%2') AND status IN(1,2,3) ORDER BY updated DESC")
        .arg(metaPrefix())
        .arg(email);
    QSqlQuery* q = selectContactQuery(sql);
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

bool GContactCacheBase::loadContactEntriesFromDb()
{
    m_contacts.clear();

	QString email = accountEmail();
    QString sql = QString("SELECT status, xml_original, xml_current, updated, contact_db_id, title, "
        "photo_href, photo_etag, photo_status, entry_id FROM %1gcontact_entry WHERE UPPER(acc_email)=UPPER('%2') AND status IN(1,2,3) ORDER BY updated DESC")
        .arg(metaPrefix())
        .arg(email);
    QSqlQuery* q = selectContactQuery(sql);
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

bool GContactCacheBase::loadContactConfigFromDb()
{
	if (!m_enable_contacts_config_table) {
		qWarning() << "config table not supported for external DB";
		return false;
	}

    m_configs.clear();
    
    QDateTime invalid_time;
    m_sync_time.swap(invalid_time);
    
    QString sql = QString("SELECT MAX(updated) FROM %1gcontact_group WHERE UPPER(acc_email)=UPPER('%2') AND status IN(1,2,3)")
        .arg(metaPrefix())
        .arg(accountEmail());
    QSqlQuery* q = selectContactQuery(sql);
    if (!q){
        qWarning() << "failed to load group MAX(updated)";
        return false;
    }
    
    if(q->next() && q->value(0).toLongLong() > 0){
        m_sync_time = QDateTime::fromMSecsSinceEpoch(q->value(0).toLongLong());
    }

    
    sql = QString("SELECT MAX(updated) FROM %1gcontact_entry WHERE UPPER(acc_email)=UPPER('%2') AND status IN(1,2,3)")
        .arg(metaPrefix())
        .arg(accountEmail());
    q = selectContactQuery(sql);
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


    sql = QString("SELECT config_name, config_value FROM %1gcontact_config WHERE UPPER(acc_email)=UPPER('%2')")
        .arg(metaPrefix())
        .arg(accountEmail());
    q = selectContactQuery(sql);
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


QString GContactCacheBase::getPhotoMediaPath(QString contactId, bool ensurePath)const
{
    QString cache_dir = contactCacheDir();
    cache_dir += "/contact-photos/";
    cache_dir += accountEmail();
    cache_dir += "/";
    QString img_file = cache_dir + QString("%1.jpg").arg(contactId);

    if (ensurePath) {
        QFileInfo fi(img_file);
        if (fi.exists()) {
            return img_file;
        }

        QDir d;

        QString path = fi.path();
        if (!d.exists(path)) {
            if (!d.mkpath(path)) {
                QString errStr = QString("Failed to create directory '%1'").arg(path);
                qWarning() << errStr;
                return "";
            };
        }
    }

    return img_file;    
};

bool GContactCacheBase::addPhoto(ContactInfo::ptr c, QString photoFileName)
{
    if (!QFile::exists(photoFileName)) {
        qWarning() << "Photo file not found " << photoFileName;
        return false;
    }

    QString targetPath = getPhotoMediaPath(c->id(), true);
    if (QFile::exists(targetPath)) {
        if (!QFile::remove(targetPath)) {
            qWarning() << "Failed to remove cache photo file " << photoFileName;
            return false;
        }
    }

    QImageReader reader;
    reader.setFileName(photoFileName);
    QImage image = reader.read();
    if (image.isNull()) {
        qWarning() << "Failed to read cache photo file " << photoFileName;
        return false;
    }

    QImageWriter writer;
    writer.setFormat("jpg");
    writer.setFileName(targetPath);
    if (!writer.write(image)) {
        qWarning() << "Failed to store cache photo file " << targetPath;
        return false;
    }
    /*
    if (!QFile::copy(photoFileName, targetPath)) {
        qWarning() << "Failed to copy photo file " << photoFileName << " to " << targetPath;
        return false;
    }*/

    c->markPhotoAsModified();
    return true;
};

ContactsReplicaSyncResult GContactCacheBase::synchronizeContactsReplica(GContactCacheBase* remoteMasterContacts)
{	
	ContactsReplicaSyncResult rv;

	auto& g_local = groups();
	auto& g_remote = remoteMasterContacts->groups();
	rv.groups = g_local.synchronizeReplica(g_remote);

	auto& c_local = contacts();
	auto& c_remote = remoteMasterContacts->contacts();
	rv.contacts = c_local.synchronizeReplica(c_remote);

	return rv;
};

GoogleTask<QString>* GcontactCacheRoutes::getContactCachePhoto_Async(ContactInfo::ptr c)
{
    GoogleTask<QString>* rv = m_endpoint.produceTask<QString>();
    QString filePath = m_endpoint.client()->contacts_cache()->getPhotoMediaPath(c->id(), true);
    if (filePath.isEmpty()) {
        QString errStr = QString("Failed to ensure photo path '%1'").arg(c->id());
        std::unique_ptr<GoogleException> ex(new GoogleException(errStr.toStdString()));
        rv->failed_callback(std::move(ex));
        return rv;
    }

    QFileInfo fi(filePath);
    if (fi.exists()) {
        std::unique_ptr<QString> s(new QString(filePath));
        rv->completed_callback(std::move(s));
        return rv;
    }    
            
    QFile* out = new QFile();
    out->setFileName(filePath);
    if (!out->open(QFile::WriteOnly | QIODevice::Truncate)) {
        QString errStr = QString("Failed to open file '%1'").arg(filePath);
        qWarning() << errStr;
        std::unique_ptr<GoogleException> ex(new GoogleException(errStr.toStdString()));
        rv->failed_callback(std::move(ex));
        return rv;
    }
   
    DownloadPhotoArg arg(c->id());
    auto t = m_c_routes.getContacts()->getContactPhoto_Async(arg, out);
    t->then([=]() 
            {
                std::unique_ptr<QString> s2(new QString(filePath));
                rv->completed_callback(std::move(s2));
            },
            [=](std::unique_ptr<GoogleException> ex) 
            {
                rv->failed_callback(std::move(ex));
            }, 
            [=]() 
            {
                out->flush();
                delete(out);
            });
    
    return rv;
};

/**
    PhotoReceiver
*/
namespace googleQt {
    namespace gcontact {
        class PhotoReceiver
        {
        public:
            PhotoReceiver(GcontactRoutes& r) :m_r(r) {};
            GoogleVoidTask* routeRequest(QString contact_id);
        protected:
            GcontactRoutes&     m_r;
        };
    }
}

GoogleVoidTask* PhotoReceiver::routeRequest(QString contact_id)
{
	auto cache = m_r.endpoint().client()->contacts_cache();//m_r.cacheRoutes()->contacts_cache();
    QString filePath = cache->getPhotoMediaPath(contact_id, true);
    if (filePath.isEmpty()) {
        GoogleVoidTask* t = m_r.endpoint().produceVoidTask();
        QString errStr = QString("Failed to ensure photo path '%1'").arg(contact_id);
        std::unique_ptr<GoogleException> ex(new GoogleException(errStr.toStdString()));
        t->failed_callback(std::move(ex));
        return t;
    }

    QFile* out = new QFile();
    out->setFileName(filePath);
    if (!out->open(QFile::WriteOnly | QIODevice::Truncate)) {
        GoogleVoidTask* t = m_r.endpoint().produceVoidTask();
        QString errStr = QString("Failed to open file '%1'").arg(filePath);
        qWarning() << errStr;
        std::unique_ptr<GoogleException> ex(new GoogleException(errStr.toStdString()));
        t->failed_callback(std::move(ex));
        return t;
    }

    DownloadPhotoArg arg(contact_id);
    GoogleVoidTask* t = m_r.getContacts()->getContactPhoto_Async(arg, out);
    t->addFinishedDelegate([out]()
    {
        out->flush();
        delete(out);
    });
    return t;
};

/**
    PhotoUploader
*/
namespace googleQt {
    namespace gcontact {
        class PhotoUploader
        {
        public:
            PhotoUploader(GcontactRoutes& r) :m_r(r) {};
            GoogleVoidTask* routeRequest(QString contact_id);
        protected:
            GcontactRoutes&     m_r;
        };
    }
}

GoogleVoidTask* PhotoUploader::routeRequest(QString contact_id)
{
    auto cache = m_r.endpoint().client()->contacts_cache();
    QString filePath = cache->getPhotoMediaPath(contact_id, true);
    if (filePath.isEmpty()) {
        GoogleVoidTask* t = m_r.endpoint().produceVoidTask();
        QString errStr = QString("Failed to ensure photo path '%1'").arg(contact_id);
        std::unique_ptr<GoogleException> ex(new GoogleException(errStr.toStdString()));
        t->failed_callback(std::move(ex));
        return t;
    }

    QFile* in = new QFile();
    in->setFileName(filePath);
    if (!in->open(QFile::ReadOnly)) {
        GoogleVoidTask* t = m_r.endpoint().produceVoidTask();
        QString errStr = QString("Failed to open file '%1'").arg(filePath);
        qWarning() << errStr;
        std::unique_ptr<GoogleException> ex(new GoogleException(errStr.toStdString()));
        t->failed_callback(std::move(ex));
        return t;
    }

    UploadPhotoArg arg(contact_id);
    GoogleVoidTask* t = m_r.getContacts()->uploadContactPhoto_Async(arg, in);
    t->addFinishedDelegate([in]()
    {
        delete(in);
    });
    return t;
}

template <class PROCESSOR>
GcontactCacheRoutes::PhotoListTask* GcontactCacheRoutes::transferPhotos_Async(const STRING_LIST& id_list)
{
    GcontactCacheRoutes::PhotoListTask* rv = new GcontactCacheRoutes::PhotoListTask(m_endpoint.apiClient());
    if (!id_list.empty()) {
        std::unique_ptr<PROCESSOR> pr(new PROCESSOR(m_c_routes));
        ConcurrentArgRunner<QString, PROCESSOR>* r = new ConcurrentArgRunner<QString, PROCESSOR>(id_list, std::move(pr), m_endpoint.apiClient());
        r->run();
        connect(r, &EndpointRunnable::finished, [=]()
        {
            //STRING_LIST completed_ids = r->completedArgList();
            rv->m_completed_ids = r->completedArgList();
            //progress_status = completed_ids.size();
            for (auto c_id : rv->m_completed_ids) {
                auto c = m_endpoint.client()->contacts_cache()->contacts().findById(c_id);
                if (c) {
                    c->markPhotoAsResolved();
                }
                else {
                    qWarning() << "failed to locate contact by ID" << c_id;
                }
            }
            
            rv->completed_callback();
            r->disposeLater();
        });
    }
    else {
        rv->completed_callback();
    }

    return rv;
};


GcontactCacheRoutes::PhotoListTask* GcontactCacheRoutes::downloadPhotos_Async()
{
    STRING_LIST id_list = m_endpoint.client()->contacts_cache()->contacts().buildUnresolvedPhotoIdList();
    return transferPhotos_Async<gcontact::PhotoReceiver>(id_list);
};

GcontactCacheRoutes::PhotoListTask* GcontactCacheRoutes::uploadPhotos_Async()
{
    STRING_LIST id_list = m_endpoint.client()->contacts_cache()->contacts().buildModifiedPhotoIdList();
    return transferPhotos_Async<gcontact::PhotoUploader>(id_list);
};

PhotoSyncTask* GcontactCacheRoutes::synchronizePhotos_Async()
{
    PhotoSyncTask* t = new PhotoSyncTask(m_endpoint.apiClient());

    auto t1 = downloadPhotos_Async();
    t1->then([=]() {
        auto t2 = uploadPhotos_Async();
        t2->then([=]() 
        {
            t->m_downloaded_ids = t1->m_completed_ids;
            t->m_uploaded_ids = t2->m_completed_ids;
			m_endpoint.client()->contacts_cache()->storeContactsToDb();
            t->completed_callback();
        },
            [=](std::unique_ptr<GoogleException> ex)
        {
            t->failed_callback(std::move(ex));
        }, [=]() 
        {
            t2->disposeLater();
        });
    }, 
        [=](std::unique_ptr<GoogleException> ex) 
    {
        t->failed_callback(std::move(ex));
    },
        [=]() 
    {
        t1->disposeLater();
    });

    return t;
};

GcontactCacheRoutes::GcontactCacheRoutes(googleQt::Endpoint& endpoint, GcontactRoutes& r):m_endpoint(endpoint), m_c_routes(r)
{
    //m_GContactsCache.reset(new GContactCache(endpoint.apiClient()->userId()));
};

template<class B, class L>
void applyBatchStep(std::shared_ptr<B> b, L& lst)
{
    if (b->succeded()) {
        auto c = lst.findById(b->id());
        if (c) {
            switch (b->batchResultId())
            {
            case EBatchId::delete_operation:
            {
                lst.retire(c);
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
        }//c - found
        else {
            if (b->batchResultId() == EBatchId::create) {
                auto c = lst.findNewCreated(b);
                if (c) {
                    c->assignContent(*(b.get()));
                    lst.idlimbo(c);
                }
                else {
                    qWarning() << "Cache/create obj locate on batch operation failed"
                        << b->id()
                        << b->batchResultOperationType()
                        << b->batchResultStatusReason();
                }
            }
            else {
                qWarning() << "Cache obj locate on batch operation failed"
                    << b->id()
                    << b->batchResultOperationType()
                    << b->batchResultStatusReason();
            }
        }//c - not found
    }
    else {// !succeded()
        auto c = lst.findById(b->id());

        if (c && b->batchResultStatusCode() == 404 && b->batchResultId() == EBatchId::delete_operation) {
            lst.retire(c);
        }
        else {
            qWarning() << "Batch operation failed"
                << b->id()
                << b->batchResultOperationType()
                << b->batchResultStatusReason();
        }
    }
}

GcontactCacheSyncTask* GcontactCacheRoutes::synchronizeContacts_Async()
{
    GcontactCacheSyncTask* rv = new GcontactCacheSyncTask(m_endpoint.apiClient());
	auto cc = m_endpoint.client()->contacts_cache();

    QDateTime dtUpdatedMin = cc->lastSyncTime();

    if (!dtUpdatedMin.isValid()) {
        ///first time sync, purge local cache just in case..
        if (!cc->clearDbCache()) {
            std::unique_ptr<GoogleException> ex(new GoogleException("Failed to clear local DB cache."));
            rv->failed_callback(std::move(ex));
            return rv;
        }
    }
    else {
        /// 1 sec - seems resonable for time diff on updated items
        /// we take all contacts from server newer than last sync + 1 sec
        dtUpdatedMin = dtUpdatedMin.addSecs(1).toUTC();
    }

    // sync as follows:
    // 1. local group changes first
    // 2. server group & contacts changes
    // 3. local contact changes

    //    std::vector<std::shared_ptr<BatchRequestGroupInfo>> delete_batch_requests;
    
    BatchRequesGroupList bg = cc->groups().buildBatchRequestList();
    for (auto& gbr : bg.items()){
        if(gbr->batchId() == EBatchId::delete_operation){
            rv->m_deleted_groups[gbr->id()] = gbr;
        }
    }
    if (!bg.items().empty()) {
        BatchGroupArg arg(bg);
        auto groups_btask = m_endpoint.client()->gcontact()->getContactGroup()->batch_Async(arg);
        groups_btask->then([=](std::unique_ptr<gcontact::BatchGroupList> rlst)
        {
			auto cc = m_endpoint.client()->contacts_cache();

            rv->m_updated_groups = std::move(rlst);
            auto& arr = rv->m_updated_groups->items();
            for (auto& b : arr) {
                applyBatchStep<BatchResultGroupInfo, GroupList>(b, cc->groups());
            }
            ///update deleted groups
            for (auto& gbr_it : rv->m_deleted_groups){
                auto g = cc->groups().findById(gbr_it.first);
                if (g) {
                    cc->groups().retire(g);
                }
                else{
                    qWarning() << "Batch-delete operation error - failed to locate group id"
                               << gbr_it.first;
                }
            }
            bool ok = cc->storeContactsToDb();
            if(!ok){
                qWarning() << "failed to store contacts";
            }
            reloadCache_Async(rv, dtUpdatedMin);
        },
            [=](std::unique_ptr<GoogleException> ex)
        {
            rv->failed_callback(std::move(ex));
        });
    }
    else {
        reloadCache_Async(rv, dtUpdatedMin);
    }
    return rv;
};


void GcontactCacheRoutes::reloadCache_Async(GcontactCacheSyncTask* rv, QDateTime dtUpdatedMin)
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

    TaskAggregator* agg = new TaskAggregator(m_endpoint.apiClient());
    agg->add(entries_task);
    agg->add(groups_task);
    agg->then([=]() 
    {
        rv->m_loaded_contacts = entries_task->detachResult();
        rv->m_loaded_groups.reset(new GroupList);
        std::unique_ptr<GroupList> loaded_groups = groups_task->detachResult();
        //rv->m_loaded_groups = groups_task->detachResult();

        for(auto& gi : loaded_groups->items()){
            if(rv->deleted_groups().find(gi->id()) == rv->deleted_groups().end()){
                rv->m_loaded_groups->add(gi);
            }
            else{
                qDebug() << "g-update-skipped-as-deleted" << gi->id() << gi->title();
            }
        }
        
        if (m_endpoint.client()->contacts_cache()->mergeServerModifications(*(rv->m_loaded_groups.get()), *(rv->m_loaded_contacts.get()))) {
            //at this point we should have loaded server changes
            //now we will try to apply our changes - modifications and deleted records

            applyLocalContacEntriesModifications_Async(rv);
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

void GcontactCacheRoutes::applyLocalContacEntriesModifications_Async(GcontactCacheSyncTask* rv)
{
    BatchRequesContactList bc = m_endpoint.client()->contacts_cache()->contacts().buildBatchRequestList();
    BatchContactArg arg(bc);

    auto entries_btask = m_endpoint.client()->gcontact()->getContacts()->batch_Async(arg);
    entries_btask->then(
        [=](std::unique_ptr<gcontact::BatchContactList> rlst) 
        {
            rv->m_updated_contacts = std::move(rlst);

            auto& arr = rv->m_updated_contacts->items();
            for (auto& b : arr) {
                applyBatchStep<BatchResultContactInfo, ContactList>(b, m_endpoint.client()->contacts_cache()->contacts());
            }
            
            bool ok = m_endpoint.client()->contacts_cache()->storeContactsToDb();
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

std::shared_ptr<GroupInfo> GroupList::findNewCreated(std::shared_ptr<BatchResultGroupInfo> b)
{    
    QString title = b->title();

    for (auto& c : items()) {
        if (c->id().isEmpty()) {
            QString title2 = c->title().trimmed();
            if (!title2.isEmpty()) {
                if (title.compare(title2) == 0) {
                    qDebug() << "located group by title";
                    return c;
                }
            }
        }
    }

    std::shared_ptr<GroupInfo> rv;
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

std::shared_ptr<ContactInfo> ContactList::findNewCreated(std::shared_ptr<BatchResultContactInfo> b)
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



STRING_LIST ContactList::buildUnresolvedPhotoIdList()
{
    STRING_LIST lst;
    for (auto& c : items()) {
        if (!c->id().isEmpty() && 
            !c->photo().etag().isEmpty() &&
            c->photo().status() == PhotoInfo::not_resolved) 
        {
            lst.push_back(c->id());
        }
    }

    return lst;
};

STRING_LIST ContactList::buildModifiedPhotoIdList()
{
    STRING_LIST lst;
    for (auto& c : items()) {
        if (!c->id().isEmpty() &&
            //!c->photo().etag().isEmpty() &&
            c->photo().status() == PhotoInfo::modified)
        {
            lst.push_back(c->id());
        }
    }

    return lst;
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
    GroupMembershipInfo g;
    UserDefinedFieldInfo f1, f2;

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
    p1.setNumber(QString("1-111-111%1").arg(param)); 
    p1.setPrimary(true);
    p2.setNumber(QString("2-222-222%1").arg(param));
    p2.setPrimary(false);
    o.setName("1-organization-name"); 
    o.setTitle("title-in-the-organization");
    a1.setCity("Mountain View-1"); 
    a1.setTypeLabel("work");
    a1.setStreet(QString("1111-%1 Amphitheatre Pkwy").arg(param));
    a1.setRegion("CA"); 
    a1.setPostcode(QString("11111-%1").arg(param));
    a1.setCountry("United States");
    a1.setPrimary(true);

    a2.setCity("Mountain View-2"); 
    a2.setTypeLabel("home");
    a2.setStreet(QString("2222-%1 Amphitheatre Pkwy").arg(param));
    a2.setRegion("NY"); 
    a2.setPostcode(QString("22222-%1").arg(param));
    a2.setCountry("United States");
    a2.setPrimary(false);


    ci.setName(n).setTitle("Title for " + first + " " + last)
        .addEmail(e1).addEmail(e2)
        .addPhone(p1).addPhone(p2)
        .setContent(QString("My notest on new contact for '%1'").arg(first))
        .setOrganizationInfo(o)
        .addAddress(a1).addAddress(a2);

    g.setUserId("me%1@gmail.com");
    g.setGroupId("my-group-etag");
    ci.addGroup(g);

    f1.setKey("key1");
    f1.setValue("value1");
    f2.setKey("key2");
    f2.setValue("value2");
    ci.addUserField(f1);
    ci.addUserField(f2);

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
    o.setName("=NEW-organization-name="); 
    o.setTitle("=NEW-title-in-the-organization=");
    c->setOrganizationInfo(o);
    NameInfo n;
    n.setFamilyName("=NEW-last=");
    n.setGivenName("=NEW-first");
    n.setFullName("=NEW-first_and_last=");
    c->setName(n);

    PhoneInfo p1, p2;
    std::vector<PhoneInfo> lst;
    p1.setNumber("=NEW-1-111-1111=");
    p1.setPrimary(true);
    p2.setNumber("=NEW-2-222-2222=");
    p2.setPrimary(false);
    lst.push_back(p1);
    lst.push_back(p2);
    c->replacePhones(lst);

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
    c->replaceEmails(e_lst);

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

static void autoTestStoreAndCheckIdentityOnLoad(gcontact::GContactCacheBase* ccache)
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

static void autoTestModifyRandomly(gcontact::GContactCacheBase* ccache)
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
    ContactList& lst = m_endpoint.client()->contacts_cache()->contacts();
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
    GroupList& g_lst = m_endpoint.client()->contacts_cache()->groups();
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
    autoTestStoreAndCheckIdentityOnLoad(m_endpoint.client()->contacts_cache());
    autoTestModifyRandomly(m_endpoint.client()->contacts_cache());
    ApiAutotest::INSTANCE() << "test persistance after modification identity";
    autoTestStoreAndCheckIdentityOnLoad(m_endpoint.client()->contacts_cache());

	m_endpoint.client()->contacts_cache()->clearDbCache();

    ApiAutotest::INSTANCE().enableRequestLog(false);

    std::function<void(QString)> auto_sync_step = [=](QString slabel) 
    {
        synchronizeContacts_Async()->waitForResultAndRelease();
        ApiAutotest::INSTANCE() << QString("%1 %2/%3")
            .arg(slabel)
            .arg(m_endpoint.client()->contacts_cache()->groups().items().size())
            .arg(m_endpoint.client()->contacts_cache()->contacts().items().size());
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


	auto cc = m_endpoint.client()->contacts_cache();

    std::function<void()> auto_sync_modifyContacts = [=]() 
    {
        for (auto c : cc->contacts().items()) {
            c->setTitle("m-" + c->title());
            c->markAsModified();
        }
        for (auto g : cc->groups().items()) {
            g->setTitle("m-" + g->title());
            g->markAsModified();
        }
        cc->storeContactsToDb();
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
        "<gContact:userDefinedField key=\"k1\" value=\"v1\"/>\n"
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
