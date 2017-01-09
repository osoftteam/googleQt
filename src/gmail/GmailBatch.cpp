#include "GmailBatch.h"
#include "GmailRoutes.h"
#include <QSqlError>
#include <ctime>

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

std::unique_ptr<mail_batch::GMailCacheQueryResult> mail_batch::GMailCache::produceCloudResultFetcher(EDataState state, ApiEndpoint& ept)
{
	std::unique_ptr<mail_batch::GMailCacheQueryResult> rv(new mail_batch::GMailCacheQueryResult(state, ept, m_r, this));
	return rv;
};

///MessageData
mail_batch::MessageData::MessageData(QString id, QString from, QString subject, QString snippet)
    :CacheData(EDataState::snippet, id), m_from(from), m_subject(subject), m_snippet(snippet)
{
};

mail_batch::MessageData::MessageData(QString id, QString from, QString subject, QString snippet, QString plain, QString html) 
	:CacheData(EDataState::body, id), m_from(from), m_subject(subject), m_snippet(snippet), m_plain(plain), m_html(html)
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
mail_batch::GMailCacheQueryResult::GMailCacheQueryResult(EDataState state, ApiEndpoint& ept, GmailRoutes& r, GMailCache* c)
    :CacheQueryResult<MessageData>(state, ept, c), m_r(r)
{

};

