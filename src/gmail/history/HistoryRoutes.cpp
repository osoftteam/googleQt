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
    return m_end_point->getStyle<std::unique_ptr<HistoryRecordList>, HistoryRecordList::factory, NotAnException>(m_end_point->buildGmailUrl("history", arg));
}

