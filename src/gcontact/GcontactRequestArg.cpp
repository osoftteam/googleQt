#include <QUrlQuery>
#include "GcontactRequestArg.h"

using namespace googleQt;
using namespace gcontact;

/**
   ContactsListArg
*/
ContactsListArg::ContactsListArg(QString contactId /*= ""*/):m_contactId(contactId)
{

};

void ContactsListArg::build(const QString& link_path, QUrl& url)const{
    QString s_link_path = link_path;
    if (!m_contactId.isEmpty()) {
        s_link_path = link_path + QString("/%1").arg(m_contactId);
    }
    UrlBuilder b(s_link_path, url);
    if(!m_alt.isEmpty()){
        b.add("alt", m_alt);
    }
    if(!m_q.isEmpty()){
        b.add("q", m_q);
    }
    if(m_max_results > 0){
        b.add("max-results", m_max_results);
    }

    if (m_updated_min.isValid()) {
        b.add("updated-min", m_updated_min.toString(Qt::ISODate));
    }

    if(!m_orderby.isEmpty()){
        b.add("orderby", m_orderby);
    }

    if(m_showdeleted){
        b.add("showdeleted", "true");
    }

    if(m_requirealldeleted){
        b.add("requirealldeleted", "true");
    }
    
    if(!m_sortorder.isEmpty()){
        b.add("sortorder", m_sortorder);
    }
    
    if(!m_group.isEmpty()){
        b.add("group", m_group);
    }    
};


/**
    DeleteContactArg
*/
DeleteContactArg::DeleteContactArg()
{

};

DeleteContactArg::DeleteContactArg(QString contact_id, QString etag):
    m_contact_id(contact_id), m_etag(etag) 
{

};

void DeleteContactArg::build(const QString& link_path, QUrl& url)const
{
    QString s_link_path = link_path;
    if (!m_contact_id.isEmpty()) {
        s_link_path = link_path + QString("/%1").arg(m_contact_id);
    }
    UrlBuilder b(s_link_path, url);
};

QString DeleteContactArg::etag()const
{
    QString rv = m_etag;
    if (m_ignore_etag) {
        rv = "*";
    }
    return rv;
};


/**
    ContactsListResult
*/
ContactsListResult::ContactsListResult(const QByteArray& data) 
{
    m_data.reset(new ContactList);
    m_is_null = !m_data->parseXml(data);
};

std::unique_ptr<ContactsListResult> ContactsListResult::factory::create(const QByteArray& data)
{
    std::unique_ptr<ContactsListResult> rv = std::unique_ptr<ContactsListResult>(new ContactsListResult(data));
    return rv;
};

ContactList* ContactsListResult::data() 
{
    return m_data.get();
};

QString ContactsListResult::toString(bool )const
{
    return m_data->toString();
};

/**
    CreateContactArg
*/
CreateContactArg::CreateContactArg() 
{

};

CreateContactArg::CreateContactArg(const ContactInfo& ci) :m_contact_info(ci)
{

};

void CreateContactArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
};

QString CreateContactArg::toXml(QString userId)const
{
    return m_contact_info.toXml(userId);
};

/**
UpdateContactArg
*/
UpdateContactArg::UpdateContactArg() 
{

};

UpdateContactArg::UpdateContactArg(const ContactInfo& ci) :m_contact_info(ci)
{

};

void UpdateContactArg::build(const QString& link_path, QUrl& url)const
{
    QString s_link_path = link_path;
    s_link_path = link_path + QString("/%1").arg(m_contact_info.id());
    UrlBuilder b(s_link_path, url);
};

QString UpdateContactArg::toXml()const
{
    return m_contact_info.mergedXml(m_contact_info.originalXml());
};

QString UpdateContactArg::etag()const 
{
    QString rv = m_contact_info.etag();
    if (m_ignore_etag) {
        rv = "*";
    }
    return rv;
};

/**
BatchContactArg
*/
BatchContactArg::BatchContactArg()
{

};

BatchContactArg::BatchContactArg(const ContactList& c) :m_batch_list(c)
{

};

void BatchContactArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
};

QString BatchContactArg::toXml(QString userId)const
{
    return m_batch_list.toXmlFeed(userId);
};


/**
ContactsListArg
*/
ContactGroupListArg::ContactGroupListArg()
{

};

void ContactGroupListArg::build(const QString& link_path, QUrl& url)const {
    QString s_link_path = link_path;    
    if (!m_groupId.isEmpty()) {
        s_link_path = link_path + QString("/%1").arg(m_groupId);
    }
    
    UrlBuilder b(s_link_path, url);
    if (!m_alt.isEmpty()) {
        b.add("alt", m_alt);
    }
    if (!m_q.isEmpty()) {
        b.add("q", m_q);
    }
    if (m_max_results > 0) {
        b.add("max-results", m_max_results);
    }

    if (m_updated_min.isValid()) {
        b.add("updated-min", m_updated_min.toString(Qt::ISODate));
    }

    if (!m_orderby.isEmpty()) {
        b.add("orderby", m_orderby);
    }

    if (m_showdeleted) {
        b.add("showdeleted", "true");
    }

    if (m_requirealldeleted) {
        b.add("requirealldeleted", "true");
    }

    if (!m_sortorder.isEmpty()) {
        b.add("sortorder", m_sortorder);
    }
}

