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
std::unique_ptr<LabelModifyBodyArg> LabelModifyBodyArg::EXAMPLE(){
    std::unique_ptr<LabelModifyBodyArg> rv(new LabelModifyBodyArg);
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
