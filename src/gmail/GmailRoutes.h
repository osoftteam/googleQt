#pragma once

#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"
#include "google/endpoint/BatchRunner.h"

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
		enum replyFormat 
		{
			replySnippet,
			replyBody,
		};

		MesagesReciever(GmailRoutes& r, replyFormat f) :m_r(r), m_msg_format(f){};
		GoogleTask<messages::MessageResource>* route(QString message_id);
	protected:
		GmailRoutes&	m_r;
		replyFormat		m_msg_format;
	};

	std::unique_ptr<BatchResult<QString, messages::MessageResource>>   getBatchMessages(MesagesReciever::replyFormat, const std::list<QString>& id_list);
	BatchRunner<QString, MesagesReciever, messages::MessageResource>* getBatchMessages_Async(MesagesReciever::replyFormat, const std::list<QString>& id_list);

  protected:
    std::unique_ptr<messages::MessagesRoutes>       m_MessagesRoutes;
    std::unique_ptr<labels::LabelsRoutes>           m_LabelsRoutes;
    std::unique_ptr<users::UsersRoutes>             m_UsersRoutes;
    std::unique_ptr<threads::ThreadsRoutes>         m_ThreadsRoutes;
    std::unique_ptr<history::HistoryRoutes>         m_HistoryRoutes;
    std::unique_ptr<drafts::DraftsRoutes>           m_DraftsRoutes;
	std::unique_ptr<MesagesReciever>				m_MessagesBatchReciever;
    Endpoint*  m_endpoint;
  };

};