/**
    ContactGroupListResult
*/

ContactGroupListResult::ContactGroupListResult(const QByteArray& data)
{    
    m_data.reset(new GroupList);
    m_data->parseXml(data);
};

std::unique_ptr<ContactGroupListResult> ContactGroupListResult::factory::create(const QByteArray& data)
{
    std::unique_ptr<ContactGroupListResult> rv = std::unique_ptr<ContactGroupListResult>(new ContactGroupListResult(data));
    return rv;
};

GroupList* ContactGroupListResult::data()
{
    return m_data.get();
};


QString ContactGroupListResult::toString(bool /*multiline = true*/)const
{
    //return m_data->toString();
    return "";
};

/**
    CreateContactGroupArg
*/
CreateContactGroupArg::CreateContactGroupArg()
{

};

CreateContactGroupArg::CreateContactGroupArg(const GroupInfo& gi) :m_group_info(gi)
{

};

void CreateContactGroupArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
};

QString CreateContactGroupArg::toXml(QString userId)const
{
    return m_group_info.toXml(userId);
};


/**
UpdateContactGroupArg
*/
UpdateContactGroupArg::UpdateContactGroupArg()
{

};

UpdateContactGroupArg::UpdateContactGroupArg(const GroupInfo& g) :m_group_info(g)
{

};

void UpdateContactGroupArg::build(const QString& link_path, QUrl& url)const
{
    QString s_link_path = link_path;
    s_link_path = link_path + QString("/%1").arg(m_group_info.id());
    UrlBuilder b(s_link_path, url);
};

QString UpdateContactGroupArg::toXml()const
{
    return m_group_info.mergedXml(m_group_info.originalXml());
};

QString UpdateContactGroupArg::etag()const
{
    QString rv =  m_group_info.etag();
    if (m_ignore_etag) {
        rv = "*";
    }
    return rv;
};

DeleteContactGroupArg::DeleteContactGroupArg()
{

};

DeleteContactGroupArg::DeleteContactGroupArg(QString group_id, QString etag) :
    m_group_id(group_id), m_etag(etag)
{

};

void DeleteContactGroupArg::build(const QString& link_path, QUrl& url)const
{
    QString s_link_path = link_path;
    if (!m_group_id.isEmpty()) {
        s_link_path = link_path + QString("/%1").arg(m_group_id);
    }
    UrlBuilder b(s_link_path, url);
};

QString DeleteContactGroupArg::etag()const
{
    QString rv = m_etag;
    if (m_ignore_etag) {
        rv = "*";
    }
    return rv;
};

/**
BatchContactArg
*/
BatchContactGroupArg::BatchContactGroupArg()
{

};

BatchContactGroupArg::BatchContactGroupArg(const GroupList& c) :m_batch_list(c)
{

};

void BatchContactGroupArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
};

QString BatchContactGroupArg::toXml(QString userId)const
{
    return m_batch_list.toXmlFeed(userId);
};


/**
    DownloadPhotoArg
*/

DownloadPhotoArg::DownloadPhotoArg(QString contactId) :m_contactId(contactId)
{

};

void DownloadPhotoArg::build(const QString& link_path, QUrl& url)const 
{
    QString s_link_path = link_path;
    if (!m_contactId.isEmpty()) {
        s_link_path = link_path + QString("/%1").arg(m_contactId);
    }
    UrlBuilder b(s_link_path, url);
};

/**
UploadPhotoArg
*/

UploadPhotoArg::UploadPhotoArg(QString contactId) :m_contactId(contactId)
{

};

void UploadPhotoArg::build(const QString& link_path, QUrl& url)const
{
    QString s_link_path = link_path;
    if (!m_contactId.isEmpty()) {
        s_link_path = link_path + QString("/%1").arg(m_contactId);
    }
    UrlBuilder b(s_link_path, url);
};


//...
/**
DeletePhotoArg
*/
DeletePhotoArg::DeletePhotoArg()
{

};

DeletePhotoArg::DeletePhotoArg(QString contact_id, QString etag) :
    m_contact_id(contact_id), m_etag(etag)
{

};

void DeletePhotoArg::build(const QString& link_path, QUrl& url)const
{
    QString s_link_path = link_path;
    if (!m_contact_id.isEmpty()) {
        s_link_path = link_path + QString("/%1").arg(m_contact_id);
    }
    UrlBuilder b(s_link_path, url);
};

QString DeletePhotoArg::etag()const
{
    QString rv = m_etag;
    if (m_ignore_etag) {
        rv = "*";
    }
    return rv;
};
//..