void mail_batch::GMailCacheQueryResult::fetchFromCloud_Async(const std::list<QString>& id_list)
{
    if (id_list.empty())
        return;
    
    BatchRunner<QString,
                mail_batch::MesagesReciever,
                messages::MessageResource>* par_runner = NULL;

    par_runner = m_r.getBatchMessages_Async(m_state, id_list);
    /*
    switch (m_state)
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
        */

    connect(par_runner, &EndpointRunnable::finished, [=]() 
            {
                RESULT_LIST<messages::MessageResource*> res = par_runner->get();
                for (auto& m : res)
                    {
                        fetchMessage(m);
                    }

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
	switch (m_state)
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

///GMailSQLiteStorage
bool mail_batch::GMailSQLiteStorage::init(QString dbPath, QString dbName, QString db_meta_prefix)
{
    m_db_meta_prefix = db_meta_prefix;
    m_initialized = false;

    m_db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    m_db.setDatabaseName(dbPath);
    if (!m_db.open()) {
        qWarning() << "Failed to connect" << dbName << dbPath;
        return false;
    }

    m_query.reset(new QSqlQuery(m_db));

    QString sql = QString("CREATE TABLE IF NOT EXISTS %1gmail_msg(msg_id TEXT PRIMARY KEY, msg_from TEXT, msg_subject TEXT, msg_snippet TEXT, msg_plain TEXT, msg_html TEXT, msg_state INTEGER)").arg(m_db_meta_prefix);

    if (!execQuery(sql))
        return false;

    sql = QString("SELECT msg_state, msg_id, msg_from, msg_subject, msg_snippet, msg_plain, msg_html FROM %1gmail_msg").arg(m_db_meta_prefix);
    QSqlQuery* q = selectQuery(sql);
    if (!q)
        return false;

    //int 
    bool cache_avail = true;
    while (q->next() && cache_avail)
    {
        std::shared_ptr<MessageData> md = loadObjFromDB(q);
        if (md == nullptr)
            continue;

        cache_avail = false;
        if (md != nullptr)
        {
            cache_avail = m_mem_cache->mem_insert(md->id(), md);
        }
        else
        {
            break;
        }
    }

    m_initialized = true;


    return m_initialized;
};

std::list<QString> mail_batch::GMailSQLiteStorage::load(const std::list<QString>& id_list,
                                                        std::unique_ptr<GMailCacheQueryResult>& cr)
{   
    std::list<QString> rv;
    std::set<QString> db_loaded;
    bool cache_avail = true;
    std::function<bool(const std::list<QString>& lst)> loadSublist = [&](const std::list<QString>& lst) -> bool
        {
            QString comma_ids = slist2commalist_decorated(lst);
            QString sql = QString("SELECT msg_state, msg_id, msg_from, msg_subject, msg_snippet, msg_plain, msg_html FROM %1gmail_msg WHERE msg_id IN(%2)")
            .arg(m_db_meta_prefix)
            .arg(comma_ids);
            QSqlQuery* q = selectQuery(sql);
            if (!q)
                return false;
            while (q->next())
                {
                    std::shared_ptr<MessageData> md = loadObjFromDB(q);
                    if (md)
                        {
                        if (cache_avail)
                        {
                            cache_avail = m_mem_cache->mem_insert(md->id(), md);
                        }
                            db_loaded.insert(md->id());
                            cr->add(md->id(), md);
                        }
                }
            return true;
        };

    
    if(isValid())
        {
            if (chunk_list_execution(id_list, loadSublist)) 
                {
                    for (std::list<QString>::const_iterator i = id_list.begin(); i != id_list.end(); i++) 
                        {
                            if (db_loaded.find(*i) == db_loaded.end())
                                {
                                    rv.push_back(*i);
                                }
                        }
                };
        }
    return rv;
};

void mail_batch::GMailSQLiteStorage::update(EDataState state, CACHE_QUERY_RESULT_LIST<mail_batch::MessageData>& r)
{
    QString sql_update, sql_insert;
    switch (state)
    {
    case EDataState::snippet:
    {
        sql_update = QString("UPDATE %1gmail_msg SET msg_state, msg_from=?, msg_subject=?, msg_snippet=? WHERE msg_id=?")
            .arg(m_db_meta_prefix)
            .arg(static_cast<int>(state));
        sql_insert = QString("INSERT INTO %1gmail_msg(msg_id, msg_state, msg_from, msg_subject, msg_snippet) VALUES(?, %2, ?, ?, ?) WHERE changes()=0")
            .arg(m_db_meta_prefix)
            .arg(static_cast<int>(state));
    }break;
    case EDataState::body: 
    {
        sql_update = QString("UPDATE %1gmail_msg SET msg_state, msg_plain=?, msg_html=? WHERE msg_id=?")
            .arg(m_db_meta_prefix)
            .arg(static_cast<int>(state));
        sql_insert = QString("INSERT INTO %1gmail_msg(msg_id, msg_state, msg_plain, msg_html) VALUES(?, %2,?, ?) WHERE changes()=0")
            .arg(m_db_meta_prefix)
            .arg(static_cast<int>(state));
    }break;
    }        

    std::function<void(QSqlQuery*, mail_batch::MessageData*)> execWithValues = 
        [&](QSqlQuery* q, 
        mail_batch::MessageData* m)
    {
        switch (state)
        {
        case EDataState::snippet:
        {
            q->addBindValue(m->from());
            q->addBindValue(m->subject());
            q->addBindValue(m->snippet());
            q->addBindValue(m->id());
        }break;
        case EDataState::body:
        {
            q->addBindValue(m->id());
            q->addBindValue(m->plain());
            q->addBindValue(m->html());
        }break;
        }
        q->exec();
    };

    for(auto& i : r)
    {
        std::shared_ptr<mail_batch::MessageData>& m = i;
        QSqlQuery* q = prepareQuery(sql_update);
        if (!q)return;
        execWithValues(q, m.get());

        q = prepareQuery(sql_insert);
        if (!q)return;
        execWithValues(q, m.get());
    }
};

std::shared_ptr<mail_batch::MessageData> mail_batch::GMailSQLiteStorage::loadObjFromDB(QSqlQuery* q)
{
    std::shared_ptr<MessageData> md;

    int state_as_int = q->value(0).toInt();
    if (state_as_int != static_cast<int>(EDataState::snippet) &&
        state_as_int != static_cast<int>(EDataState::body))
        {
            qWarning() << "Invalid DB state" << state_as_int << q->value(1).toString();
            return nullptr;
        }
    EDataState st = static_cast<EDataState>(q->value(0).toInt());
    QString msg_id = q->value(1).toString();
    if (msg_id.isEmpty())
        {
            qWarning() << "Invalid(empty) DBID";
            return nullptr;
        }

    QString msg_from = q->value(2).toString();
    QString msg_subject = q->value(3).toString();
    QString msg_snippet = q->value(4).toString();
    QString msg_plain = "";
    QString msg_html = "";
    
    if (st == EDataState::body)
        {
            msg_plain = q->value(5).toString();
            msg_html = q->value(6).toString();
            md = std::make_shared<MessageData>(msg_id, msg_from, msg_subject, msg_snippet, msg_plain, msg_html);
        }
    else
        {
            md = std::make_shared<MessageData>(msg_id, msg_from, msg_subject, msg_snippet);
        }
    return md;
};

bool mail_batch::GMailSQLiteStorage::execQuery(QString sql)
{
    if(!m_query)
        {
            qWarning() << "Expected internal query";
            return false;
        }
    
    if(!m_query->prepare(sql))
        {
            QString error = m_query->lastError().text();
            qWarning() << "Failed to prepare sql query" << error << sql;
            return false;
        };    
    if(!m_query->exec(sql))
        {
            QString error = m_query->lastError().text();
            qWarning() << "Failed to execute query" << error << sql;
            return false;
        };

    return true;
};

QSqlQuery* mail_batch::GMailSQLiteStorage::prepareQuery(QString sql)
{
    if (!m_query)
    {
        qWarning() << "Expected internal query";
        return nullptr;
    }
    if (!m_query->prepare(sql))
    {
        QString error = m_query->lastError().text();
        qWarning() << "Failed to prepare sql query" << error << sql;
        return nullptr;
    };
    return m_query.get();
};

QSqlQuery* mail_batch::GMailSQLiteStorage::selectQuery(QString sql)
{
    QSqlQuery* q = prepareQuery(sql);
    if (!q)return nullptr;
    if(!q->exec(sql))
        {
            QString error = q->lastError().text();
            qWarning() << "Failed to execute query" << error << sql;
            return nullptr;
        };
    return q;
};

#ifdef API_QT_AUTOTEST
std::unique_ptr<mail_batch::MessageData> mail_batch::MessageData::EXAMPLE(EDataState st)
{
    static int idx = time(NULL);
    QString id = QString("%1").arg(idx);
    QString from = QString("%1_from").arg(idx);
    QString subject = QString("%1_subject").arg(idx);
    QString snippet = QString("%1_snippet").arg(idx);

    std::unique_ptr<mail_batch::MessageData> rv;
    switch (st) 
    {
    case EDataState::snippet:
    {
        rv.reset(new mail_batch::MessageData(id, from, subject, snippet));
    }break;
    case EDataState::body: 
    {
        QString plain = QString("%1_plain").arg(idx);
        QString html = QString("<html>%1_html</html>").arg(idx);

        rv.reset(new mail_batch::MessageData(id, from, subject, snippet, plain, html));
    }break;
    }
    return rv;
};
#endif //API_QT_AUTOTEST
