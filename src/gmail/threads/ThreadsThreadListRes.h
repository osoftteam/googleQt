/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "threads"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/threads/ThreadsThreadResource.h"

namespace googleQt{
namespace threads{
    class ThreadListRes{
        /**
            field: threads: List of threads.
            field: next_page_token: Token to retrieve the next page of results
                in the list.
            field: result_size_estimate: Estimated total number of results.
        */

    public:
        ThreadListRes(){};

        ThreadListRes(const std::list <ThreadResource>& arg){ m_threads = arg; };

    public:
            /**
                List of threads.
            */
        const std::list <ThreadResource>& threads()const{return m_threads;};
        ThreadListRes& setThreads(const std::list <ThreadResource>& arg){m_threads=arg;return *this;};

            /**
                Token to retrieve the next page of results in the list.
            */
        QString nextpagetoken()const{return m_nextPageToken;};
        ThreadListRes& setNextpagetoken(const QString& arg){m_nextPageToken=arg;return *this;};

            /**
                Estimated total number of results.
            */
        qint64 resultsizeestimate()const{return m_resultSizeEstimate;};
        ThreadListRes& setResultsizeestimate(const qint64& arg){m_resultSizeEstimate=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<ThreadListRes>  create(const QByteArray& data);
            static std::unique_ptr<ThreadListRes>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<ThreadListRes> EXAMPLE(int context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                List of threads.
            */
        std::list <ThreadResource> m_threads;

            /**
                Token to retrieve the next page of results in the list.
            */
        QString m_nextPageToken;

            /**
                Estimated total number of results.
            */
        qint64 m_resultSizeEstimate = {0};

    };//ThreadListRes

}//threads
}//googleQt
