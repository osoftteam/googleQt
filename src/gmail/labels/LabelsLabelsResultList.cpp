/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "labels"
 www.prokarpaty.net
***********************************************************/

#include "gmail/labels/LabelsLabelsResultList.h"
using namespace googleQt;

namespace googleQt{

namespace labels{
///LabelsResultList

LabelsResultList::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void LabelsResultList::toJson(QJsonObject& js)const{

    js["labels"] = struct_list2jsonarray(m_labels);
}

void LabelsResultList::fromJson(const QJsonObject& js){

    jsonarray2struct_list(js["labels"].toArray(), m_labels);
}

QString LabelsResultList::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<LabelsResultList>  LabelsResultList::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<LabelsResultList>  LabelsResultList::factory::create(const QJsonObject& js)
{
    std::unique_ptr<LabelsResultList> rv;
    rv = std::unique_ptr<LabelsResultList>(new LabelsResultList);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<LabelsResultList> LabelsResultList::EXAMPLE(){
    std::unique_ptr<LabelsResultList> rv(new LabelsResultList);
    return rv;
}
#endif //API_QT_AUTOTEST

}//labels
}//googleQt
