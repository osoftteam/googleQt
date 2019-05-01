/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "comments"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
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
std::unique_ptr<CommentListResult> CommentListResult::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<CommentListResult> rv(new CommentListResult);
    rv->m_nextPageToken = ApiAutotest::INSTANCE().getString("comments::CommentListResult", "m_nextPageToken", QString("nextPageToken_%1").arg(example_idx));
    rv->m_kind = ApiAutotest::INSTANCE().getString("comments::CommentListResult", "m_kind", QString("kind_%1").arg(example_idx));
    std::vector<comments::Comment> list_of_comments;
    for(int i = 0; i < 5; i++){
        comments::Comment p = *(comments::Comment::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("comments::CommentListResult", "comments::Comment", &p, i, context_index);
        rv->m_comments.push_back(p);
    }
    return rv;
}
#endif //API_QT_AUTOTEST

}//comments
}//googleQt
