#include "GoogleTask.h"
#include "ApiEndpoint.h"

using namespace googleQt;

///EndpointRunnable
void EndpointRunnable::notifyOnFinished()
{
    m_finished = true;
    emit finished();
    if (m_in_wait_loop)
    {
        m_endpoint.exitEventsLoop();
    }
};

void EndpointRunnable::waitUntillFinishedOrCancelled()
{
    m_endpoint.runEventsLoop();
};

bool EndpointRunnable::waitForResult()const
{
    if (!isCompleted() && !isFailed())
    {
        m_in_wait_loop = true;
        m_endpoint.runEventsLoop();
    }

    return isCompleted();
};

GoogleException* EndpointRunnable::error()
{
    GoogleException* rv = nullptr;
    if (m_failed)
    {
        rv = m_failed.get();
    }
    return rv;
};

void EndpointRunnable::failed_callback(std::unique_ptr<GoogleException> ex)
{
    m_failed = std::move(ex);
    notifyOnFinished();
};

/**
    GoogleVoidTask
*/
void GoogleVoidTask::waitForResultAndRelease()
{
    if (!isCompleted() && !isFailed())
    {
        m_in_wait_loop = true;
        m_endpoint.runEventsLoop();
    }

    if (isFailed())
    {
        std::unique_ptr<GoogleException> ex;
        ex = std::move(m_failed);
        deleteLater();
        if (ex)
            ex->raise();
    }
    deleteLater();
};

void GoogleVoidTask::then(std::function<void()> after_completed_processing, 
    std::function<void(std::unique_ptr<GoogleException>)> on_error)
{
    std::function<void(void)> on_finished_processing = [=]()
    {
        if (isCompleted()) {
            if (after_completed_processing) {
                after_completed_processing();
            }
        }
        else {
            if (isFailed() && on_error) {
                on_error(std::move(m_failed));
            }
        }
        deleteLater();
    };


    if (isFinished()) {
        on_finished_processing();
    }
    else {
        connect(this, &EndpointRunnable::finished,
            [=]()
        {
            on_finished_processing();
        });
    }
};

/**
    TaskAggregator
*/
bool TaskAggregator::isCompleted()const 
{
    for (auto t : m_runnables) {
        if (!t->isCompleted())
            return false;
    }
    return true;
};

bool TaskAggregator::isFailed()const
{
    for (auto t : m_runnables) {
        if (!t->isFailed())
            return true;
    }
    return false;
};

bool TaskAggregator::areAllFinished()const 
{
    for (auto t : m_runnables) {
        if (!t->isFinished())
            return false;
    }
    return true;
};

void TaskAggregator::then(std::function<void()> after_completed_processing /*= nullptr*/,
    std::function<void(std::unique_ptr<GoogleException>)> on_error /*= nullptr*/) 
{
    std::function<void(void)> on_finished_processing = [=]()
    {
        if (isCompleted()) {
            if (after_completed_processing) {
                after_completed_processing();
            }
        }
        else {
            if (isFailed() && on_error) {
                for (auto t : m_runnables) {
                    if (t->isFailed()) {
                        GoogleException* e = t->error();
                        if (e) {
                            m_failed.reset(e->clone());
                            break;
                        }
                    }
                }
                on_error(std::move(m_failed));
            }
        }
        deleteLater();
    };


    if (areAllFinished()) {
        on_finished_processing();
    }
    else {
        for (auto t : m_runnables) {
            connect(t, &EndpointRunnable::finished,
                [=]()
            {
                if (areAllFinished()) {
                    on_finished_processing();
                }
            });
        }
    }
};

