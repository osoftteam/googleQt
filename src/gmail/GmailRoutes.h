#pragma once

#include "google/endpoint/ApiUtil.h"
#include "google/endpoint/ApiClient.h"
#include "google/endpoint/BatchRunner.h"

#include "gmail/messages/MessagesRoutes.h"
#include "gmail/attachments/AttachmentsRoutes.h"
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
	  class MessagesReceiver;
	  class MessageData;
	  class GMailCacheQueryResult;
	  class GMailCache;
  };


  class GmailRoutes{    
  public:

    GmailRoutes(Endpoint*);

    Endpoint* endpoint(){return m_endpoint;}

    messages::MessagesRoutes* getMessages();
	attachments::AttachmentsRoutes* getAttachments();
    labels::LabelsRoutes*     getLabels();
    users::UsersRoutes*       getUsers();
    threads::ThreadsRoutes*   getThreads();
    history::HistoryRoutes*   getHistory();
    drafts::DraftsRoutes*     getDrafts();

    std::unique_ptr<UserBatchResult<QString, messages::MessageResource>>   getUserBatchMessages(EDataState, const std::list<QString>& id_list);
    UserBatchRunner<QString, mail_batch::MessagesReceiver, messages::MessageResource>* getUserBatchMessages_Async(EDataState, const std::list<QString>& id_list);

	/// check for new emails - get top messagesCount messages and update cache
    std::unique_ptr<mail_batch::MessagesList> getNextCacheMessages(int messagesCount = 40, 
		QString pageToken = "", 
		QStringList* labels = nullptr,
		std::set<QString>* msg2skip = nullptr);
    mail_batch::GMailCacheQueryResult* getNextCacheMessages_Async(int messagesCount = 40, 
		QString pageToken = "", 
		QStringList* labels = nullptr,
		std::set<QString>* msg2skip = nullptr);

    /// load emails by ID-list while updating local cache
    std::unique_ptr<mail_batch::MessagesList> getCacheMessages(EDataState, const std::list<QString>& id_list);    
	mail_batch::GMailCacheQueryResult* getCacheMessages_Async(EDataState, const std::list<QString>& id_list, 
        mail_batch::GMailCacheQueryResult* rfetcher = nullptr);

    /// load messages from cache, numberOfMessages = -1 if all messages from cache
    std::unique_ptr<mail_batch::MessagesList> getCacheMessages(int numberOfMessages);

	void setStarred(mail_batch::MessageData* d, bool starred_on = true);
	GoogleTask<messages::MessageResource>* setStarred_Async(mail_batch::MessageData* d, bool starred_on = true);

    /// init local cache table using SQlite DB, tables will have 'dbprefix' prefix
    /// file path and DB-name should be specified
    bool setupSQLiteCache(QString dbPath, QString dbName = "googleqt", QString dbprefix = "api");

#ifdef API_QT_AUTOTEST
    void autotest();
    void autotestParLoad(EDataState state, const std::list<QString>& id_list);
    void autotestParDBLoad(EDataState state, const std::list<QString>& id_list);
#endif
  protected:
      void ensureCache();
      mail_batch::GMailCacheQueryResult* newResultFetcher(EDataState state);

    std::unique_ptr<messages::MessagesRoutes>       m_MessagesRoutes;
	std::unique_ptr<attachments::AttachmentsRoutes> m_AttachmentsRoutes;
    std::unique_ptr<labels::LabelsRoutes>           m_LabelsRoutes;
    std::unique_ptr<users::UsersRoutes>             m_UsersRoutes;
    std::unique_ptr<threads::ThreadsRoutes>         m_ThreadsRoutes;
    std::unique_ptr<history::HistoryRoutes>         m_HistoryRoutes;
    std::unique_ptr<drafts::DraftsRoutes>           m_DraftsRoutes;
	std::unique_ptr<mail_batch::GMailCache>			m_GMailCache;
    Endpoint*  m_endpoint;
  };

};
