/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "history"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/history/HistoryHistoryRecord.h"

namespace googleQt{
namespace history{
    class HistoryRecordList{
        /**
            field: history: List of history records. Any messages contained in
                the response will typically only have id and threadId fields
                populated.
            field: next_page_token: Page token to retrieve the next page of
                results in the list.
            field: history_id: The ID of the mailbox's current history record.
        */

    public:
        HistoryRecordList(){};

        HistoryRecordList(const std::list <HistoryRecord>& arg){ m_history = arg; };

    public:
            /**
                List of history records. Any messages contained in the response
                will typically only have id and threadId fields populated.
            */
        const std::list <HistoryRecord>& history()const{return m_history;};
        HistoryRecordList& setHistory(const std::list <HistoryRecord>& arg){m_history=arg;return *this;};

            /**
                Page token to retrieve the next page of results in the list.
            */
        QString nextpagetoken()const{return m_nextPageToken;};
        HistoryRecordList& setNextpagetoken(const QString& arg){m_nextPageToken=arg;return *this;};

            /**
                The ID of the mailbox's current history record.
            */
        QString historyid()const{return m_historyId;};
        HistoryRecordList& setHistoryid(const QString& arg){m_historyId=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<HistoryRecordList>  create(const QByteArray& data);
            static std::unique_ptr<HistoryRecordList>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<HistoryRecordList> EXAMPLE(int context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                List of history records. Any messages contained in the response
                will typically only have id and threadId fields populated.
            */
        std::list <HistoryRecord> m_history;

            /**
                Page token to retrieve the next page of results in the list.
            */
        QString m_nextPageToken;

            /**
                The ID of the mailbox's current history record.
            */
        QString m_historyId;

    };//HistoryRecordList

}//history
}//googleQt
