/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasklists"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gtask/GtaskRequestArg.h"

namespace googleQt{
namespace tasklists{
    class TaskListResource{
        /**
            field: id: Task list identifier.
            field: title: Title of the task list.
            field: kind: Type of the resource. This is always tasks#taskList.
            field: etag: ETag of the resource.
            field: self_link: URL pointing to this task list. Used to retrieve,
                update, or delete this task list.
            field: updated: Last modification time of the task list (as a RFC
                3339 timestamp).
        */

    public:
        TaskListResource():
        m_kind("tasks#taskList")
        {};

        TaskListResource(const QString& arg):
        m_kind("tasks#taskList")
        { m_id = arg; };
        virtual ~TaskListResource(){};

    public:
            /**
                Task list identifier.
            */
        QString id()const{return m_id;};
        TaskListResource& setId(const QString& arg){m_id=arg;return *this;};

            /**
                Title of the task list.
            */
        QString title()const{return m_title;};
        TaskListResource& setTitle(const QString& arg){m_title=arg;return *this;};

            /**
                Type of the resource. This is always tasks#taskList.
            */
        QString kind()const{return m_kind;};
        TaskListResource& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                ETag of the resource.
            */
        QString etag()const{return m_etag;};
        TaskListResource& setEtag(const QString& arg){m_etag=arg;return *this;};

            /**
                URL pointing to this task list. Used to retrieve, update, or
                delete this task list.
            */
        QString selflink()const{return m_selfLink;};
        TaskListResource& setSelflink(const QString& arg){m_selfLink=arg;return *this;};

            /**
                Last modification time of the task list (as a RFC 3339
                timestamp).
            */
        QDateTime updated()const{return m_updated;};
        TaskListResource& setUpdated(const QDateTime& arg){m_updated=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<TaskListResource>  create(const QByteArray& data);
            static std::unique_ptr<TaskListResource>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<TaskListResource> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                Task list identifier.
            */
        QString m_id;

            /**
                Title of the task list.
            */
        QString m_title;

            /**
                Type of the resource. This is always tasks#taskList.
            */
        QString m_kind;

            /**
                ETag of the resource.
            */
        QString m_etag;

            /**
                URL pointing to this task list. Used to retrieve, update, or
                delete this task list.
            */
        QString m_selfLink;

            /**
                Last modification time of the task list (as a RFC 3339
                timestamp).
            */
        QDateTime m_updated;

    };//TaskListResource

}//tasklists
}//googleQt
