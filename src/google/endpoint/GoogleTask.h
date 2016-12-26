#pragma once

#include <memory>
#include <functional>
#include <QObject>
#include "ApiException.h"

namespace googleQt{
    class ApiEndpoint;
    class Endpoint;

    class EndpointRunnable : public QObject
    {
        Q_OBJECT;
    public:
        EndpointRunnable(ApiEndpoint& ept) :m_endpoint(ept) {}
        bool isFinished()const { return m_finished; }

    signals:
        void finished();

    protected:
        void notifyOnFinished();
        void waitUntillFinishedOrCancelled();

    protected:
        ApiEndpoint& m_endpoint;
        bool m_finished{ false };
        mutable bool m_in_wait_loop{ false };
    };

    class GoogleBaseTask : public EndpointRunnable
    {
        friend class Endpoint;
    public:
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

        bool waitForResult()const;

    protected:
        GoogleBaseTask(ApiEndpoint& ept) :EndpointRunnable(ept) {};

        void failed_callback(std::unique_ptr<GoogleException> ex)
        {
            m_failed = std::move(ex);
            notifyOnFinished();
        };

    protected:
        std::unique_ptr<GoogleException> m_failed;
    };

    template <class RESULT>
    class GoogleTask : public GoogleBaseTask
    {
        friend class Endpoint;
    public:
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

        ///this function will block execution (via event loop) and return
        ///result object using move semantic via std::unique_ptr in case of success or 
        ///raise exception in case of error
        ///also this function will schedule dispose of the Task via deleteLater
        std::unique_ptr<RESULT> waitForResultAndRelease()
        {
            std::unique_ptr<RESULT> res;
            if (!isCompleted() && !isFailed())
            {
                m_in_wait_loop = true;
                waitUntillFinishedOrCancelled();                
            }

            if (isCompleted())
            {
                res = std::move(m_completed);
            }
            else if (isFailed())
            {
                std::unique_ptr<GoogleException> ex;
                ex = std::move(m_failed);
                deleteLater();
                if (ex)
                    ex->raise();
            }
            deleteLater();
            return res;
        };

    protected:
        GoogleTask(ApiEndpoint& ept) :GoogleBaseTask(ept) {};
        void completed_callback(std::unique_ptr<RESULT> r)
        {
            m_completed = std::move(r);
            notifyOnFinished();
        };
    protected:
        std::unique_ptr<RESULT> m_completed;
    };

    class GoogleVoidTask : public GoogleBaseTask
    {
        friend class Endpoint;
    public:
        virtual bool isCompleted()const override { return m_completed; };

        ///this function will block execution (via event loop) and return
        ///object in case os success or raise exception in case of error
        ///also this function will schedule dispose of the Task via deleteLater
        void waitForResultAndRelease();

    protected:
        GoogleVoidTask(ApiEndpoint& ept) :GoogleBaseTask(ept) {};
        void completed_callback(void)
        {
            m_completed = true;
            notifyOnFinished();
        };

    protected:
        bool m_completed = { false };
    };
};
