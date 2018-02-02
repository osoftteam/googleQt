/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "tasks"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gtask/GtaskRequestArg.h"

namespace googleQt{
namespace tasks{
    class TaskLink{
        /**
            field: type: Type of the link, e.g. email.
            field: description: The description. In HTML speak: Everything
                between <a> and </a>.
            field: link: The URL.
        */

    public:
        TaskLink(){};

        TaskLink(const QString& arg){ m_type = arg; };
        virtual ~TaskLink(){};

    public:
            /**
                Type of the link, e.g. email.
            */
        QString type()const{return m_type;};
        TaskLink& setType(const QString& arg){m_type=arg;return *this;};

            /**
                The description. In HTML speak: Everything between <a> and </a>.
            */
        QString description()const{return m_description;};
        TaskLink& setDescription(const QString& arg){m_description=arg;return *this;};

            /**
                The URL.
            */
        QString link()const{return m_link;};
        TaskLink& setLink(const QString& arg){m_link=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<TaskLink>  create(const QByteArray& data);
            static std::unique_ptr<TaskLink>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<TaskLink> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                Type of the link, e.g. email.
            */
        QString m_type;

            /**
                The description. In HTML speak: Everything between <a> and </a>.
            */
        QString m_description;

            /**
                The URL.
            */
        QString m_link;

    };//TaskLink

}//tasks
}//googleQt
