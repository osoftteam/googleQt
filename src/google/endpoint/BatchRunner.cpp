#include "BatchRunner.h"
#include "ApiEndpoint.h"

using namespace googleQt;

void BatchBaseRunner::notifyOnFinished() 
{
    emit finished();
    if (m_in_wait_loop)
    {
        m_endpoint.exitEventsLoop();
    }
};

void BatchBaseRunner::waitUntillFinishedOrCancelled()
{
    m_endpoint.runEventsLoop();
};