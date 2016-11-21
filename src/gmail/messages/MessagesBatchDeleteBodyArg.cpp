/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 www.prokarpaty.net
***********************************************************/

#include "gmail/messages/MessagesBatchDeleteBodyArg.h"
using namespace googleQt;

namespace googleQt{

namespace messages{
///BatchDeleteBodyArg

BatchDeleteBodyArg::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void BatchDeleteBodyArg::toJson(QJsonObject& js)const{

    js["ids"] = ingrl_list2jsonarray(m_ids);
}

void BatchDeleteBodyArg::fromJson(const QJsonObject& js){

    jsonarray2ingrl_list(js["ids"].toArray(), m_ids);
}

QString BatchDeleteBodyArg::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<BatchDeleteBodyArg>  BatchDeleteBodyArg::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<BatchDeleteBodyArg>  BatchDeleteBodyArg::factory::create(const QJsonObject& js)
{
    std::unique_ptr<BatchDeleteBodyArg> rv;
    rv = std::unique_ptr<BatchDeleteBodyArg>(new BatchDeleteBodyArg);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<BatchDeleteBodyArg> BatchDeleteBodyArg::EXAMPLE(){
    std::unique_ptr<BatchDeleteBodyArg> rv(new BatchDeleteBodyArg);
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