#ifdef API_QT_AUTOTEST
std::unique_ptr<ContactsListArg> ContactsListArg::EXAMPLE(int, int)
{
    std::unique_ptr<ContactsListArg> rv(new ContactsListArg);
    return rv;
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
        //QString e_id = .;//QString("id%1").arg(i);
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

std::unique_ptr<ContactsListResult> ContactsListResult::EXAMPLE(int context_index, int parent_context_index)
{
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);

    IDSET idset = ApiAutotest::INSTANCE().getReservedIdSet("gcontact::ContactsListResult");
    if (idset.empty()) {
        idset.insert("c1id");
    }
    QByteArray d(XmlContactsResponseSample(idset).toStdString().c_str());
    std::unique_ptr<ContactsListResult> rv(new ContactsListResult(d));
    return rv;
};

std::unique_ptr<CreateContactArg> CreateContactArg::EXAMPLE(int context_index, int parent_content_index)
{
    std::unique_ptr<ContactInfo> ci = ContactInfo::EXAMPLE(context_index, parent_content_index);
    std::unique_ptr<CreateContactArg> rv(new CreateContactArg(*(ci.get())));
    return rv;
};

std::unique_ptr<DeleteContactArg> DeleteContactArg::EXAMPLE(int, int )
{
    std::unique_ptr<DeleteContactArg> rv(new DeleteContactArg("my-contact-id", "my-etag4contact"));
    return rv;
};

std::unique_ptr<UpdateContactArg> UpdateContactArg::EXAMPLE(int context_index, int parent_content_index)
{
    std::unique_ptr<ContactInfo> ci = ContactInfo::EXAMPLE(context_index, parent_content_index);
    std::unique_ptr<UpdateContactArg> rv(new UpdateContactArg(*(ci.get())));
    return rv;
};

std::unique_ptr<ContactGroupListArg> ContactGroupListArg::EXAMPLE(int, int)
{
    std::unique_ptr<ContactGroupListArg> rv(new ContactGroupListArg);
    return rv;
};

std::unique_ptr<ContactGroupListResult> ContactGroupListResult::EXAMPLE(int, int)
{
    IDSET idset = ApiAutotest::INSTANCE().getReservedIdSet("gcontact::ContactGroupListResult");
    if (idset.empty()) {
        idset.insert("g1id");
    }
    QByteArray d(XmlContactsResponseSample(idset).toStdString().c_str());
    std::unique_ptr<ContactGroupListResult> rv(new ContactGroupListResult(d));
    return rv;
};

std::unique_ptr<DownloadPhotoArg> DownloadPhotoArg::EXAMPLE(int, int)
{
    std::unique_ptr<DownloadPhotoArg> rv(new DownloadPhotoArg);
    return rv;
};

std::unique_ptr<UploadPhotoArg> UploadPhotoArg::EXAMPLE(int context_index, int parent_content_index)
{
    std::unique_ptr<UploadPhotoArg> rv(new UploadPhotoArg(QString("contact-id%1-%2").arg(context_index).arg(parent_content_index)));
    return rv;
};


std::unique_ptr<CreateContactGroupArg> CreateContactGroupArg::EXAMPLE(int context_index, int parent_content_index)
{
    std::unique_ptr<GroupInfo> gi = GroupInfo::EXAMPLE(context_index, parent_content_index);
    std::unique_ptr<CreateContactGroupArg> rv(new CreateContactGroupArg(*(gi.get())));
    return rv;
};

std::unique_ptr<UpdateContactGroupArg> UpdateContactGroupArg::EXAMPLE(int context_index, int parent_content_index)
{
    std::unique_ptr<GroupInfo> g = GroupInfo::EXAMPLE(context_index, parent_content_index);
    std::unique_ptr<UpdateContactGroupArg> rv(new UpdateContactGroupArg(*(g.get())));
    return rv;
};

std::unique_ptr<DeleteContactGroupArg> DeleteContactGroupArg::EXAMPLE(int, int)
{
    std::unique_ptr<DeleteContactGroupArg> rv(new DeleteContactGroupArg("my-group-id", "my-etag4group"));
    return rv;
};

std::unique_ptr<DeletePhotoArg> DeletePhotoArg::EXAMPLE(int, int)
{
    std::unique_ptr<DeletePhotoArg> rv(new DeletePhotoArg("my-contact-id", "my-etag4contact"));
    return rv;
};

std::unique_ptr<BatchContactArg> BatchContactArg::EXAMPLE(int, int)
{
    std::unique_ptr<BatchContactArg> rv(new BatchContactArg());
    return rv;
};

std::unique_ptr<BatchContactGroupArg> BatchContactGroupArg::EXAMPLE(int , int ) 
{
    std::unique_ptr<BatchContactGroupArg> rv(new BatchContactGroupArg());
    return rv;
};

#endif
