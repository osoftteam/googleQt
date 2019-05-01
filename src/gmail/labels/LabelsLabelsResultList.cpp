/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "labels"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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
std::unique_ptr<LabelsResultList> LabelsResultList::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<LabelsResultList> rv(new LabelsResultList);
    std::vector<labels::LabelResource> list_of_labels;
    for(int i = 0; i < 5; i++){
        labels::LabelResource p = *(labels::LabelResource::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("labels::LabelsResultList", "labels::LabelResource", &p, i, context_index);
        rv->m_labels.push_back(p);
    }
    return rv;
}
#endif //API_QT_AUTOTEST

}//labels
}//googleQt
