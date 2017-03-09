/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#include "gmail/messages/MessagesLabelModifyBodyArg.h"
using namespace googleQt;

namespace googleQt{

namespace messages{
///LabelModifyBodyArg

LabelModifyBodyArg::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void LabelModifyBodyArg::toJson(QJsonObject& js)const{

    js["addLabels"] = ingrl_list2jsonarray(m_addLabels);
    js["removeLabels"] = ingrl_list2jsonarray(m_removeLabels);
}

void LabelModifyBodyArg::fromJson(const QJsonObject& js){

    jsonarray2ingrl_list(js["addLabels"].toArray(), m_addLabels);
    jsonarray2ingrl_list(js["removeLabels"].toArray(), m_removeLabels);
}

QString LabelModifyBodyArg::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<LabelModifyBodyArg>  LabelModifyBodyArg::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<LabelModifyBodyArg>  LabelModifyBodyArg::factory::create(const QJsonObject& js)
{
    std::unique_ptr<LabelModifyBodyArg> rv;
    rv = std::unique_ptr<LabelModifyBodyArg>(new LabelModifyBodyArg);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<LabelModifyBodyArg> LabelModifyBodyArg::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<LabelModifyBodyArg> rv(new LabelModifyBodyArg);
    std::list<QString> list_of_addLabels;
    for(int i = 0; i < 3; i++){
        rv->m_addLabels.push_back(QString("_%1_%2").arg(i).arg(example_idx));
    }
    std::list<QString> list_of_removeLabels;
    for(int i = 0; i < 3; i++){
        rv->m_removeLabels.push_back(QString("_%1_%2").arg(i).arg(example_idx));
    }
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
