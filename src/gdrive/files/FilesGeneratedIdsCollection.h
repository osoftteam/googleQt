/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "files"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"

namespace googleQt{
namespace files{
    class GeneratedIdsCollection{
        /**
            field: kind: This is always drive#generatedIds.
            field: space: The type of file that can be created with these IDs.
                Supported values are 'drive' and 'appDataFolder'.
            field: ids: The IDs generated for the requesting user in the
                specified space.
        */

    public:
        GeneratedIdsCollection():
        m_kind("drive#generatedIds")
        {};

        GeneratedIdsCollection(const QString& arg):
        m_kind("drive#generatedIds")
        { m_kind = arg; };
        virtual ~GeneratedIdsCollection(){};

    public:
            /**
                This is always drive#generatedIds.
            */
        QString kind()const{return m_kind;};
        GeneratedIdsCollection& setKind(const QString& arg){m_kind=arg;return *this;};

            /**
                The type of file that can be created with these IDs. Supported
                values are 'drive' and 'appDataFolder'.
            */
        QString space()const{return m_space;};
        GeneratedIdsCollection& setSpace(const QString& arg){m_space=arg;return *this;};

            /**
                The IDs generated for the requesting user in the specified
                space.
            */
        const std::list <QString>& ids()const{return m_ids;};
        GeneratedIdsCollection& setIds(const std::list <QString>& arg){m_ids=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<GeneratedIdsCollection>  create(const QByteArray& data);
            static std::unique_ptr<GeneratedIdsCollection>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<GeneratedIdsCollection> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                This is always drive#generatedIds.
            */
        QString m_kind;

            /**
                The type of file that can be created with these IDs. Supported
                values are 'drive' and 'appDataFolder'.
            */
        QString m_space;

            /**
                The IDs generated for the requesting user in the specified
                space.
            */
        std::list <QString> m_ids;

    };//GeneratedIdsCollection

}//files
}//googleQt
