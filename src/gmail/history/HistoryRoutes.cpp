/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "history"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#include "gmail/history/HistoryRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace history;

std::unique_ptr<HistoryRecordList> HistoryRoutes::list(const gmail::HistoryListArg& arg){
    return list_Async(arg)->waitForResultAndRelease();
}

GoogleTask<HistoryRecordList>* HistoryRoutes::list_Async(const gmail::HistoryListArg& arg)
{
    GoogleTask<HistoryRecordList>* t = m_end_point->produceTask<HistoryRecordList>();
    m_end_point->getStyle<
        HistoryRecordList,
        HistoryRecordList::factory
        >
        (m_end_point->buildGmailUrl("history", arg),
        t);
    return t;
}

void HistoryRoutes::list_AsyncCB(
    const gmail::HistoryListArg& arg,
    std::function<void(std::unique_ptr<HistoryRecordList>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        HistoryRecordList,
        HistoryRecordList::factory
        >
        (m_end_point->buildGmailUrl("history", arg),
        completed_callback,
        failed_callback);
}

