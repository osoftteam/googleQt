/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/comments/CommentsComment.h"
using namespace googleQt;

namespace googleQt{

namespace comments{
///Comment

Comment::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void Comment::toJson(QJsonObject& js)const{

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
    js["resolved"] = m_resolved;
    js["quotedFileContent"] = (QJsonObject)m_quotedFileContent;
    if(!m_anchor.isEmpty())
        js["anchor"] = QString(m_anchor);
    js["replies"] = struct_list2jsonarray(m_replies);
}

void Comment::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    m_kind = js["kind"].toString();
    m_createdTime = QDateTime::fromString(js["createdTime"].toString(), Qt::ISODate);
    m_modifiedTime = QDateTime::fromString(js["modifiedTime"].toString(), Qt::ISODate);
    m_author.fromJson(js["author"].toObject());
    m_htmlContent = js["htmlContent"].toString();
    m_content = js["content"].toString();
    m_deleted = js["deleted"].toVariant().toBool();
    m_resolved = js["resolved"].toVariant().toBool();
    m_quotedFileContent.fromJson(js["quotedFileContent"].toObject());
    m_anchor = js["anchor"].toString();
    jsonarray2struct_list(js["replies"].toArray(), m_replies);
}

QString Comment::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<Comment>  Comment::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<Comment>  Comment::factory::create(const QJsonObject& js)
{
    std::unique_ptr<Comment> rv;
    rv = std::unique_ptr<Comment>(new Comment);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<Comment> Comment::EXAMPLE(){
    std::unique_ptr<Comment> rv(new Comment);
    rv->m_id = "test1value";
    rv->m_kind = "test2value";
    rv->m_createdTime = QDateTime::currentDateTime();
    rv->m_modifiedTime = QDateTime::currentDateTime();
    rv->m_author = *(comments::User::EXAMPLE().get());
    rv->m_htmlContent = "test6value";
    rv->m_content = "test7value";
    rv->m_quotedFileContent = *(comments::QuotedFileContent::EXAMPLE().get());
    rv->m_anchor = "test11value";
    return rv;
}
#endif //API_QT_AUTOTEST

}//comments
}//googleQt
