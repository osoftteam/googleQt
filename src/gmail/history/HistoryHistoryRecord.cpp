/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "history"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/history/HistoryHistoryRecord.h"
using namespace googleQt;

namespace googleQt{

namespace history{
///HistoryRecord

HistoryRecord::operator QJsonObject()const{
    QJsonObject js;
    this->toJson(js);
    return js;
}


void HistoryRecord::toJson(QJsonObject& js)const{

    if(!m_id.isEmpty())
        js["id"] = QString(m_id);
    js["messages"] = struct_list2jsonarray(m_messages);
    js["messagesAdded"] = struct_list2jsonarray(m_messagesAdded);
    js["messagesDeleted"] = struct_list2jsonarray(m_messagesDeleted);
    js["labelsAdded"] = struct_list2jsonarray(m_labelsAdded);
    js["labelsRemoved"] = struct_list2jsonarray(m_labelsRemoved);
}

void HistoryRecord::fromJson(const QJsonObject& js){

    m_id = js["id"].toString();
    jsonarray2struct_list(js["messages"].toArray(), m_messages);
    jsonarray2struct_list(js["messagesAdded"].toArray(), m_messagesAdded);
    jsonarray2struct_list(js["messagesDeleted"].toArray(), m_messagesDeleted);
    jsonarray2struct_list(js["labelsAdded"].toArray(), m_labelsAdded);
    jsonarray2struct_list(js["labelsRemoved"].toArray(), m_labelsRemoved);
}

QString HistoryRecord::toString(bool multiline)const
{
    QJsonObject js;
    toJson(js);
    QJsonDocument doc(js);
    QString s(doc.toJson(multiline ? QJsonDocument::Indented : QJsonDocument::Compact));
    return s;
}


std::unique_ptr<HistoryRecord>  HistoryRecord::factory::create(const QByteArray& data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject js = doc.object();
    return create(js);
}


std::unique_ptr<HistoryRecord>  HistoryRecord::factory::create(const QJsonObject& js)
{
    std::unique_ptr<HistoryRecord> rv;
    rv = std::unique_ptr<HistoryRecord>(new HistoryRecord);
    rv->fromJson(js);
    return rv;
}

#ifdef API_QT_AUTOTEST
std::unique_ptr<HistoryRecord> HistoryRecord::EXAMPLE(int context_index, int parent_context_index){
    Q_UNUSED(context_index);
    Q_UNUSED(parent_context_index);
    static int example_idx = 0;
    example_idx++;
    std::unique_ptr<HistoryRecord> rv(new HistoryRecord);
    rv->m_id = ApiAutotest::INSTANCE().getId("history::HistoryRecord", example_idx);
    std::vector<messages::MessageResource> list_of_messages;
    for(int i = 0; i < 5; i++){
        messages::MessageResource p = *(messages::MessageResource::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("history::HistoryRecord", "messages::MessageResource", &p, i, context_index);
        rv->m_messages.push_back(p);
    }
    std::vector<messages::MessageResource> list_of_messagesAdded;
    for(int i = 0; i < 5; i++){
        messages::MessageResource p = *(messages::MessageResource::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("history::HistoryRecord", "messages::MessageResource", &p, i, context_index);
        rv->m_messagesAdded.push_back(p);
    }
    std::vector<messages::MessageResource> list_of_messagesDeleted;
    for(int i = 0; i < 5; i++){
        messages::MessageResource p = *(messages::MessageResource::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("history::HistoryRecord", "messages::MessageResource", &p, i, context_index);
        rv->m_messagesDeleted.push_back(p);
    }
    std::vector<history::LabelHistoryRecord> list_of_labelsAdded;
    for(int i = 0; i < 5; i++){
        history::LabelHistoryRecord p = *(history::LabelHistoryRecord::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("history::HistoryRecord", "history::LabelHistoryRecord", &p, i, context_index);
        rv->m_labelsAdded.push_back(p);
    }
    std::vector<history::LabelHistoryRecord> list_of_labelsRemoved;
    for(int i = 0; i < 5; i++){
        history::LabelHistoryRecord p = *(history::LabelHistoryRecord::EXAMPLE(i, context_index).get());
        ApiAutotest::INSTANCE().prepareAutoTestObj("history::HistoryRecord", "history::LabelHistoryRecord", &p, i, context_index);
        rv->m_labelsRemoved.push_back(p);
    }
    return rv;
}
#endif //API_QT_AUTOTEST

}//history
}//googleQt
