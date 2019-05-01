/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "errors"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/errors/ErrorsErrorInfo.h"
using namespace googleQt;

namespace googleQt{

namespace errors{
///ErrorInfo

ErrorInfo::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void ErrorInfo::toJson(QJsonObject& js)const{

    js["errors"] = struct_list2jsonarray(m_errors);
    js["code"] = QString("%1").arg(m_code);
    if(!m_message.isEmpty())
        js["message"] = QString(m_message);
}

void ErrorInfo::fromJson(const QJsonObject& js){

    jsonarray2struct_list(js["errors"].toArray(), m_errors);
    m_code = js["code"].toVariant().toString().toULongLong();
    m_message = js["message"].toString();
}

QString ErrorInfo::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<ErrorInfo>  ErrorInfo::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<ErrorInfo>  ErrorInfo::factory::create(const QJsonObject& js)
{
    std::unique_ptr<ErrorInfo> rv;
    rv = std::unique_ptr<ErrorInfo>(new ErrorInfo);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<ErrorInfo> ErrorInfo::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<ErrorInfo> rv(new ErrorInfo);
    std::vector<errors::ErrorPart> list_of_errors;
    for(int i = 0; i < 5; i++){
        errors::ErrorPart p = *(errors::ErrorPart::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("errors::ErrorInfo", "errors::ErrorPart", &p, i, context_index);
        rv->m_errors.push_back(p);
    }
    rv->m_code = ApiAutotest::INSTANCE().getInt("errors::ErrorInfo", "m_code", 2 + example_idx);
    rv->m_message = ApiAutotest::INSTANCE().getString("errors::ErrorInfo", "m_message", QString("message_%1").arg(example_idx));
    return rv;
}
#endif //API_QT_AUTOTEST

}//errors
}//googleQt
