/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "messages"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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
std::unique_ptr<BatchDeleteBodyArg> BatchDeleteBodyArg::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<BatchDeleteBodyArg> rv(new BatchDeleteBodyArg);
    std::vector<QString> list_of_ids;
    for(int i = 0; i < 5; i++){
        rv->m_ids.push_back(QString("id_%1").arg(i+1));
    }
    QString tmp_m_ids = ApiAutotest::INSTANCE().getString4List("messages::BatchDeleteBodyArg", "m_ids");
    if(!tmp_m_ids.isEmpty())rv->m_ids.push_back(tmp_m_ids);
    return rv;
}
#endif //API_QT_AUTOTEST

}//messages
}//googleQt
