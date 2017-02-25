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
