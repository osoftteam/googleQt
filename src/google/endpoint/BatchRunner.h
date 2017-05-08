#pragma once

#include <list>
#include <memory>
#include <iostream>
#include <QObject>
#include "GoogleTask.h"

namespace googleQt{

    template <class ARG_PARAM, class RESULT>
    using TASK_MAP = std::map<ARG_PARAM, std::unique_ptr<GoogleTask<RESULT>>>;
    template <class RESULT>
    using RESULT_LIST = std::list<RESULT>;

    template <class ARG_PARAM>
    using REQUEST_LIST = std::list<ARG_PARAM>;

    template <class ARG_PARAM, class RESULT>
    class UserBatchResult
    {
    public:
        void registerRequest(const ARG_PARAM& ap)
        {
            m_requests.push_back(ap);
        };
        void registerResult(const ARG_PARAM& ap, GoogleTask<RESULT>* t) 
        {
			///ykh: our result map becomes owner of Tasks via unique_ptr
			///not sure it's a good idea, see how UserBatchResult is used after
            m_results[ap] = std::unique_ptr<GoogleTask<RESULT>>(t);
        };

        RESULT_LIST<RESULT*> results()
        {
            RESULT_LIST<RESULT*> rv;
            for (auto& i : m_results) 
            {
                GoogleTask<RESULT>* t = i.second.get();
                if (t->isCompleted()) 
                {
                    rv.push_back(t->get());
                }
            }
            return rv;
        }

    protected:
        TASK_MAP<ARG_PARAM, RESULT> m_results;
        REQUEST_LIST<ARG_PARAM> m_requests;
    };


    template <class ARG_PARAM, class ROUTER, class RESULT>
    class UserBatchRunner: public EndpointRunnable
    {
    public:
        UserBatchRunner(const std::list<ARG_PARAM>& arg_params, ROUTER* r, ApiEndpoint& ept)
            :EndpointRunnable(ept)
        {
            m_available_concurrent_routes_count = m_max_concurrent_routes_count;
            m_arg_parameters = arg_params;
            m_router.reset(r);
        }

        void run()
        {
            if (!m_result)
            {
                m_result.reset(new UserBatchResult<ARG_PARAM, RESULT>);
            }

            m_steps2complete = static_cast<int>(m_arg_parameters.size());
            while(m_arg_parameters.size() > 0 && 
                m_available_concurrent_routes_count > 0)
                {
                    runSingleRequest();
                }
        }

        RESULT_LIST<RESULT*> get() { return m_result->results(); }
        std::unique_ptr<UserBatchResult<ARG_PARAM, RESULT> > waitForResultAndRelease()
        {
            std::unique_ptr<UserBatchResult<ARG_PARAM, RESULT> > res;

            if (!isFinished()) 
            {
                m_in_wait_loop = true;
                waitUntillFinishedOrCancelled();
            }

            res = std::move(m_result);

            deleteLater();
            return res;
        }

        bool isCompleted()const override { return m_completed; }

    protected:
        void runSingleRequest() 
        {
            if(m_steps2complete <= 0)
                {
                    m_completed = true;
                    notifyOnFinished();
                }
            
            if (m_arg_parameters.empty())
                {                
                    return;
                }
            
            ARG_PARAM ap = m_arg_parameters.front();
            m_arg_parameters.pop_front();
            m_result->registerRequest(ap);
            GoogleTask<RESULT>* t = m_router->routeSingleBatchRequest(ap);
            m_available_concurrent_routes_count--;
            
            connect(t, &GoogleTask<RESULT>::finished, this, [=]()
            {
				m_result->registerResult(ap, t);
				afterSingleStepFinished();
			});
        }

        void afterSingleStepFinished()
        {
            m_available_concurrent_routes_count++;
            m_steps2complete--;
            runSingleRequest();
        }

    protected:
        std::list<ARG_PARAM>            m_arg_parameters;
        std::unique_ptr<ROUTER>         m_router        {nullptr};
        std::unique_ptr<UserBatchResult<ARG_PARAM,RESULT> > m_result;
        int  m_max_concurrent_routes_count{ 4 };
        int  m_available_concurrent_routes_count{ 0 };
        int  m_steps2complete{ 0 };
        bool m_completed{false};
    };

};
