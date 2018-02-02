/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasks"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gtask/GtaskRequestArg.h"
#include "gtask/tasks/TasksTaskResource.h"

namespace googleQt{
namespace tasks{
    class TaskCollectionRes{
        /**
            field: kind: Type of the resource. This is always tasks#tasks.
            field: etag: ETag of the resource.
            field: next_page_token: Token that can be used to request the next
                page of this result.
            field: items: Collection of task lists.
        */

    public:
        TaskCollectionRes():
        m_kind("tasks#tasks")
        {};

        TaskCollectionRes(const QString& arg):
        m_kind("tasks#tasks")
        { m_kind = arg; };
        virtual ~TaskCollectionRes(){};

    public:
            /**
                Type of the resource. This is always tasks#tasks.
            */
        QString kind()const{return m_kind;};
        TaskCollectionRes& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                ETag of the resource.
            */
        QString etag()const{return m_etag;};
        TaskCollectionRes& setEtag(const QString& arg){m_etag=arg;return *this;};

            /**
                Token that can be used to request the next page of this result.
            */
        QString nextpagetoken()const{return m_nextPageToken;};
        TaskCollectionRes& setNextpagetoken(const QString& arg){m_nextPageToken=arg;return *this;};

            /**
                Collection of task lists.
            */
        const std::list <TaskResource>& items()const{return m_items;};
        TaskCollectionRes& setItems(const std::list <TaskResource>& arg){m_items=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<TaskCollectionRes>  create(const QByteArray& data);
            static std::unique_ptr<TaskCollectionRes>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<TaskCollectionRes> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                Type of the resource. This is always tasks#tasks.
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
        std::list <TaskResource> m_items;

    };//TaskCollectionRes

}//tasks
}//googleQt
