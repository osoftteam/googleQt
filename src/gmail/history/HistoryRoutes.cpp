/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "history"
 www.prokarpaty.net
***********************************************************/

#include "gmail/history/HistoryRoutes.h"
#include "Endpoint.h"
#include "gmail/GmailRoutes.h"
using namespace googleQt;
using namespace history;

std::unique_ptr<HistoryRecordList> HistoryRoutes::list(const gmail::HistoryListArg& arg){
    GOOGLE_BLOCKING_CALL(list_AsyncCB, HistoryRecordList, arg);
}

GoogleTask<HistoryRecordList>* HistoryRoutes::list_Async(const gmail::HistoryListArg& arg)
{
    GoogleTask<HistoryRecordList>* t = new GoogleTask<HistoryRecordList>();
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

