/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "drafts"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include "gmail/drafts/DraftsDraftResource.h"

namespace googleQt{
namespace drafts{
    class DraftListRes{
        /**
            field: drafts: List of drafts.
            field: next_page_token: Token to retrieve the next page of results
                in the list.
            field: result_size_estimate: Estimated total number of results.
        */

    public:
        DraftListRes(){};

        DraftListRes(const std::list <DraftResource>& arg){ m_drafts = arg; };

    public:
            /**
                List of drafts.
            */
        const std::list <DraftResource>& drafts()const{return m_drafts;};
        DraftListRes& setDrafts(const std::list <DraftResource>& arg){m_drafts=arg;return *this;};

            /**
                Token to retrieve the next page of results in the list.
            */
        QString nextpagetoken()const{return m_nextPageToken;};
        DraftListRes& setNextpagetoken(const QString& arg){m_nextPageToken=arg;return *this;};

            /**
                Estimated total number of results.
            */
        quint64 resultsizeestimate()const{return m_resultSizeEstimate;};
        DraftListRes& setResultsizeestimate(const quint64& arg){m_resultSizeEstimate=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<DraftListRes>  create(const QByteArray& data);
            static std::unique_ptr<DraftListRes>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<DraftListRes> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                List of drafts.
            */
        std::list <DraftResource> m_drafts;

            /**
                Token to retrieve the next page of results in the list.
            */
        QString m_nextPageToken;

            /**
                Estimated total number of results.
            */
        quint64 m_resultSizeEstimate = {0};

    };//DraftListRes

}//drafts
}//googleQt
