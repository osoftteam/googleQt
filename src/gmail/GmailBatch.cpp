#include "GmailBatch.h"
#include "GmailRoutes.h"
#include <QSqlError>
#include <QDebug>
#include <ctime>

using namespace googleQt;

GoogleTask<messages::MessageResource>* mail_batch::MessagesReceiver::route(QString message_id)
{   
    gmail::IdArg arg(message_id);
    if (m_msg_format == EDataState::snippet)
        {
            arg.setFormat("metadata");
            arg.headers().push_back("Subject");
            arg.headers().push_back("From");
            arg.headers().push_back("To");
			arg.headers().push_back("Cc");
			arg.headers().push_back("Bcc");
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
mail_batch::GMailCache::GMailCache(ApiEndpoint& ept)
	:GoogleCache<MessageData, GMailCacheQueryResult>(ept)
{

};

///MessageData
mail_batch::MessageData::MessageData(QString id,
                                     QString from,
                                     QString to,
                                     QString cc,
                                     QString bcc,
                                     QString subject,
                                     QString snippet,
                                     qlonglong internalDate,
                                     qlonglong standardLabels)
    :CacheData(EDataState::snippet, id),
     m_from(from),
     m_to(to),
     m_cc(cc),
     m_bcc(bcc),
     m_subject(subject),
     m_snippet(snippet),
     m_internalDate(internalDate)     
{
    m_standardLabels.flags = standardLabels;
};

mail_batch::MessageData::MessageData(QString id,
                                     QString from,
                                     QString to,
                                     QString cc,
                                     QString bcc,
                                     QString subject,
                                     QString snippet,
                                     QString plain,
                                     QString html,
                                     qlonglong internalDate,
                                     qlonglong standardLabels)
	:CacheData(EDataState::body, id),
    m_from(from),
    m_to(to),
    m_cc(cc),
    m_bcc(bcc),
    m_subject(subject),
    m_snippet(snippet),
    m_plain(plain),
    m_html(html),
    m_internalDate(internalDate)
{
    m_standardLabels.flags = standardLabels;
};

mail_batch::MessageData::MessageData(int agg_state,
                                     QString id,
                                     QString from,
                                     QString to,
                                     QString cc,
                                     QString bcc,
                                     QString subject,
                                     QString snippet,
                                     QString plain,
                                     QString html,
                                     qlonglong internalDate,
                                     qlonglong standardLabels)
:CacheData(EDataState::body, id),
 m_from(from),
 m_to(to),
 m_cc(cc),
 m_bcc(bcc),
 m_subject(subject),
 m_snippet(snippet),
 m_plain(plain),
 m_html(html),
 m_internalDate(internalDate)
{
    m_state_agg = agg_state;
    m_standardLabels.flags = standardLabels;
};

void mail_batch::MessageData::updateSnippet(QString from,
                                            QString to,
                                            QString cc,
                                            QString bcc,
                                            QString subject,
                                            QString snippet,
                                            qlonglong standardLabels)
{
	m_state_agg |= static_cast<int>(EDataState::snippet);
	m_from = from;
    m_to = to;
    m_cc = cc;
    m_bcc = bcc;
	m_subject = subject;
	m_snippet = snippet;
    m_standardLabels.flags = standardLabels;
};

void mail_batch::MessageData::updateBody(QString plain, QString html)
{
	m_state_agg |= static_cast<int>(EDataState::body);
	m_plain = plain;
	m_html = html;
};

uint64_t mail_batch::MessageData::packStardardLabels(const std::list <QString>& labels)
{
    LABEL_FLAGS f;
    
    if(labels.size() > 0){
        for (auto& lb : labels) {
            f.flags |= labelMask(lb);
        }
    }
    return f.flags;
};

bool mail_batch::MessageData::inLabelFilter(uint64_t data)const
{
    bool rv = (data & m_standardLabels.flags) != 0;
    return rv;
};

#define RUN_LABELS_CHECK                        \
    CHECK_LABEL(IMPORTANT)                      \
    CHECK_LABEL(CHAT)                           \
    CHECK_LABEL(SENT)                           \
    CHECK_LABEL(INBOX)                          \
    CHECK_LABEL(TRASH)                          \
    CHECK_LABEL(DRAFT)                          \
    CHECK_LABEL(SPAM)                           \
    CHECK_LABEL(STARRED)                        \
    CHECK_LABEL(UNREAD)                         \
    CHECK_LABEL(CATEGORY_PERSONAL)              \
    CHECK_LABEL(CATEGORY_SOCIAL)                \
    CHECK_LABEL(CATEGORY_FORUMS)                \
    CHECK_LABEL(CATEGORY_UPDATES)               \
    CHECK_LABEL(CATEGORY_PROMOTIONS)            \


#define CHECK_LABEL(L)if(m_standardLabels.L == 1)labels.push_back(#L);
std::list<QString> mail_batch::MessageData::getLabels()const
{
    std::list<QString> labels;
    RUN_LABELS_CHECK;
    return labels;
};

#undef CHECK_LABEL
#define CHECK_LABEL(L) else if(lb.compare(#L, Qt::CaseInsensitive) == 0){f.L = 1;}

uint64_t mail_batch::MessageData::labelMask(QString lb)
{
    LABEL_FLAGS f;
    if(lb.isEmpty()){}
    //RUN_LABELS_CHECK;
    /*
    if(lb.compare("INBOX", Qt::CaseInsensitive) == 0){
        f.INBOX = 1;
    }else if(lb.compare("SPAM", Qt::CaseInsensitive) == 0){
        f.SPAM = 1;
    }else if(lb.compare("TRASH", Qt::CaseInsensitive) == 0){
        f.TRASH = 1;
    }else if(lb.compare("UNREAD", Qt::CaseInsensitive) == 0){
        f.UNREAD = 1;
    }else if(lb.compare("STARRED", Qt::CaseInsensitive) == 0){
        f.STARRED = 1;
    }if(lb.compare("IMPORTANT", Qt::CaseInsensitive) == 0){
        f.IMPORTANT = 1;
    }
    CHECK_LABEL(INBOX);
    */
    return f.flags;
};

#undef CHECK_LABEL

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
    
    UserBatchRunner<QString,
                mail_batch::MessagesReceiver,
                messages::MessageResource>* par_runner = NULL;

    par_runner = m_r.getUserBatchMessages_Async(m_state, id_list);
 
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

void mail_batch::GMailCacheQueryResult::loadHeaders(messages::MessageResource* m,
                                                    QString& from,
                                                    QString& to,
													QString& cc,
													QString& bcc,
                                                    QString& subject)
{
	auto& header_list = m->payload().headers();
	for (auto& h : header_list)
        {
            if (h.name().compare("From", Qt::CaseInsensitive) == 0)
                {
                    from = h.value();
                }
            else if (h.name().compare("To", Qt::CaseInsensitive) == 0)
                {
                    to = h.value();
                }
			else if (h.name().compare("CC", Qt::CaseInsensitive) == 0)
                {
                    cc = h.value();
                }
			else if (h.name().compare("BCC", Qt::CaseInsensitive) == 0)
                {
                    bcc = h.value();
                }
            else if (h.name().compare("Subject", Qt::CaseInsensitive) == 0)
                {
                    subject = h.value();
                }
        }
};

void mail_batch::GMailCacheQueryResult::loadLabels(messages::MessageResource* m, uint64_t& f)
{
    const std::list <QString>& labels = m->labelids();
    if(labels.size() > 0){
        f = MessageData::packStardardLabels(labels);
    }
};

void mail_batch::GMailCacheQueryResult::fetchMessage(messages::MessageResource* m)
{
	switch (m_state)
        {
        case googleQt::EDataState::snippet:
            {
                uint64_t labels;
                loadLabels(m, labels);
                QString from, to, cc, bcc, subject;
				loadHeaders(m, from, to, cc, bcc, subject);
				std::shared_ptr<MessageData> md = std::make_shared<MessageData>(m->id(), 
                                                                                from, 
                                                                                to, 
                                                                                cc,
                                                                                bcc,
                                                                                subject, 
                                                                                m->snippet(), 
                                                                                m->internaldate(),
                                                                                labels);
                add(md);
            }break;
        case googleQt::EDataState::body:
            {
                QString plain_text, html_text;
				auto p = m->payload();
				if (p.mimetype().compare("text/html") == 0)
                    {
                        QByteArray payload_body = QByteArray::fromBase64(p.body().data(), QByteArray::Base64UrlEncoding);
                        html_text = payload_body.constData();
                        plain_text = html_text;
                        plain_text.remove(QRegExp("<[^>]*>"));
                    }
				else
                    {
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
                                                    html_text = QByteArray::fromBase64(pt_body.data(),
                                                                                       QByteArray::Base64UrlEncoding).constData();
                                                    break;
                                                }
                                        }//"Content-Type"
                                    }//pt_headers
                                if (plain_text_loaded && html_text_loaded)
                                    break;
                            }// parts
                    }

                auto i = m_result.find(m->id());
                if (i == m_result.end())
                    {
                        uint64_t labels;
                        loadLabels(m, labels);
						QString from, to, cc, bcc, subject;
						loadHeaders(m, from, to, cc, bcc, subject);
						std::shared_ptr<MessageData> md = std::make_shared<MessageData>(m->id(), 
                                                                                        from, 
                                                                                        to, 
                                                                                        cc, 
                                                                                        bcc, 
                                                                                        subject, 
                                                                                        m->snippet(), 
                                                                                        m->internaldate(),
                                                                                        labels);
                        add(md);
						md->updateBody(plain_text, html_text);
                    }
                else
                    {
                        std::shared_ptr<MessageData> md = i->second;
						if (!md->isLoaded(googleQt::EDataState::snippet)) 
                            {
                                uint64_t labels;
                                loadLabels(m, labels);
                                QString from, to, cc, bcc, subject;
                                loadHeaders(m, from, to, cc, bcc, subject);
                                md->updateSnippet(from, to, cc, bcc, subject, m->snippet(), labels);
                            }
                        md->updateBody(plain_text, html_text);
                    }
            }break;//body
        }
};

