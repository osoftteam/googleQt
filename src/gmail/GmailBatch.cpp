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
#ifdef API_QT_AUTOTEST
    ApiAutotest::INSTANCE().addId("messages::MessageResource", message_id);
#endif
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

mail_batch::MessageData::MessageData(int agg_state, QString id, QString from, QString subject, QString snippet, QString plain, QString html) 
    :CacheData(EDataState::body, id), m_from(from), m_subject(subject), m_snippet(snippet), m_plain(plain), m_html(html)
{
    m_state_agg = agg_state;
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
 
    std::function<void(void)> fetchMessagesOnFinished = [=]() 
    {
        RESULT_LIST<messages::MessageResource*> res = par_runner->get();
        for (auto& m : res)
        {
            fetchMessage(m);
        }
        std::set<QString> id_set;
        for (std::list<QString>::const_iterator i = id_list.cbegin(); i != id_list.cend(); i++)
        {
            id_set.insert(*i);
        }
        notifyFetchCompleted(m_result, id_set);
        par_runner->deleteLater();
    };

    if (par_runner->isFinished())
    {
        fetchMessagesOnFinished();
    }
    else
    {
        connect(par_runner, &EndpointRunnable::finished, [=]()
        {
            fetchMessagesOnFinished();
        });
    }
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
//                        qWarning() << "Expected partially loaded message[1]" << m->id();
						QString from, subject;
						loadHeaders(m, from, subject);
                        qDebug() << m->id() << from << subject;
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

    int loaded_objects = 0, skipped = 0;
    bool cache_avail = true;
    while (q->next() && cache_avail)
    {
        std::shared_ptr<MessageData> md = loadObjFromDB(q);
        if (md == nullptr)
            continue;

        cache_avail = false;
        if (md != nullptr)
        {
            if (m_mem_cache->mem_has_object(md->id())) 
            {
                skipped++;
/*#ifdef API_QT_AUTOTEST
                ApiAutotest::INSTANCE() << QString("%1. DB-loaded (skipped) %2").arg(loaded_objects + 1).arg(md->id());
#endif*/
            }
            else
            {
                cache_avail = m_mem_cache->mem_insert(md->id(), md);
/*#ifdef API_QT_AUTOTEST
                ApiAutotest::INSTANCE() << QString("%1. DB-loaded %2").arg(loaded_objects + 1).arg(md->id());
#endif*/
            }
        }
        else
        {
            break;
        }

        loaded_objects++;
    }

#ifdef API_QT_AUTOTEST
    ApiAutotest::INSTANCE() << QString("DB-loaded %1 records, skipped %2, mem-cache-size: %3").arg(loaded_objects).arg(skipped).arg(m_mem_cache->mem_size());
#endif

    m_initialized = true;

    return m_initialized;
};

std::list<QString> mail_batch::GMailSQLiteStorage::load(EDataState state, 
                                                        const std::list<QString>& id_list,
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
            switch (state)
            {
            case EDataState::snippet: 
            {
                sql += " AND (msg_state = 1 OR msg_state = 3)";
            }break;
            case EDataState::body:
            {
                sql += " AND (msg_state = 2 OR msg_state = 3)";
            }break;
            }
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
                            auto j = db_loaded.find(*i);
                            if (j == db_loaded.end())
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
        sql_update = QString("UPDATE %1gmail_msg SET msg_state=?, msg_from=?, msg_subject=?, msg_snippet=? WHERE msg_id=?")
            .arg(m_db_meta_prefix);
        sql_insert = QString("INSERT INTO %1gmail_msg(msg_state, msg_from, msg_subject, msg_snippet, msg_id) VALUES(?, ?, ?, ?, ?)")
            .arg(m_db_meta_prefix);
    }break;
    case EDataState::body: 
    {
        sql_update = QString("UPDATE %1gmail_msg SET msg_state=?, msg_from=?, msg_subject=?, msg_snippet=?, msg_plain=?, msg_html=? WHERE msg_id=?")
            .arg(m_db_meta_prefix);
        sql_insert = QString("INSERT INTO %1gmail_msg(msg_state, msg_from, msg_subject, msg_snippet, msg_plain, msg_html, msg_id) VALUES(?, ?, ?, ?, ?, ?, ?)")
            .arg(m_db_meta_prefix);
    }break;
    }        

    std::function<bool (QSqlQuery*, mail_batch::MessageData*)> execWithValues = 
        [&](QSqlQuery* q, 
        mail_batch::MessageData* m) -> bool
    {
        switch (state)
        {
        case EDataState::snippet:
        {
            q->addBindValue(m->aggState());
            q->addBindValue(m->from());
            q->addBindValue(m->subject());
            q->addBindValue(m->snippet());
            q->addBindValue(m->id());
        }break;
        case EDataState::body:
        {
            q->addBindValue(m->aggState());
            q->addBindValue(m->from());
            q->addBindValue(m->subject());
            q->addBindValue(m->snippet());
            q->addBindValue(m->plain());
            q->addBindValue(m->html());
            q->addBindValue(m->id());
        }break;
        }
        return q->exec();
    };

    int updated_records = 0;
    int inserted_records = 0;

    for(auto& i : r)
    {
        std::shared_ptr<mail_batch::MessageData>& m = i;
        QSqlQuery* q = prepareQuery(sql_update);
        if (!q)return;
        bool ok = execWithValues(q, m.get());
        if (!ok) 
        {
            qWarning() << "SQL update failed" << q->lastError().text() << i->id();
        }
        int rows_updated = q->numRowsAffected();
        if (rows_updated <= 0)
        {
            q = prepareQuery(sql_insert);
            if (!q)return;
            ok = execWithValues(q, m.get());
            if (!ok)
            {
                qWarning() << "SQL insert failed" << q->lastError().text() << i->id();
            }
            else
            {
                rows_updated = q->numRowsAffected();
                if (rows_updated > 0)
                    inserted_records++;
                qDebug() << "insert " << i->id() << " " << rows_updated << "/" << inserted_records << "/" << r.size();
            }
        }
        else 
        {
            updated_records++;
            qDebug() << "update " << i->id() << " " << rows_updated << "/" << updated_records << "/" << r.size();
        }
    }
};

void mail_batch::GMailSQLiteStorage::remove(const std::list<QString>& ids2remove) 
{
    std::function<bool(const std::list<QString>& lst)> removeSublist = [&](const std::list<QString>& lst) -> bool
    {
        QString comma_ids = slist2commalist_decorated(lst);
        QString sql = QString("DELETE FROM %1gmail_msg WHERE msg_id IN(%2)")
            .arg(m_db_meta_prefix)
            .arg(comma_ids);
        QSqlQuery* q = prepareQuery(sql);
        if (!q)return false;
        if (!q->exec()) return false;
        return true;
    };

    if (isValid())
    {
        if(!chunk_list_execution(ids2remove, removeSublist))
        {
            qWarning() << "Failed to remove list of records.";
        }
    }
};

std::shared_ptr<mail_batch::MessageData> mail_batch::GMailSQLiteStorage::loadObjFromDB(QSqlQuery* q)
{
    std::shared_ptr<MessageData> md;

    int agg_state = q->value(0).toInt();
    if (agg_state < 1 || agg_state > 3)
        {
            qWarning() << "Invalid DB state" << agg_state << q->value(1).toString();
            return nullptr;
        }
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
    
    if (agg_state > 1)
    {
        msg_plain = q->value(5).toString();
        msg_html = q->value(6).toString();
    }

    md = std::shared_ptr<MessageData>(new MessageData(agg_state, msg_id, msg_from, msg_subject, msg_snippet, msg_plain, msg_html));
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
