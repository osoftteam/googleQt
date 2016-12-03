/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "history"
 www.prokarpaty.net
***********************************************************/

#pragma once

#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "GoogleRouteBase.h"
#include "gmail/history/HistoryHistoryRecordList.h"

namespace googleQt{
namespace history{


    class HistoryRoutes: public GoogleRouteBase{
    public:
        HistoryRoutes(Endpoint* ep):GoogleRouteBase(ep){};
        std::unique_ptr<HistoryRecordList> list(const gmail::HistoryListArg& arg);
        GoogleTask<HistoryRecordList>* list_Async(const gmail::HistoryListArg& arg);
        void list_AsyncCB(
            const gmail::HistoryListArg&,
            std::function<void(std::unique_ptr<HistoryRecordList>)> completed_callback = nullptr,
            std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr);

    protected:
    };//HistoryRoutes

}//history
}//googleQt
