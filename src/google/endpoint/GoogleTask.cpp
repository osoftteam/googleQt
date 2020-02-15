#include "GoogleTask.h"
#include "ApiEndpoint.h"
#include "ApiClient.h"

using namespace googleQt;

///EndpointRunnable
EndpointRunnable::EndpointRunnable(ApiClient* cl)
{
	m_client = cl->shared_from_this();
};

EndpointRunnable::~EndpointRunnable() 
{
    detachProgress();
};


void EndpointRunnable::notifyOnFinished()
{
    if (!m_finished_delegates.empty()) {
        for (auto& d : m_finished_delegates) {
            d();
        }
        m_finished_delegates.clear();
    }

    m_finished = true;
    emit finished();
    if (m_in_wait_loop)
    {
		m_client->endpoint()->exitEventsLoop();
    }
};

void EndpointRunnable::waitUntillFinishedOrCancelled()
{
	m_client->endpoint()->runEventsLoop();
};

bool EndpointRunnable::waitForResult()const
{
    if (!isCompleted() && !isFailed())
    {
        m_in_wait_loop = true;
		m_client->endpoint()->runEventsLoop();
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

void EndpointRunnable::disposeLater() 
{
    if (m_dispose_delegates.empty()) {
        for (auto& d : m_dispose_delegates) {
            d();
        }
        m_dispose_delegates.clear();
    }
    deleteLater();
};

void EndpointRunnable::addDisposeDelegate(std::function<void()> dispose_callback)
{
    m_dispose_delegates.push_back(dispose_callback);
};

void EndpointRunnable::addFinishedDelegate(std::function<void()> finished_callback)
{
    m_finished_delegates.push_back(finished_callback);
};

void EndpointRunnable::detachProgress() 
{
    if (m_progress_notifier_owner) {
        if (m_progress) {
            m_progress->deleteLater();          
        }       
    }

    m_progress = nullptr;
    m_progress_notifier_owner = false;
};

TaskProgress* EndpointRunnable::createProgressNotifier()
{
    if (m_progress) {
        if (m_progress_notifier_owner) {
            qWarning() << "task already is an owner of ProgressNotifier, don't create twice";
            return m_progress;
        }
        else {
            qWarning() << "task already has ProgressNotifier, don't create twice";
            return m_progress;
        }
    }

    detachProgress();
    m_progress = new TaskProgress();
    m_progress_notifier_owner = true;
    return m_progress;
};

void EndpointRunnable::delegateProgressNotifier(TaskProgress* p) 
{
    detachProgress();
    m_progress_notifier_owner = false;
    m_progress = p;
};

/**
    GoogleVoidTask
*/
void GoogleVoidTask::waitForResultAndRelease()
{
    if (!isCompleted() && !isFailed())
    {
        m_in_wait_loop = true;
		m_client->endpoint()->runEventsLoop();
    }

    if (isFailed())
    {
        std::unique_ptr<GoogleException> ex;
        ex = std::move(m_failed);
        disposeLater();
        if (ex)
            ex->raise();
    }
    disposeLater();
};

void GoogleVoidTask::then(std::function<void()> after_completed_processing, 
    std::function<void(std::unique_ptr<GoogleException>)> on_error,
    std::function<void()> on_dispose)
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
        disposeLater();
    };

    if (on_dispose) {
        addDisposeDelegate(on_dispose);
    }

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
        if (t->isFailed())
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

void TaskAggregator::disposeLater()
{
    for (auto t : m_runnables) {
        t->disposeLater();
    }

    EndpointRunnable::disposeLater();
};

void TaskAggregator::waitForResultAndRelease()
{
    if (!isCompleted() && !isFailed())
    {
        for (auto t : m_runnables) {
            connect(t, &EndpointRunnable::finished,
                [=]()
            {
                if (areAllFinished()) {
                    completed_callback();
                }
            });
        }
        m_in_wait_loop = true;
		m_client->endpoint()->runEventsLoop();
    }

    if (isFailed())
    {
        std::unique_ptr<GoogleException> ex;
        for (auto t : m_runnables) {
            if (t->isFailed()) {
                GoogleException* e = t->error();
                if (e) {
                    ex.reset(e->clone());
                    disposeLater();
                    if (ex)
                        ex->raise();
                    break;
                }
            }
        }
    }

    disposeLater();
}

void TaskAggregator::then(std::function<void()> after_completed_processing /*= nullptr*/,
    std::function<void(std::unique_ptr<GoogleException>)> on_error /*= nullptr*/,
    std::function<void()> on_dispose /*= nullptr*/)
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
        disposeLater();
    };

    if (on_dispose) {
        addDisposeDelegate(on_dispose);
    }

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

/**
    TaskProgress
*/
TaskProgress::TaskProgress() 
{

};

int TaskProgress::value()const 
{
    return m_value;
};

int TaskProgress::maximum()const 
{
    return m_max;
};

QString TaskProgress::statusText()const 
{
    return m_status_text;
};

void TaskProgress::setValue(int value) 
{
    m_value = value;
    emit valueChanged(m_value);
};

void TaskProgress::setMaximum(int maxv, QString statusText)
{
    m_max = maxv;
    m_status_text = statusText;
    emit valueChanged(m_value);
};
