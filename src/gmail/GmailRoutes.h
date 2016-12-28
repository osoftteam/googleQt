#pragma once

#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"
#include "google/endpoint/BatchRunner.h"
#include "google/endpoint/Cache.h"

#include "gmail/messages/MessagesRoutes.h"
#include "gmail/labels/LabelsRoutes.h"
#include "gmail/users/UsersRoutes.h"
#include "gmail/threads/ThreadsRoutes.h"
#include "gmail/history/HistoryRoutes.h"
#include "gmail/drafts/DraftsRoutes.h"

namespace googleQt{
  class Endpoint;

  class GmailRoutes{    
  public:

    GmailRoutes(Endpoint*);

    Endpoint* endpoint(){return m_endpoint;}

    messages::MessagesRoutes* getMessages();
    labels::LabelsRoutes*     getLabels();
    users::UsersRoutes*       getUsers();
    threads::ThreadsRoutes*   getThreads();
    history::HistoryRoutes*   getHistory();
    drafts::DraftsRoutes*     getDrafts();

    class MesagesReciever
    {
    public:
        MesagesReciever(GmailRoutes& r, EDataState f) :m_r(r), m_msg_format(f){};
        GoogleTask<messages::MessageResource>* route(QString message_id);
    protected:
        GmailRoutes&    m_r;
        EDataState      m_msg_format;
    };

    class GMailCacheQueryResult;
    class MessageData : public CacheData
    {
        friend class GMailCacheQueryResult;
    public:
        QString from()const { return m_from; }
        QString to()const { return m_to; }
        QString subject()const{return m_subject;}
        QString snippet()const { return m_snippet; }
        QString plain()const { return m_plain; }
        QString html()const { return m_html; }
    protected:
        QString m_from;
        QString m_to;
        QString m_subject;
        QString m_snippet;
        QString m_plain;
        QString m_html;
    private:
        MessageData(QString id, QString from, QString to, QString subject, QString snippet);
        MessageData(QString plain, QString html);
    };

    class GMailCacheQueryResult: public CacheQueryResult<MessageData>
    {
    public:
        GMailCacheQueryResult(EDataState load, ApiEndpoint& ept, GmailRoutes* gm);
        void fetchFromCloud_Async(const std::list<QString>& id_list)override;

    protected:
        GmailRoutes*  m_gm;
    };

    std::unique_ptr<BatchResult<QString, messages::MessageResource>>   getBatchMessages(EDataState, const std::list<QString>& id_list);
    BatchRunner<QString, MesagesReciever, messages::MessageResource>* getBatchMessages_Async(EDataState, const std::list<QString>& id_list);

//  BatchRunner<QString, MesagesReciever, messages::MessageResource>* getCacheMessages_Async(EDataState, const std::list<QString>& id_list);
  protected:
    std::unique_ptr<messages::MessagesRoutes>       m_MessagesRoutes;
    std::unique_ptr<labels::LabelsRoutes>           m_LabelsRoutes;
    std::unique_ptr<users::UsersRoutes>             m_UsersRoutes;
    std::unique_ptr<threads::ThreadsRoutes>         m_ThreadsRoutes;
    std::unique_ptr<history::HistoryRoutes>         m_HistoryRoutes;
    std::unique_ptr<drafts::DraftsRoutes>           m_DraftsRoutes;
    std::unique_ptr<MesagesReciever>                m_MessagesBatchReciever;
    Endpoint*  m_endpoint;
  };

};
