#include "GoogleTask.h"
#include "ApiEndpoint.h"

using namespace googleQt;

///GoogleBaseTask
bool GoogleBaseTask::waitForResult()const
{
    if (!isCompleted() && !isFailed())
    {
        m_in_wait_loop = true;
        m_endpoint.runEventsLoop();
    }

    return isCompleted();
};

void GoogleBaseTask::notifyOnFinished()
{
    emit finished();
    if (m_in_wait_loop)
    {
        m_endpoint.exitEventsLoop();
    }
};

void GoogleBaseTask::waitUntillFinishedOrCancelled()
{
    m_endpoint.runEventsLoop();
}

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
