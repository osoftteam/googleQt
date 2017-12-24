/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "contacts"
 www.prokarpaty.net
***********************************************************/

#include "gcontact/contacts/ContactsContactCollectionRes.h"
using namespace googleQt;

namespace googleQt{

namespace contacts{
///ContactCollectionRes

ContactCollectionRes::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void ContactCollectionRes::toJson(QJsonObject& js)const{

    if(!m_version.isEmpty())
        js["version"] = QString(m_version);
    js["entries"] = struct_list2jsonarray(m_entries);
}

void ContactCollectionRes::fromJson(const QJsonObject& js){

    m_version = js["version"].toString();
    jsonarray2struct_list(js["entries"].toArray(), m_entries);
}

QString ContactCollectionRes::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<ContactCollectionRes>  ContactCollectionRes::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<ContactCollectionRes>  ContactCollectionRes::factory::create(const QJsonObject& js)
{
    std::unique_ptr<ContactCollectionRes> rv;
    rv = std::unique_ptr<ContactCollectionRes>(new ContactCollectionRes);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<ContactCollectionRes> ContactCollectionRes::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<ContactCollectionRes> rv(new ContactCollectionRes);
    rv->m_version = ApiAutotest::INSTANCE().getString("contacts::ContactCollectionRes", "m_version", QString("version_%1").arg(example_idx));
    std::list<contacts::ContactEntry> list_of_entries;
    for(int i = 0; i < 5; i++){
        contacts::ContactEntry p = *(contacts::ContactEntry::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("contacts::ContactCollectionRes", "contacts::ContactEntry", &p, i, context_index);
        rv->m_entries.push_back(p);
    }
    return rv;
}
#endif //API_QT_AUTOTEST

}//contacts
}//googleQt
