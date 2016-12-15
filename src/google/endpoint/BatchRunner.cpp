#include "BatchRunner.h"
#include "ApiEndpoint.h"

using namespace googleQt;

void BatchBaseRunner::waitUntillFinishedOrCancelled()
{
	m_endpoint.runEventsLoop();
};