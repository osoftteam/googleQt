#include "GmailRoutes.h"
#include "Endpoint.h"


using namespace googleQt;

GmailRoutes::GmailRoutes(Endpoint* e):m_endpoint(e)
{

};

messages::MessagesRoutes* GmailRoutes::getMessages()
{
    if (!m_MessagesRoutes) {
        m_MessagesRoutes.reset(new messages::MessagesRoutes(m_endpoint));
    }
    return m_MessagesRoutes.get();
};


labels::LabelsRoutes* GmailRoutes::getLabels()
{
    if(!m_LabelsRoutes){
        m_LabelsRoutes.reset(new labels::LabelsRoutes(m_endpoint));
    }
    return m_LabelsRoutes.get();
};

users::UsersRoutes* GmailRoutes::getUsers()
{
    if(!m_UsersRoutes){
        m_UsersRoutes.reset(new users::UsersRoutes(m_endpoint));
    }
    return m_UsersRoutes.get();
};

threads::ThreadsRoutes* GmailRoutes::getThreads() 
{
    if (!m_ThreadsRoutes) {
        m_ThreadsRoutes.reset(new threads::ThreadsRoutes(m_endpoint));
    }
    return m_ThreadsRoutes.get();
};

history::HistoryRoutes* GmailRoutes::getHistory() 
{
    if (!m_HistoryRoutes) {
        m_HistoryRoutes.reset(new history::HistoryRoutes(m_endpoint));
    }
    return m_HistoryRoutes.get();
};

drafts::DraftsRoutes* GmailRoutes::getDrafts()
{
    if (!m_DraftsRoutes) {
        m_DraftsRoutes.reset(new drafts::DraftsRoutes(m_endpoint));
    }
    return m_DraftsRoutes.get();
};

GoogleTask<messages::MessageResource>* GmailRoutes::MesagesReciever::route(QString message_id)
{
    gmail::IdArg arg(message_id);
    if (m_msg_format == EDataState::partialyLoaded)
        {
            arg.setFormat("metadata");
            arg.headers().push_back("Subject");
            arg.headers().push_back("From");
        }
    else if (m_msg_format == EDataState::completlyLoaded)
        {
        
        }

    return m_r.getMessages()->get_Async(arg);
}


std::unique_ptr<BatchResult<QString, 
                            messages::MessageResource>> GmailRoutes::getBatchMessages(EDataState f,
                                                                                      const std::list<QString>& id_list)
{
    return getBatchMessages_Async(f, id_list)->waitForResultAndRelease();
};

BatchRunner<QString,
            GmailRoutes::MesagesReciever,
            messages::MessageResource>* GmailRoutes::getBatchMessages_Async(EDataState f, const std::list<QString>& id_list)
{
    if (!m_MessagesBatchReciever) {
        m_MessagesBatchReciever.reset(new MesagesReciever(*this, f));
    }

    BatchRunner<QString,
                GmailRoutes::MesagesReciever,
                messages::MessageResource>* r = new BatchRunner<QString,
                                                                GmailRoutes::MesagesReciever,
                                                                messages::MessageResource>(id_list, m_MessagesBatchReciever.get(), *m_endpoint);
    r->run();
    return r;
};

///MessageData
GmailRoutes::MessageData::MessageData(QString id, QString from, QString to, QString subject, QString snippet)
    :CacheData(EDataState::partialyLoaded, id), m_from(from), m_to(to), m_subject(subject), m_snippet(snippet)
{
};

///GMailCacheQueryResult
GmailRoutes::GMailCacheQueryResult::GMailCacheQueryResult(EDataState load, ApiEndpoint& ept, GmailRoutes* gm) 
    :CacheQueryResult<MessageData>(load, ept), m_gm(gm)
{

};

void GmailRoutes::GMailCacheQueryResult::fetchFromCloud_Async(const std::list<QString>& id_list)
{
    if (id_list.empty())
        return;

    BatchRunner<QString,
                GmailRoutes::MesagesReciever,
                messages::MessageResource>* par_runner = NULL;

    switch (m_load)
        {
        case googleQt::EDataState::partialyLoaded:
            {
                par_runner = m_gm->getBatchMessages_Async(EDataState::partialyLoaded, id_list);
            }
            break;
        case googleQt::EDataState::completlyLoaded: 
            {
                par_runner = m_gm->getBatchMessages_Async(EDataState::completlyLoaded, id_list);
            }
            break;
        default:
            break;
        }

    connect(par_runner, &EndpointRunnable::finished, [=]() 
    {
        RESULT_LIST<messages::MessageResource*> res = par_runner->get();
        for (auto& m : res)
        {
            auto& payload = m->payload();
            auto& header_list = payload.headers();

            switch (m_load)
                {
                case googleQt::EDataState::partialyLoaded:break;
                }
            
            notifyOnFinished();
        }

        par_runner->deleteLater();
    });
};
