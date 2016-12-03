#pragma once

#include <memory>
#include <functional>
#include <QObject>
#include "ApiException.h"

namespace googleQt{
    class GoogleBaseTask : public QObject
    {
        Q_OBJECT;
        friend class Endpoint;
    public:
        GoogleBaseTask() {};
        virtual ~GoogleBaseTask() {};

        virtual bool isCompleted()const = 0;
        virtual bool isFailed()const { return (m_failed != nullptr); };

        GoogleException* error()
        {
            GoogleException* rv = nullptr;
            if (m_failed)
                {
                    rv = m_failed.get();
                }
            return rv;
        };

    signals:
        void completed();
        void failed();
    protected:
        void failed_callback(std::unique_ptr<GoogleException> ex)
        {
            m_failed = std::move(ex);
            emit failed();
        };
    protected:
        std::unique_ptr<GoogleException> m_failed;
    };

    template <class RESULT>
    class GoogleTask : public GoogleBaseTask
    {
        friend class Endpoint;
    public:
        GoogleTask() {};
        virtual ~GoogleTask() {};

        RESULT* get()
        {
            RESULT* rv = nullptr;
            if (m_completed)
                {
                    rv = m_completed.get();
                }
            return rv;
        }

        virtual bool isCompleted()const override { return (m_completed != nullptr); };

    protected:
        void completed_callback(std::unique_ptr<RESULT> r)
        {
            m_completed = std::move(r);
            emit completed();
        };
    protected:
        std::unique_ptr<RESULT> m_completed;
    };

    class GoogleVoidTask : public GoogleBaseTask
    {
        friend class Endpoint;
    public:
        GoogleVoidTask() {};
        virtual ~GoogleVoidTask() {};

        virtual bool isCompleted()const override { return m_completed; };

    protected:
        void completed_callback(void)
        {
            m_completed = true;
            emit completed();
        };
    protected:
        bool m_completed = { false };
    };
};