static bool compare_internalDate(std::shared_ptr<mail_batch::MessageData>& f,
                                 std::shared_ptr<mail_batch::MessageData>& s) 
{
    return (f->internalDate() > s->internalDate());
};

std::unique_ptr<mail_batch::MessagesList> mail_batch::GMailCacheQueryResult::waitForSortedResultListAndRelease()
{
    if (!isFinished())
        {
            m_in_wait_loop = true;
            waitUntillFinishedOrCancelled();
        }

    m_result_list.sort(compare_internalDate);
    std::unique_ptr<mail_batch::MessagesList> rv(new mail_batch::MessagesList);
    rv->messages = std::move(m_result_list);
    rv->messages_map = std::move(m_result);
    rv->state = m_state;

    deleteLater();
    return rv;
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

    QString sql = QString("CREATE TABLE IF NOT EXISTS %1gmail_msg(msg_id TEXT PRIMARY KEY, msg_from TEXT, "
                          "msg_to TEXT, msg_cc TEXT, msg_bcc TEXT, msg_subject TEXT, msg_snippet TEXT, msg_plain TEXT, "
                          "msg_html TEXT, internal_date INTEGER, msg_state INTEGER, msg_cache_lock INTEGER, msg_standard_labels INTEGER)").arg(m_db_meta_prefix);

    if (!execQuery(sql))
        return false;

    sql = QString("SELECT msg_state, msg_id, msg_from, msg_to, msg_cc, msg_bcc, "
                  "msg_subject, msg_snippet, msg_plain, msg_html, internal_date, msg_standard_labels FROM %1gmail_msg ORDER BY internal_date DESC").arg(m_db_meta_prefix);
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
                        }
                    else
                        {
                            cache_avail = m_mem_cache->mem_insert(md->id(), md);
                        }
                }
            else
                {
                    break;
                }

            loaded_objects++;
        }

