/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 www.prokarpaty.net
***********************************************************/

#include "gdrive/comments/CommentsCommentListResult.h"
using namespace googleQt;

namespace googleQt{

namespace comments{
///CommentListResult

CommentListResult::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void CommentListResult::toJson(QJsonObject& js)const{

    if(!m_nextPageToken.isEmpty())
        js["nextPageToken"] = QString(m_nextPageToken);
    if(!m_kind.isEmpty())
        js["kind"] = QString(m_kind);
    js["comments"] = struct_list2jsonarray(m_comments);
}

void CommentListResult::fromJson(const QJsonObject& js){

    m_nextPageToken = js["nextPageToken"].toString();
    m_kind = js["kind"].toString();
    jsonarray2struct_list(js["comments"].toArray(), m_comments);
}

QString CommentListResult::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<CommentListResult>  CommentListResult::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<CommentListResult>  CommentListResult::factory::create(const QJsonObject& js)
{
    std::unique_ptr<CommentListResult> rv;
    rv = std::unique_ptr<CommentListResult>(new CommentListResult);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<CommentListResult> CommentListResult::EXAMPLE(){
    std::unique_ptr<CommentListResult> rv(new CommentListResult);
    rv->m_nextPageToken = "test1value";
    rv->m_kind = "test2value";
    return rv;
}
#endif //API_QT_AUTOTEST

}//comments
}//googleQt
