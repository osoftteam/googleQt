/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/comments/CommentsReply.h"
using namespace googleQt;

namespace googleQt{

namespace comments{
///Reply

Reply::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void Reply::toJson(QJsonObject& js)const{

    if(!m_id.isEmpty())
        js["id"] = QString(m_id);
    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    if(m_createdTime.isValid())
        js["createdTime"] = m_createdTime.toString(Qt::ISODate);
    if(m_modifiedTime.isValid())
        js["modifiedTime"] = m_modifiedTime.toString(Qt::ISODate);
    js["author"] = (QJsonObject)m_author;
    if(!m_htmlContent.isEmpty())
        js["htmlContent"] = QString(m_htmlContent);
    if(!m_content.isEmpty())
        js["content"] = QString(m_content);
    js["deleted"] = m_deleted;
    if(!m_action.isEmpty())
        js["action"] = QString(m_action);
}

void Reply::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    m_kind = js["kind"].toString();
    m_createdTime = QDateTime::fromString(js["createdTime"].toString(), Qt::ISODate);
    m_modifiedTime = QDateTime::fromString(js["modifiedTime"].toString(), Qt::ISODate);
    m_author.fromJson(js["author"].toObject());
    m_htmlContent = js["htmlContent"].toString();
    m_content = js["content"].toString();
    m_deleted = js["deleted"].toVariant().toBool();
    m_action = js["action"].toString();
}

QString Reply::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<Reply>  Reply::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<Reply>  Reply::factory::create(const QJsonObject& js)
{
    std::unique_ptr<Reply> rv;
    rv = std::unique_ptr<Reply>(new Reply);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<Reply> Reply::EXAMPLE(){
    std::unique_ptr<Reply> rv(new Reply);
    rv->m_id = "test1value";
    rv->m_kind = "test2value";
    rv->m_createdTime = QDateTime::currentDateTime();
    rv->m_modifiedTime = QDateTime::currentDateTime();
    rv->m_author = *(comments::User::EXAMPLE().get());
    rv->m_htmlContent = "test6value";
    rv->m_content = "test7value";
    rv->m_action = "test9value";
    return rv;
}
#endif //API_QT_AUTOTEST

}//comments
}//googleQt
