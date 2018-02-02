/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification "revisions"
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"

namespace googleQt{
namespace revisions{
    class UpdateRevisionDetails{
        /**
            field: keep_forever: Whether to keep this revision forever, even if
                it is no longer the head revision. If not set, the revision will
                be automatically purged 30 days after newer content is uploaded.
                This can be set on a maximum of 200 revisions for a file. This
                field is only applicable to files with binary content in Drive.
            field: published: Whether this revision is published. This is only
                applicable to Google Docs.
            field: publish_auto: Whether subsequent revisions will be
                automatically republished. This is only applicable to Google
                Docs.
            field: published_outside_domain: Whether this revision is published
                outside the domain. This is only applicable to Google Docs.
        */

    public:
        UpdateRevisionDetails(){};

        UpdateRevisionDetails(const bool& arg){ m_keepForever = arg; };
        virtual ~UpdateRevisionDetails(){};

    public:
            /**
                Whether to keep this revision forever, even if it is no longer
                the head revision. If not set, the revision will be
                automatically purged 30 days after newer content is uploaded.
                This can be set on a maximum of 200 revisions for a file. This
                field is only applicable to files with binary content in Drive.
            */
        bool keepforever()const{return m_keepForever;};
        UpdateRevisionDetails& setKeepforever(const bool& arg){m_keepForever=arg;return *this;};

            /**
                Whether this revision is published. This is only applicable to
                Google Docs.
            */
        bool published()const{return m_published;};
        UpdateRevisionDetails& setPublished(const bool& arg){m_published=arg;return *this;};

            /**
                Whether subsequent revisions will be automatically republished.
                This is only applicable to Google Docs.
            */
        bool publishauto()const{return m_publishAuto;};
        UpdateRevisionDetails& setPublishauto(const bool& arg){m_publishAuto=arg;return *this;};

            /**
                Whether this revision is published outside the domain. This is
                only applicable to Google Docs.
            */
        bool publishedoutsidedomain()const{return m_publishedOutsideDomain;};
        UpdateRevisionDetails& setPublishedoutsidedomain(const bool& arg){m_publishedOutsideDomain=arg;return *this;};

    public:
        operator QJsonObject ()const;
        virtual void fromJson(const QJsonObject& js);
        virtual void toJson(QJsonObject& js)const;
        virtual QString toString(bool multiline = true)const;


        class factory{
        public:
            static std::unique_ptr<UpdateRevisionDetails>  create(const QByteArray& data);
            static std::unique_ptr<UpdateRevisionDetails>  create(const QJsonObject& js);
        };


        #ifdef API_QT_AUTOTEST
        static std::unique_ptr<UpdateRevisionDetails> EXAMPLE(int context_index, int parent_context_index);
        #endif //API_QT_AUTOTEST


    protected:
            /**
                Whether to keep this revision forever, even if it is no longer
                the head revision. If not set, the revision will be
                automatically purged 30 days after newer content is uploaded.
                This can be set on a maximum of 200 revisions for a file. This
                field is only applicable to files with binary content in Drive.
            */
        bool m_keepForever;

            /**
                Whether this revision is published. This is only applicable to
                Google Docs.
            */
        bool m_published;

            /**
                Whether subsequent revisions will be automatically republished.
                This is only applicable to Google Docs.
            */
        bool m_publishAuto;

            /**
                Whether this revision is published outside the domain. This is
                only applicable to Google Docs.
            */
        bool m_publishedOutsideDomain;

    };//UpdateRevisionDetails

}//revisions
}//googleQt
