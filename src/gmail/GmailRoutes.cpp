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
	if (m_msg_format == replySnippet)
	{
		arg.setFormat("metadata");
		arg.headers().push_back("Subject");
		arg.headers().push_back("From");
	}
	else if (m_msg_format == replyBody) 
	{
		
	}

	return m_r.getMessages()->get_Async(arg);
}


std::unique_ptr<BatchResult<QString, 
	messages::MessageResource>> GmailRoutes::getBatchMessages(MesagesReciever::replyFormat f,
							const std::list<QString>& id_list)
{
	return getBatchMessages_Async(f, id_list)->waitForResultAndRelease();
};

BatchRunner<QString,
	GmailRoutes::MesagesReciever,
	messages::MessageResource>* GmailRoutes::getBatchMessages_Async(MesagesReciever::replyFormat f, const std::list<QString>& id_list)
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