#include <QUrlQuery>
#include "GcontactRequestArg.h"

using namespace googleQt;
using namespace gcontact;

/**
   ContactsListArg
*/
ContactsListArg::ContactsListArg()
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

void CreateContactArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
};

QString CreateContactArg::toXmlString()const 
{
    return "";
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

std::unique_ptr<CreateContactArg> CreateContactArg::EXAMPLE(int , int )
{
    std::unique_ptr<CreateContactArg> rv(new CreateContactArg);
    return rv;
};


#endif
