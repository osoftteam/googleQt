/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "revisions"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include "gdrive/revisions/RevisionsRevisionResource.h"

namespace googleQt{
namespace revisions{
    class RevisionResourceCollection{
        /**
            field: next_page_token: The page token for the next page of
                revisions. This will be absent if the end of the revisions list
                has been reached. If the token is rejected for any reason, it
                should be discarded, and pagination should be restarted from the
                first page of results.
            field: kind: This is always drive#revisionList.
            field: files: The list of revisions. If nextPageToken is populated,
                then this list may be incomplete and an additional page of
                results should be fetched.
        */

    public:
        RevisionResourceCollection():
        m_kind("drive#revisionList")
        {};

        RevisionResourceCollection(const QString& arg):
        m_kind("drive#revisionList")
        { m_nextPageToken = arg; };
        virtual ~RevisionResourceCollection(){};

    public:
            /**
                The page token for the next page of revisions. This will be
                absent if the end of the revisions list has been reached. If the
                token is rejected for any reason, it should be discarded, and
                pagination should be restarted from the first page of results.
            */
        QString nextpagetoken()const{return m_nextPageToken;};
        RevisionResourceCollection& setNextpagetoken(const QString& arg){m_nextPageToken=arg;return *this;};

            /**
                This is always drive#revisionList.
            */
        QString kind()const{return m_kind;};
        RevisionResourceCollection& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                The list of revisions. If nextPageToken is populated, then this
                list may be incomplete and an additional page of results should
                be fetched.
            */
        const std::list <RevisionResource>& files()const{return m_files;};
        RevisionResourceCollection& setFiles(const std::list <RevisionResource>& arg){m_files=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<RevisionResourceCollection>  create(const QByteArray& data);
            static std::unique_ptr<RevisionResourceCollection>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<RevisionResourceCollection> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                The page token for the next page of revisions. This will be
                absent if the end of the revisions list has been reached. If the
                token is rejected for any reason, it should be discarded, and
                pagination should be restarted from the first page of results.
            */
        QString m_nextPageToken;

            /**
                This is always drive#revisionList.
            */
        QString m_kind;

            /**
                The list of revisions. If nextPageToken is populated, then this
                list may be incomplete and an additional page of results should
                be fetched.
            */
        std::list <RevisionResource> m_files;

    };//RevisionResourceCollection

}//revisions
}//googleQt
