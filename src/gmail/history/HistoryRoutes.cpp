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
    GOOGLE_BLOCKING_CALL(list_Async, HistoryRecordList, arg);
}

void HistoryRoutes::list_Async(
    const gmail::HistoryListArg& arg,
    std::function<void(std::unique_ptr<HistoryRecordList>)> completed_callback ,
    std::function<void(std::unique_ptr<GoogleException>)> failed_callback)
{
    m_end_point->getStyle
        <
        std::unique_ptr<HistoryRecordList>,
        HistoryRecordList::factory
        >
        (m_end_point->buildGmailUrl("history", arg),
        completed_callback,
        failed_callback);
}

