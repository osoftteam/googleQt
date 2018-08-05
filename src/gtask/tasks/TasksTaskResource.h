/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasks"
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gtask/GtaskRequestArg.h"
#include "gtask/tasks/TasksTaskLink.h"

namespace googleQt{
namespace tasks{
    class TaskResource{
        /**
            field: id: Task identifier.
            field: title: Title of the task.
            field: kind: Type of the resource. This is always tasks#task
            field: etag: ETag of the resource.
            field: updated: Last modification time of the task (as a RFC 3339
                timestamp).
            field: self_link: URL pointing to this task. Used to retrieve,
                update, or delete this task.
            field: parent: Parent task identifier. This field is omitted if it
                is a top-level task. This field is read-only. Use the 'move'
                method to move the task under a different parent or to the top
                level.
            field: position: String indicating the position of the task among
                its sibling tasks under the same parent task or at the top
                level. If this string is greater than another task's
                corresponding position string according to lexicographical
                ordering, the task is positioned after the other task under the
                same parent task (or at the top level). This field is read-only.
                Use the 'move' method to move the task to another position.
            field: notes: Notes describing the task. Optional.
            field: status: Status of the task. This is either needsAction or
                completed.
            field: due: Due date of the task (as a RFC 3339 timestamp).
                Optional.
            field: completed: Completion date of the task (as a RFC 3339
                timestamp). This field is omitted if the task has not been
                completed.
            field: deleted: Flag indicating whether the task has been deleted.
                The default if False.
            field: hidden: Flag indicating whether the task is hidden. This is
                the case if the task had been marked completed when the task
                list was last cleared. The default is False. This field is
                read-only.
        */

    public:
        TaskResource():
        m_kind("tasks#task")
        ,m_deleted(false)
        ,m_hidden(false)
        {};

        TaskResource(const QString& arg):
        m_kind("tasks#task")
        ,m_deleted(false)
        ,m_hidden(false)
        { m_id = arg; };
        virtual ~TaskResource(){};

    public:
            /**
                Task identifier.
            */
        QString id()const{return m_id;};
        TaskResource& setId(const QString& arg){m_id=arg;return *this;};

            /**
                Title of the task.
            */
        QString title()const{return m_title;};
        TaskResource& setTitle(const QString& arg){m_title=arg;return *this;};

            /**
                Type of the resource. This is always tasks#task
            */
        QString kind()const{return m_kind;};
        TaskResource& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                ETag of the resource.
            */
        QString etag()const{return m_etag;};
        TaskResource& setEtag(const QString& arg){m_etag=arg;return *this;};

            /**
                Last modification time of the task (as a RFC 3339 timestamp).
            */
        QDateTime updated()const{return m_updated;};
        TaskResource& setUpdated(const QDateTime& arg){m_updated=arg;return *this;};

            /**
                URL pointing to this task. Used to retrieve, update, or delete
                this task.
            */
        QString selflink()const{return m_selfLink;};
        TaskResource& setSelflink(const QString& arg){m_selfLink=arg;return *this;};

            /**
                Parent task identifier. This field is omitted if it is a
                top-level task. This field is read-only. Use the 'move' method
                to move the task under a different parent or to the top level.
            */
        QString parent()const{return m_parent;};
        TaskResource& setParent(const QString& arg){m_parent=arg;return *this;};

            /**
                String indicating the position of the task among its sibling
                tasks under the same parent task or at the top level. If this
                string is greater than another task's corresponding position
                string according to lexicographical ordering, the task is
                positioned after the other task under the same parent task (or
                at the top level). This field is read-only. Use the 'move'
                method to move the task to another position.
            */
        QString position()const{return m_position;};
        TaskResource& setPosition(const QString& arg){m_position=arg;return *this;};

            /**
                Notes describing the task. Optional.
            */
        QString notes()const{return m_notes;};
        TaskResource& setNotes(const QString& arg){m_notes=arg;return *this;};

            /**
                Status of the task. This is either needsAction or completed.
            */
        QString status()const{return m_status;};
        TaskResource& setStatus(const QString& arg){m_status=arg;return *this;};

            /**
                Due date of the task (as a RFC 3339 timestamp). Optional.
            */
        QDateTime due()const{return m_due;};
        TaskResource& setDue(const QDateTime& arg){m_due=arg;return *this;};

            /**
                Completion date of the task (as a RFC 3339 timestamp). This
                field is omitted if the task has not been completed.
            */
        QDateTime completed()const{return m_completed;};
        TaskResource& setCompleted(const QDateTime& arg){m_completed=arg;return *this;};

            /**
                Flag indicating whether the task has been deleted. The default
                if False.
            */
        bool deleted()const{return m_deleted;};
        TaskResource& setDeleted(const bool& arg){m_deleted=arg;return *this;};

            /**
                Flag indicating whether the task is hidden. This is the case if
                the task had been marked completed when the task list was last
                cleared. The default is False. This field is read-only.
            */
        bool hidden()const{return m_hidden;};
        TaskResource& setHidden(const bool& arg){m_hidden=arg;return *this;};

        const std::list <TaskLink>& links()const{return m_links;};
        TaskResource& setLinks(const std::list <TaskLink>& arg){m_links=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<TaskResource>  create(const QByteArray& data);
            static std::unique_ptr<TaskResource>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<TaskResource> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                Task identifier.
            */
        QString m_id;

            /**
                Title of the task.
            */
        QString m_title;

            /**
                Type of the resource. This is always tasks#task
            */
        QString m_kind;

            /**
                ETag of the resource.
            */
        QString m_etag;

            /**
                Last modification time of the task (as a RFC 3339 timestamp).
            */
        QDateTime m_updated;

            /**
                URL pointing to this task. Used to retrieve, update, or delete
                this task.
            */
        QString m_selfLink;

            /**
                Parent task identifier. This field is omitted if it is a
                top-level task. This field is read-only. Use the 'move' method
                to move the task under a different parent or to the top level.
            */
        QString m_parent;

            /**
                String indicating the position of the task among its sibling
                tasks under the same parent task or at the top level. If this
                string is greater than another task's corresponding position
                string according to lexicographical ordering, the task is
                positioned after the other task under the same parent task (or
                at the top level). This field is read-only. Use the 'move'
                method to move the task to another position.
            */
        QString m_position;

            /**
                Notes describing the task. Optional.
            */
        QString m_notes;

            /**
                Status of the task. This is either needsAction or completed.
            */
        QString m_status;

            /**
                Due date of the task (as a RFC 3339 timestamp). Optional.
            */
        QDateTime m_due;

            /**
                Completion date of the task (as a RFC 3339 timestamp). This
                field is omitted if the task has not been completed.
            */
        QDateTime m_completed;

            /**
                Flag indicating whether the task has been deleted. The default
                if False.
            */
        bool m_deleted;

            /**
                Flag indicating whether the task is hidden. This is the case if
                the task had been marked completed when the task list was last
                cleared. The default is False. This field is read-only.
            */
        bool m_hidden;

        std::list <TaskLink> m_links;

    };//TaskResource

}//tasks
}//googleQt
