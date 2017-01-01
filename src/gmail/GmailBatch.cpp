#include "GmailBatch.h"
#include "GmailRoutes.h"

using namespace googleQt;

GoogleTask<messages::MessageResource>* mail_batch::MesagesReciever::route(QString message_id)
{   
    gmail::IdArg arg(message_id);
    if (m_msg_format == EDataState::snippet)
        {
            arg.setFormat("metadata");
            arg.headers().push_back("Subject");
            arg.headers().push_back("From");
        }
    else if (m_msg_format == EDataState::body)
        {
        
        }

    return m_r.getMessages()->get_Async(arg);
}

///GMailCache
mail_batch::GMailCache::GMailCache(ApiEndpoint& ept, GmailRoutes& r)
	:GoogleCache<MessageData, GMailCacheQueryResult>(ept), m_r(r)
{

};

std::unique_ptr<mail_batch::GMailCacheQueryResult> mail_batch::GMailCache::produceCloudResultFetcher(EDataState load, ApiEndpoint& ept)
{
	std::unique_ptr<mail_batch::GMailCacheQueryResult> rv(new mail_batch::GMailCacheQueryResult(load, ept, m_r, this));
	return rv;
};

///MessageData
mail_batch::MessageData::MessageData(QString id, QString from, QString subject, QString snippet)
    :CacheData(EDataState::snippet, id), m_from(from), m_subject(subject), m_snippet(snippet)
{
};

void mail_batch::MessageData::updateSnippet(QString from, QString subject, QString snippet)
{
	m_state_agg |= static_cast<int>(EDataState::snippet);
	m_from = from;
	m_subject = subject;
	m_snippet = snippet;
};

void mail_batch::MessageData::updateBody(QString plain, QString html)
{
	m_state_agg |= static_cast<int>(EDataState::body);
	m_plain = plain;
	m_html = html;
};

void mail_batch::MessageData::merge(CacheData* other)
{
    mail_batch::MessageData* md = dynamic_cast<mail_batch::MessageData*>(other);
    if(!md)
        {
            qWarning() << "Expected MessageData";
            return;
        }
    if(m_id != md->m_id)
        {
            qWarning() << "Expected ID-identity MessageData" << m_id << md->m_id;
            return;            
        }
    
    if(!isLoaded(EDataState::snippet))
        {
            if(md->isLoaded(EDataState::snippet))
                {
                    m_from = md->from();
                    m_subject = md->subject();
                    m_state_agg |= static_cast<int>(EDataState::snippet);
                }
        }

    if(!isLoaded(EDataState::body))
        {
            if(md->isLoaded(EDataState::body))
                {
                    m_plain = md->plain();
                    m_html = md->html();
                    m_state_agg |= static_cast<int>(EDataState::body);
                }
        }    
};

///GMailCacheQueryResult
mail_batch::GMailCacheQueryResult::GMailCacheQueryResult(EDataState load, ApiEndpoint& ept, GmailRoutes& r, GMailCache* c)
    :CacheQueryResult<MessageData>(load, ept, c), m_r(r)
{

};

void mail_batch::GMailCacheQueryResult::fetchFromCloud_Async(const std::list<QString>& id_list)
{
    if (id_list.empty())
        return;
    
    BatchRunner<QString,
                mail_batch::MesagesReciever,
                messages::MessageResource>* par_runner = NULL;

    switch (m_load)
        {
        case googleQt::EDataState::snippet:
            {
                par_runner = m_r.getBatchMessages_Async(EDataState::snippet, id_list);
            }
            break;
        case googleQt::EDataState::body: 
            {
                par_runner = m_r.getBatchMessages_Async(EDataState::body, id_list);
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
                        fetchMessage(m);
                    }

                //                notifyOnFinished();
                notifyFetchCompleted(m_result);
                par_runner->deleteLater();
            });
};

void mail_batch::GMailCacheQueryResult::loadHeaders(messages::MessageResource* m, QString& from, QString& subject)
{
	auto& header_list = m->payload().headers();
	for (auto& h : header_list)
	{
		if (h.name().compare("From", Qt::CaseInsensitive) == 0)
		{
			from = h.value();
		}
		else if (h.name().compare("Subject", Qt::CaseInsensitive) == 0)
		{
			subject = h.value();
		}
	}
};

void mail_batch::GMailCacheQueryResult::fetchMessage(messages::MessageResource* m)
{
	switch (m_load)
        {
        case googleQt::EDataState::snippet:
            {
                QString from, to, subject;
				loadHeaders(m, from, subject);
				std::shared_ptr<MessageData> md = std::make_shared<MessageData>(m->id(), from, subject, m->snippet());
                m_result[m->id()] = md;
            }break;
        case googleQt::EDataState::body:
            {
                QString plain_text, html_test;

                auto p = m->payload();
                auto parts = p.parts();
                for (auto pt : parts)
                    {
                        bool plain_text_loaded = false;
                        bool html_text_loaded = false;
                        auto pt_headers = pt.headers();
                        for (auto h : pt_headers)
                            {
                                if (h.name() == "Content-Type") {
                                    if ((h.value().indexOf("text/plain") == 0))
                                        {
                                            plain_text_loaded = true;
                                            const messages::MessagePartBody& pt_body = pt.body();
                                            plain_text = QByteArray::fromBase64(pt_body.data(), 
                                                                                QByteArray::Base64UrlEncoding).constData();
                                            break;
                                        }
                                    else if ((h.value().indexOf("text/html") == 0))
                                        {
                                            html_text_loaded = true;
                                            const messages::MessagePartBody& pt_body = pt.body();
                                            html_test = QByteArray::fromBase64(pt_body.data(), 
                                                                               QByteArray::Base64UrlEncoding).constData();
                                            break;
                                        }
                                }//"Content-Type"
                            }//pt_headers
                        if (plain_text_loaded && html_text_loaded)
                            break;
                    }// parts
                auto i = m_result.find(m->id());
                if (i == m_result.end())
                    {
                        qWarning() << "Expected partially loaded message[1]" << m->id();
						QString from, subject;
						loadHeaders(m, from, subject);
						std::shared_ptr<MessageData> md = std::make_shared<MessageData>(m->id(), from, subject, m->snippet());
						m_result[m->id()] = md;
						md->updateBody(plain_text, html_test);
                    }
                else
                    {
                        std::shared_ptr<MessageData> md = i->second;
						if (!md->isLoaded(googleQt::EDataState::snippet)) 
						{							
							QString from, to, subject;
							loadHeaders(m, from, subject);
							md->updateSnippet(from, subject, m->snippet());
						}
                        md->updateBody(plain_text, html_test);
                    }
            }break;
        }
};

