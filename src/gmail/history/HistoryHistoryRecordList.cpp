/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "history"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/history/HistoryHistoryRecordList.h"
using namespace googleQt;

namespace googleQt{

namespace history{
///HistoryRecordList

HistoryRecordList::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void HistoryRecordList::toJson(QJsonObject& js)const{

    js["history"] = struct_list2jsonarray(m_history);
    if(!m_nextPageToken.isEmpty())
        js["nextPageToken"] = QString(m_nextPageToken);
    if(!m_historyId.isEmpty())
        js["historyId"] = QString(m_historyId);
}

void HistoryRecordList::fromJson(const QJsonObject& js){

    jsonarray2struct_list(js["history"].toArray(), m_history);
    m_nextPageToken = js["nextPageToken"].toString();
    m_historyId = js["historyId"].toString();
}

QString HistoryRecordList::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<HistoryRecordList>  HistoryRecordList::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<HistoryRecordList>  HistoryRecordList::factory::create(const QJsonObject& js)
{
    std::unique_ptr<HistoryRecordList> rv;
    rv = std::unique_ptr<HistoryRecordList>(new HistoryRecordList);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<HistoryRecordList> HistoryRecordList::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<HistoryRecordList> rv(new HistoryRecordList);
    std::vector<history::HistoryRecord> list_of_history;
    for(int i = 0; i < 5; i++){
        history::HistoryRecord p = *(history::HistoryRecord::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("history::HistoryRecordList", "history::HistoryRecord", &p, i, context_index);
        rv->m_history.push_back(p);
    }
    rv->m_nextPageToken = ApiAutotest::INSTANCE().getString("history::HistoryRecordList", "m_nextPageToken", QString("nextPageToken_%1").arg(example_idx));
    rv->m_historyId = ApiAutotest::INSTANCE().getString("history::HistoryRecordList", "m_historyId", QString("historyId_%1").arg(example_idx));
    return rv;
}
#endif //API_QT_AUTOTEST

}//history
}//googleQt
