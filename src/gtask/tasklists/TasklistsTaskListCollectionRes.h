/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasklists"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gtask/GtaskRequestArg.h"
#include "gtask/tasklists/TasklistsTaskListResource.h"

namespace googleQt{
namespace tasklists{
    class TaskListCollectionRes{
        /**
            field: kind: Type of the resource. This is always tasks#taskLists.
            field: etag: ETag of the resource.
            field: next_page_token: Token that can be used to request the next
                page of this result.
            field: items: Collection of task lists.
        */

    public:
        TaskListCollectionRes():
        m_kind("tasks#taskLists")
        {};

        TaskListCollectionRes(const QString& arg):
        m_kind("tasks#taskLists")
        { m_kind = arg; };
        virtual ~TaskListCollectionRes(){};

    public:
            /**
                Type of the resource. This is always tasks#taskLists.
            */
        QString kind()const{return m_kind;};
        TaskListCollectionRes& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                ETag of the resource.
            */
        QString etag()const{return m_etag;};
        TaskListCollectionRes& setEtag(const QString& arg){m_etag=arg;return *this;};

            /**
                Token that can be used to request the next page of this result.
            */
        QString nextpagetoken()const{return m_nextPageToken;};
        TaskListCollectionRes& setNextpagetoken(const QString& arg){m_nextPageToken=arg;return *this;};

            /**
                Collection of task lists.
            */
        const std::vector<TaskListResource>& items()const{return m_items;};
        TaskListCollectionRes& setItems(const std::vector<TaskListResource>& arg){m_items=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<TaskListCollectionRes>  create(const QByteArray& data);
            static std::unique_ptr<TaskListCollectionRes>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<TaskListCollectionRes> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                Type of the resource. This is always tasks#taskLists.
            */
        QString m_kind;

            /**
                ETag of the resource.
            */
        QString m_etag;

            /**
                Token that can be used to request the next page of this result.
            */
        QString m_nextPageToken;

            /**
                Collection of task lists.
            */
        std::vector<TaskListResource> m_items;

    };//TaskListCollectionRes

}//tasklists
}//googleQt
