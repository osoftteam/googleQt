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

#include "gmail/GmailBatch.h"
#include "google/endpoint/Cache.h"

namespace googleQt{
  class Endpoint;

  namespace mail_batch {
	  class MesagesReciever;
	  class MessageData;
	  class GMailCacheQueryResult;
	  class GMailCache;
  };


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

    std::unique_ptr<BatchResult<QString, messages::MessageResource>>   getBatchMessages(EDataState, const std::list<QString>& id_list);
    BatchRunner<QString, mail_batch::MesagesReciever, messages::MessageResource>* getBatchMessages_Async(EDataState, const std::list<QString>& id_list);

	/// async load latest messagesCount emails starting pageToken and update cache
	void getCacheMessages_AsyncCB(EDataState, 
		int messagesCount = 40, 
		QString pageToken = "",		
		std::function<void(std::list<std::shared_ptr<mail_batch::MessageData>>)> completed_callback = nullptr,
		std::function<void(std::unique_ptr<GoogleException>)> failed_callback = nullptr
	);
	/// async load emails by ID-list while updating local cache
    std::list<std::shared_ptr<mail_batch::MessageData>> getCacheMessages(EDataState, const std::list<QString>& id_list);
	std::unique_ptr<mail_batch::GMailCacheQueryResult> getCacheMessages_Async(EDataState, const std::list<QString>& id_list);
    std::list<std::shared_ptr<mail_batch::MessageData>> getCacheMessages();
    bool setupSQLiteCache(QString dbPath, QString dbName = "googleqt", QString dbprefix = "api");

#ifdef API_QT_AUTOTEST
    void autotest();
    void autotestParLoad(EDataState state, const std::list<QString>& id_list);
    void autotestParDBLoad(EDataState state, const std::list<QString>& id_list);
#endif
  protected:
      void checkCacheObj();

    std::unique_ptr<messages::MessagesRoutes>       m_MessagesRoutes;
    std::unique_ptr<labels::LabelsRoutes>           m_LabelsRoutes;
    std::unique_ptr<users::UsersRoutes>             m_UsersRoutes;
    std::unique_ptr<threads::ThreadsRoutes>         m_ThreadsRoutes;
    std::unique_ptr<history::HistoryRoutes>         m_HistoryRoutes;
    std::unique_ptr<drafts::DraftsRoutes>           m_DraftsRoutes;
	std::unique_ptr<mail_batch::GMailCache>			m_GMailCache;
    Endpoint*  m_endpoint;
  };

};
