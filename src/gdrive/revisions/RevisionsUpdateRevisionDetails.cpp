/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "revisions"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gdrive/revisions/RevisionsUpdateRevisionDetails.h"
using namespace googleQt;

namespace googleQt{

namespace revisions{
///UpdateRevisionDetails

UpdateRevisionDetails::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void UpdateRevisionDetails::toJson(QJsonObject& js)const{

    js["keepForever"] = m_keepForever;
    js["published"] = m_published;
    js["publishAuto"] = m_publishAuto;
    js["publishedOutsideDomain"] = m_publishedOutsideDomain;
}

void UpdateRevisionDetails::fromJson(const QJsonObject& js){

    m_keepForever = js["keepForever"].toVariant().toBool();
    m_published = js["published"].toVariant().toBool();
    m_publishAuto = js["publishAuto"].toVariant().toBool();
    m_publishedOutsideDomain = js["publishedOutsideDomain"].toVariant().toBool();
}

QString UpdateRevisionDetails::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<UpdateRevisionDetails>  UpdateRevisionDetails::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<UpdateRevisionDetails>  UpdateRevisionDetails::factory::create(const QJsonObject& js)
{
    std::unique_ptr<UpdateRevisionDetails> rv;
    rv = std::unique_ptr<UpdateRevisionDetails>(new UpdateRevisionDetails);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<UpdateRevisionDetails> UpdateRevisionDetails::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<UpdateRevisionDetails> rv(new UpdateRevisionDetails);
    return rv;
}
#endif //API_QT_AUTOTEST

}//revisions
}//googleQt
