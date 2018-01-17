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

QString ContactsListResult::toString(bool /*multiline = true*/)const
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

QString UpdateContactArg::toXml(QString userId)const
{
    return m_contact_info.mergedXml(userId, m_contact_info.originalXml());
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

ContactGroupListResult::ContactGroupListResult(const QByteArray& )
{
    /*
    m_data.reset(new ContactList);
    m_is_null = !m_data->parseXml(data);
    */
};

std::unique_ptr<ContactGroupListResult> ContactGroupListResult::factory::create(const QByteArray& data)
{
    std::unique_ptr<ContactGroupListResult> rv = std::unique_ptr<ContactGroupListResult>(new ContactGroupListResult(data));
    return rv;
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

void CreateContactGroupArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
};

QString CreateContactGroupArg::toXml(QString userId)const
{
    //return m_contact_info.toXml(userId);
    return "";
};


//...
/**
UpdateContactGroupArg
*/
UpdateContactGroupArg::UpdateContactGroupArg()
{

};

void UpdateContactGroupArg::build(const QString& link_path, QUrl& url)const
{
    QString s_link_path = link_path;
   // s_link_path = link_path + QString("/%1").arg(m_contact_info.id());
    UrlBuilder b(s_link_path, url);
};

QString UpdateContactGroupArg::toXml(QString userId)const
{
    return "";//m_contact_info.mergedXml(userId, m_contact_info.originalXml());
};

QString UpdateContactGroupArg::etag()const
{
    QString rv = "";// m_contact_info.etag();
    if (m_ignore_etag) {
        rv = "*";
    }
    return rv;
};

DeleteContactGroupArg::DeleteContactGroupArg()
{

};

DeleteContactGroupArg::DeleteContactGroupArg(QString contact_id, QString etag) :
    m_contact_id(contact_id), m_etag(etag)
{

};

void DeleteContactGroupArg::build(const QString& link_path, QUrl& url)const
{
    QString s_link_path = link_path;
    if (!m_contact_id.isEmpty()) {
        s_link_path = link_path + QString("/%1").arg(m_contact_id);
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

//...

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



#ifdef API_QT_AUTOTEST
std::unique_ptr<ContactsListArg> ContactsListArg::EXAMPLE(int, int)
{
    std::unique_ptr<ContactsListArg> rv(new ContactsListArg);
    return rv;
};

static const char* xml_sample =
"<?xml version = \"1.0\" encoding = \"UTF-8\"?>\n"
"<feed gd:etag = \"&quot;qwertyfeeedvalue.&quot;\" xmlns = \"http://www.w3.org/2005/Atom\" xmlns:batch = \"http://schemas.google.com/gdata/batch\" xmlns:gContact = \"http://schemas.google.com/contact/2008\" xmlns:gd = \"http://schemas.google.com/g/2005\" xmlns:openSearch = \"http://a9.com/-/spec/opensearch/1.1/\">\n"
"<id>me@gmail.com</id>\n"
"<entry gd:etag = \"&quot;qwertytagvalue12344.&quot;\">\n"
"<id>http://www.google.com/m8/feeds/contacts/me%40gmail.com/base/asdfgh123</id>\n"
"<updated>2017 - 12 - 27T11:33:51.728Z</updated>\n"
"<app:edited xmlns:app = \"http://www.w3.org/2007/app\">2017-12-27T11:33:51.728Z</app:edited>\n"
"<category scheme = \"http://schemas.google.com/g/2005#kind\" term=\"http://schemas.google.com/contact/2008#contact\"/>\n"
"<title>test</title>\n"
"<content>testNotes</content>\n"
"<link rel = \"http://schemas.google.com/contacts/2008/rel#photo\" type=\"image/*\" href=\"https://www.googleapis.com/m8/feeds/photos/media/me%40gmail.com/asdfgh123\"/>\n"
"<link rel = \"self\" type=\"application/atom+xml\" href=\"https://www.googleapis.com/m8/feeds/contacts/me%40gmail.com/full/asdfgh123\"/>\n"
"<link rel = \"edit\" type=\"application/atom+xml\" href=\"https://www.googleapis.com/m8/feeds/contacts/me%40gmail.com/full/asdfgh123\"/>\n"
"<gd:name>\n"
"<gd:fullName>test</gd:fullName>\n"
"<gd:givenName>test</gd:givenName>\n"
"</gd:name>\n"
"<gd:organization rel = \"http://schemas.google.com/g/2005#other\">\n"
"<gd:orgName>testCompany</gd:orgName>\n"
"<gd:orgTitle>testJobTitle</gd:orgTitle>\n"
"</gd:organization>\n"
"<gd:email rel = \"http://schemas.google.com/g/2005#other\" address=\"testEmail@yahoo.com\"/>\n"
"<gd:phoneNumber rel = \"http://schemas.google.com/g/2005#home\">917 111-1111</gd:phoneNumber>\n"
"<gd:phoneNumber rel = \"http://schemas.google.com/g/2005#work\" uri=\"tel:+1-917-222-2222\">917 222-2222</gd:phoneNumber>\n"
"<gContact:groupMembershipInfo deleted = \"false\" href=\"http://www.google.com/m8/feeds/groups/me%40gmail.com/base/6\"/>\n"
"</entry>\n"
"</feed>\n";


std::unique_ptr<ContactsListResult> ContactsListResult::EXAMPLE(int , int ) 
{
    QByteArray d(xml_sample);
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
    QByteArray d(xml_sample);
    std::unique_ptr<ContactGroupListResult> rv(new ContactGroupListResult(d));
    return rv;
};

std::unique_ptr<DownloadPhotoArg> DownloadPhotoArg::EXAMPLE(int, int)
{
    std::unique_ptr<DownloadPhotoArg> rv(new DownloadPhotoArg);
    return rv;
};

std::unique_ptr<CreateContactGroupArg> CreateContactGroupArg::EXAMPLE(int , int )
{
    std::unique_ptr<CreateContactGroupArg> rv(new CreateContactGroupArg());
    return rv;
};

std::unique_ptr<UpdateContactGroupArg> UpdateContactGroupArg::EXAMPLE(int , int )
{
    std::unique_ptr<UpdateContactGroupArg> rv(new UpdateContactGroupArg());
    return rv;
};

std::unique_ptr<DeleteContactGroupArg> DeleteContactGroupArg::EXAMPLE(int, int)
{
    std::unique_ptr<DeleteContactGroupArg> rv(new DeleteContactGroupArg("my-contact-id", "my-etag4contact"));
    return rv;
};

#endif
