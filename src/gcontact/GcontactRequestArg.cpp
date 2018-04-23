#include <QUrlQuery>
#include "GcontactRequestArg.h"

using namespace googleQt;
using namespace gcontact;

/**
   ContactListArg
*/
ContactListArg::ContactListArg(QString contactId /*= ""*/):m_contactId(contactId)
{

};

void ContactListArg::build(const QString& link_path, QUrl& url)const{
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
    return m_contact_info.mergedXml(m_contact_info.parsedXml());
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

BatchContactArg::BatchContactArg(const BatchRequesContactList& c) :m_batch_list(c)
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
ContactListArg
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
    return m_group_info.mergedXml(m_group_info.parsedXml());
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
BatchGroupArg::BatchGroupArg()
{

};

BatchGroupArg::BatchGroupArg(const BatchRequesGroupList& c) :m_batch_list(c)
{

};

void BatchGroupArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
};

QString BatchGroupArg::toXml(QString userId)const
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

#ifdef API_QT_AUTOTEST
std::unique_ptr<ContactListArg> ContactListArg::EXAMPLE(int, int)
{
    std::unique_ptr<ContactListArg> rv(new ContactListArg);
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

std::unique_ptr<BatchGroupArg> BatchGroupArg::EXAMPLE(int , int ) 
{
    std::unique_ptr<BatchGroupArg> rv(new BatchGroupArg());
    return rv;
};

#endif
