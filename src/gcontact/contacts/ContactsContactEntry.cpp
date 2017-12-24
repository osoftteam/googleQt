/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "contacts"
 www.prokarpaty.net
***********************************************************/

#include "gcontact/contacts/ContactsContactEntry.h"
using namespace googleQt;

namespace googleQt{

namespace contacts{
///ContactEntry

ContactEntry::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void ContactEntry::toJson(QJsonObject& js)const{

    if(!m_id.isEmpty())
        js["id"] = QString(m_id);
}

void ContactEntry::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
}

QString ContactEntry::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<ContactEntry>  ContactEntry::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<ContactEntry>  ContactEntry::factory::create(const QJsonObject& js)
{
    std::unique_ptr<ContactEntry> rv;
    rv = std::unique_ptr<ContactEntry>(new ContactEntry);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<ContactEntry> ContactEntry::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<ContactEntry> rv(new ContactEntry);
    rv->m_id = ApiAutotest::INSTANCE().getId("contacts::ContactEntry", example_idx);
    return rv;
}
#endif //API_QT_AUTOTEST

}//contacts
}//googleQt
