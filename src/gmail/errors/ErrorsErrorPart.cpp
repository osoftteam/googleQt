/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "errors"
 www.prokarpaty.net
***********************************************************/

#include "gmail/errors/ErrorsErrorPart.h"
using namespace googleQt;

namespace googleQt{

namespace errors{
///ErrorPart

ErrorPart::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void ErrorPart::toJson(QJsonObject& js)const{

    if(!m_domain.isEmpty())
        js["domain"] = QString(m_domain);
    if(!m_reason.isEmpty())
        js["reason"] = QString(m_reason);
    if(!m_message.isEmpty())
        js["message"] = QString(m_message);
}

void ErrorPart::fromJson(const QJsonObject& js){

    m_domain = js["domain"].toString();
    m_reason = js["reason"].toString();
    m_message = js["message"].toString();
}

QString ErrorPart::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<ErrorPart>  ErrorPart::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<ErrorPart>  ErrorPart::factory::create(const QJsonObject& js)
{
    std::unique_ptr<ErrorPart> rv;
    rv = std::unique_ptr<ErrorPart>(new ErrorPart);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<ErrorPart> ErrorPart::EXAMPLE(){
    std::unique_ptr<ErrorPart> rv(new ErrorPart);
    rv->m_domain = "test1value";
    rv->m_reason = "test2value";
    rv->m_message = "test3value";
    return rv;
}
#endif //API_QT_AUTOTEST

}//errors
}//googleQt