#ifdef API_QT_AUTOTEST
    ApiAutotest::INSTANCE() << QString("DB-loaded %1 records, skipped %2, mem-cache-size: %3")
        .arg(loaded_objects).arg(skipped).arg(m_mem_cache->mem_size());
#endif

    m_initialized = true;

    return m_initialized;
};

std::list<QString> mail_batch::GMailSQLiteStorage::load(EDataState state, 
                                                        const std::list<QString>& id_list,
                                                        GMailCacheQueryResult* cr)
{   
    std::list<QString> rv;
    std::set<QString> db_loaded;
    bool cache_avail = true;
    std::function<bool(const std::list<QString>& lst)> loadSublist = [&](const std::list<QString>& lst) -> bool
        {
            QString comma_ids = slist2commalist_decorated(lst);
            QString sql = QString("SELECT msg_state, msg_id, msg_from, msg_to, msg_cc, msg_bcc, "
                                  "msg_subject, msg_snippet, msg_plain, msg_html, internal_date, msg_standard_labels FROM %1gmail_msg WHERE msg_id IN(%2)")
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
            sql += " ORDER BY internal_date DESC";
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
                            cr->add(md);
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
                sql_update = QString("UPDATE %1gmail_msg SET msg_state=?, msg_from=?, msg_to=?, "
                                     "msg_cc=?, msg_bcc=?, msg_subject=?, msg_snippet=?, internal_date=?, msg_standard_labels=? WHERE msg_id=?")
                    .arg(m_db_meta_prefix);
                sql_insert = QString("INSERT INTO %1gmail_msg(msg_state, msg_from, msg_to, msg_cc, msg_bcc, msg_subject, "
                                     "msg_snippet, internal_date, msg_standard_labels, msg_id) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")
                    .arg(m_db_meta_prefix);
            }break;
        case EDataState::body: 
            {
                sql_update = QString("UPDATE %1gmail_msg SET msg_state=?, msg_from=?, msg_to=?, msg_cc=?, msg_bcc=?, "
                                     "msg_subject=?, msg_snippet=?, msg_plain=?, msg_html=?, internal_date=?, msg_standard_labels=? WHERE msg_id=?")
                    .arg(m_db_meta_prefix);
                sql_insert = QString("INSERT INTO %1gmail_msg(msg_state, msg_from, msg_to, msg_cc, msg_bcc, msg_subject, "
                                     "msg_snippet, msg_plain, msg_html, internal_date, msg_standard_labels, msg_id) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")
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
                    q->addBindValue(m->to());
                    q->addBindValue(m->cc());
                    q->addBindValue(m->bcc());
                    q->addBindValue(m->subject());
                    q->addBindValue(m->snippet());
                    q->addBindValue(m->internalDate());
                    q->addBindValue(m->stardardLabelsFlags());
                    q->addBindValue(m->id());
                }break;
                case EDataState::body:
                {
                    q->addBindValue(m->aggState());
                    q->addBindValue(m->from());
                    q->addBindValue(m->to());
                    q->addBindValue(m->cc());
                    q->addBindValue(m->bcc());
                    q->addBindValue(m->subject());
                    q->addBindValue(m->snippet());
                    q->addBindValue(m->plain());
                    q->addBindValue(m->html());
                    q->addBindValue(m->internalDate());
                    q->addBindValue(m->stardardLabelsFlags());
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

void mail_batch::GMailSQLiteStorage::remove(const std::set<QString>& set_of_ids2remove) 
{
    std::list<QString> ids2remove;
    for (auto& i : set_of_ids2remove) 
        {
            ids2remove.push_back(i);
        }

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


void mail_batch::GMailSQLiteStorage::updateStandardLabels(QString msg_id, uint64_t flags)
{
    QString sql_update;
    sql_update = QString("UPDATE %1gmail_msg SET msg_standard_labels=%2 WHERE msg_id=%3")
        .arg(m_db_meta_prefix)
        .arg(flags)
        .arg(msg_id);
    execQuery(sql_update);
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
    QString msg_to = q->value(3).toString();
    QString msg_cc = q->value(4).toString();
    QString msg_bcc = q->value(5).toString();
    QString msg_subject = q->value(6).toString();
    QString msg_snippet = q->value(7).toString();
    QString msg_plain = "";
    QString msg_html = "";
    
    if (agg_state > 1)
        {
            msg_plain = q->value(8).toString();
            msg_html = q->value(9).toString();
        }
    qlonglong  msg_internalDate = q->value(10).toLongLong();
    qlonglong  msg_standardLabels = q->value(11).toLongLong();
    
    md = std::shared_ptr<MessageData>(new MessageData(agg_state, 
                                                      msg_id, 
                                                      msg_from, 
                                                      msg_to, 
                                                      msg_cc,
                                                      msg_bcc,
                                                      msg_subject, 
                                                      msg_snippet, 
                                                      msg_plain, 
                                                      msg_html, 
                                                      msg_internalDate,
                                                      msg_standardLabels));
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

