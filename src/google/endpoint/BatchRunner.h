#pragma once

#include <list>
#include <memory>
#include <iostream>
#include <QObject>
#include "GoogleTask.h"

namespace googleQt{

    template <class RESULT>
    using RESULT_LIST = std::vector<std::unique_ptr<RESULT>>;

    /**
        base class for concurrent runners, see ConcurrentValueRunner, ConcurrentArgRunner
    */
    template <class ARG_PARAM, class ROUTER>
    class ConcurrentBaseRunner : public EndpointRunnable
    {
    public:
        ConcurrentBaseRunner(const std::vector<ARG_PARAM>& arg_params, std::unique_ptr<ROUTER>&& r, ApiClient* cl)
            :EndpointRunnable(cl), m_router(std::move(r))
        {
            m_available_concurrent_routes_count = m_max_concurrent_routes_count;
            m_arg_parameters = arg_params;
        }

        bool isCompleted()const override { return m_completed; }
        const std::vector<ARG_PARAM>& completedArgList()const { return m_completed_arg_list; }

        void run()
        {
            m_steps2complete = static_cast<int>(m_arg_parameters.size());
            if(m_steps2complete < 1){
                m_completed = true;
                notifyOnFinished();                
            }
            
            while (m_arg_parameters.size() > 0 &&
                m_available_concurrent_routes_count > 0)
            {
                runSingleRequest();
            }
        }

    protected:

        void runSingleRequest()
        {
            if (m_steps2complete <= 0)
            {
                m_completed = true;
                notifyOnFinished();
            }

            if (m_arg_parameters.empty())
            {
                return;
            }

            //ARG_PARAM ap = m_arg_parameters.front();
            //m_arg_parameters.pop_front();
            ARG_PARAM ap = m_arg_parameters.back();
            m_arg_parameters.pop_back();
            m_available_concurrent_routes_count--;

            runArg(ap);
        }

        virtual void runArg(ARG_PARAM& ap) = 0;

        void afterSingleStepFinished()
        {
            m_available_concurrent_routes_count++;
            m_steps2complete--;
            runSingleRequest();
        }
    protected:
        std::unique_ptr<ROUTER>         m_router{ nullptr };
        std::vector<ARG_PARAM>            m_arg_parameters;
        std::vector<ARG_PARAM>            m_completed_arg_list;
        int  m_max_concurrent_routes_count{ 4 };///4 -> 2 servr might not like too many parallel requests, not sure..
        int  m_available_concurrent_routes_count{ 0 };
        int  m_steps2complete{ 0 };
        bool m_completed{ false };
    };

    /*
        The class has meaning close to Haskell 'map' - it will apply function routeRequest from ROUTER
        to every argument from list and produce list of results. The selection and processing
        requests happen concurrently with number of queries limited to 4 (no more then 4 queries at a time).
        (a->r) -> [a] -> [r]

        the execution of the list won't break in case of single request error but batch result will return only
        results from completed tasks
    */
    template <class ARG_PARAM, class ROUTER, class RESULT>
    class ConcurrentValueRunner : public ConcurrentBaseRunner<ARG_PARAM, ROUTER>
    {
    public:
        ConcurrentValueRunner(const std::vector<ARG_PARAM>& arg_params, std::unique_ptr<ROUTER>&& r, ApiClient* cl)
            :ConcurrentBaseRunner<ARG_PARAM, ROUTER>(arg_params, std::move(r), cl){}

        RESULT_LIST<RESULT>&& detachResult() { return std::move(m_result_list); }

        RESULT_LIST<RESULT>&& waitForResultAndRelease()
        {
            if (!EndpointRunnable::isFinished())
            {
                EndpointRunnable::m_in_wait_loop = true;
                EndpointRunnable::waitUntillFinishedOrCancelled();
            }
            EndpointRunnable::disposeLater();
            return std::move(m_result_list);
        }        

    protected:

        void runArg(ARG_PARAM& ap)override 
        {
            GoogleTask<RESULT>* t = ConcurrentBaseRunner<ARG_PARAM, ROUTER>::m_router->routeRequest(ap);
            EndpointRunnable::connect(t, &GoogleTask<RESULT>::finished, this, [=]()
            {
                if (t->isCompleted()) {
                    m_result_list.emplace_back(t->detachResult());
                    ConcurrentBaseRunner<ARG_PARAM, ROUTER>::m_completed_arg_list.push_back(ap);
                }
                ConcurrentBaseRunner<ARG_PARAM, ROUTER>::afterSingleStepFinished();
                t->disposeLater();
            });
        }

    protected:
        RESULT_LIST<RESULT>             m_result_list;
    };

    /**
        apply function routeRequest from ROUTER to every argument from arg list and produce list of completed args
        similiar to ConcurrentValueRunner but for tasks that don't bring back data only indication of successfull completion
        (a->()) -> [a] -> [a]

        completedArgList can be used to get list of completed parameters (usualy id)
    */
    template <class ARG_PARAM, class ROUTER>
    class ConcurrentArgRunner : public ConcurrentBaseRunner<ARG_PARAM, ROUTER>
    {
    public:
        ConcurrentArgRunner(const std::vector<ARG_PARAM>& arg_params, std::unique_ptr<ROUTER>&& r, ApiClient* cl)
            :ConcurrentBaseRunner<ARG_PARAM, ROUTER>(arg_params, std::move(r), cl) {}


        RESULT_LIST<ARG_PARAM>&& waitForResultAndRelease()
        {
            if (!EndpointRunnable::isFinished())
            {
                EndpointRunnable::m_in_wait_loop = true;
                EndpointRunnable::waitUntillFinishedOrCancelled();
            }
            EndpointRunnable::disposeLater();
            return std::move(ConcurrentBaseRunner<ARG_PARAM, ROUTER>::m_completed_arg_list);
        }

    protected:

        void runArg(ARG_PARAM& ap)override
        {
            GoogleVoidTask* t = ConcurrentBaseRunner<ARG_PARAM, ROUTER>::m_router->routeRequest(ap);
            EndpointRunnable::connect(t, &GoogleVoidTask::finished, this, [=]()
            {
                if (t->isCompleted()) {
                    ConcurrentBaseRunner<ARG_PARAM, ROUTER>::m_completed_arg_list.push_back(ap);
                }
                ConcurrentBaseRunner<ARG_PARAM, ROUTER>::afterSingleStepFinished();
                t->disposeLater();
            });
        }
    };//ConcurrentArgRunner
};
