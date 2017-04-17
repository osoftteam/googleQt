#include "GmailBatch.h"
#include "GmailRoutes.h"
#include <QSqlError>
#include <QDebug>
#include <ctime>

using namespace googleQt;

GoogleTask<messages::MessageResource>* mail_cache::MessagesReceiver::route(QString message_id)
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
mail_cache::GMailCache::GMailCache(ApiEndpoint& ept)
	:GoogleCache<MessageData, GMailCacheQueryResult>(ept)
{

};

mail_cache::GMailSQLiteStorage* mail_cache::GMailCache::sqlite_storage()
{
	mail_cache::GMailSQLiteStorage* rv = nullptr;
	if (m_localDB)
	{
		rv = dynamic_cast<mail_cache::GMailSQLiteStorage*>(m_localDB.get());
	}
	return rv;
};

///MessageData
mail_cache::MessageData::MessageData(QString id,
                                     QString from,
                                     QString to,
                                     QString cc,
                                     QString bcc,
                                     QString subject,
                                     QString snippet,
                                     qlonglong internalDate,
                                     qlonglong labels)
    :CacheData(EDataState::snippet, id),
     m_from(from),
     m_to(to),
     m_cc(cc),
     m_bcc(bcc),
     m_subject(subject),
     m_snippet(snippet),
     m_internalDate(internalDate)     
{
    m_labels = labels;
};

mail_cache::MessageData::MessageData(QString id,
                                     QString from,
                                     QString to,
                                     QString cc,
                                     QString bcc,
                                     QString subject,
                                     QString snippet,
                                     QString plain,
                                     QString html,
                                     qlonglong internalDate,
                                     qlonglong labels)
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
    m_labels = labels;
};

mail_cache::MessageData::MessageData(int agg_state,
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
                                     qlonglong labels)
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
    m_labels = labels;
};

void mail_cache::MessageData::updateSnippet(QString from,
                                            QString to,
                                            QString cc,
                                            QString bcc,
                                            QString subject,
                                            QString snippet,
                                            qlonglong labels)
{
	m_state_agg |= static_cast<int>(EDataState::snippet);
	m_from = from;
    m_to = to;
    m_cc = cc;
    m_bcc = bcc;
	m_subject = subject;
	m_snippet = snippet;
    m_labels = labels;
};

void mail_cache::MessageData::updateBody(QString plain, QString html)
{
	m_state_agg |= static_cast<int>(EDataState::body);
	m_plain = plain;
	m_html = html;
};

bool mail_cache::MessageData::inLabelFilter(uint64_t data)const
{
    bool rv = (data & m_labels) != 0;
    return rv;
};

void mail_cache::MessageData::merge(CacheData* other)
{
    mail_cache::MessageData* md = dynamic_cast<mail_cache::MessageData*>(other);
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

mail_cache::LabelData::LabelData() 
{

};

mail_cache::LabelData::LabelData(QString id, 
	QString name, 
	int mask_base,
	bool as_system,
	uint64_t unread_messages):
m_label_id(id), 
m_label_name(name), 
m_mask_base(mask_base),
m_is_system_label(as_system),
m_unread_messages(unread_messages)
{
	static uint64_t theone = 1;
	m_label_mask = (theone << m_mask_base);
};

bool mail_cache::LabelData::isStarred()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::STARRED), Qt::CaseInsensitive) == 0);
    return rv;
};

bool mail_cache::LabelData::isUnread()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::UNREAD), Qt::CaseInsensitive) == 0);
    return rv;
};

bool mail_cache::LabelData::isSpam()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::SPAM), Qt::CaseInsensitive) == 0);
    return rv;
};

bool mail_cache::LabelData::isTrash()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::TRASH), Qt::CaseInsensitive) == 0);
    return rv;
};

bool mail_cache::LabelData::isDraft()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::DRAFT), Qt::CaseInsensitive) == 0);
    return rv;
};


bool mail_cache::LabelData::isImportant()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::IMPORTANT), Qt::CaseInsensitive) == 0);
    return rv;
};


bool mail_cache::LabelData::isPromotionCategory()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::CATEGORY_PROMOTIONS), Qt::CaseInsensitive) == 0);
    return rv;
};

bool mail_cache::LabelData::isForumsCategory()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::CATEGORY_FORUMS), Qt::CaseInsensitive) == 0);
    return rv;
};

bool mail_cache::LabelData::isUpdatesCategory()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::CATEGORY_UPDATES), Qt::CaseInsensitive) == 0);
    return rv;
};

bool mail_cache::LabelData::isSocialCategory()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::CATEGORY_SOCIAL), Qt::CaseInsensitive) == 0);
    return rv;
};


///GMailCacheQueryResult
mail_cache::GMailCacheQueryResult::GMailCacheQueryResult(EDataState state, ApiEndpoint& ept, GmailRoutes& r, GMailCache* c)
    :CacheQueryResult<MessageData>(state, ept, c), m_r(r)
{

};

void mail_cache::GMailCacheQueryResult::fetchFromCloud_Async(const std::list<QString>& id_list)
{
    if (id_list.empty())
        return;
    
    UserBatchRunner<QString,
                mail_cache::MessagesReceiver,
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

void mail_cache::GMailCacheQueryResult::loadHeaders(messages::MessageResource* m,
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

void mail_cache::GMailCacheQueryResult::loadLabels(messages::MessageResource* m, uint64_t& f)
{
    const std::list <QString>& labels = m->labelids();
    if(labels.size() > 0){
		f = m_r.cache()->sqlite_storage()->packLabels(labels);
    }
};

void mail_cache::GMailCacheQueryResult::fetchMessage(messages::MessageResource* m)
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

static bool compare_internalDate(std::shared_ptr<mail_cache::MessageData>& f,
                                 std::shared_ptr<mail_cache::MessageData>& s) 
{
    return (f->internalDate() > s->internalDate());
};

std::unique_ptr<mail_cache::MessagesList> mail_cache::GMailCacheQueryResult::waitForSortedResultListAndRelease()
{
    if (!isFinished())
        {
            m_in_wait_loop = true;
            waitUntillFinishedOrCancelled();
        }

    m_result_list.sort(compare_internalDate);
    std::unique_ptr<mail_cache::MessagesList> rv(new mail_cache::MessagesList);
    rv->messages = std::move(m_result_list);
    rv->messages_map = std::move(m_result);
    rv->state = m_state;

    deleteLater();
    return rv;
};

///GMailSQLiteStorage
bool mail_cache::GMailSQLiteStorage::init(QString dbPath, QString dbName, QString db_meta_prefix)
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

    static QString sql_messages = QString("CREATE TABLE IF NOT EXISTS %1gmail_msg(msg_id TEXT PRIMARY KEY, msg_from TEXT, "
                          "msg_to TEXT, msg_cc TEXT, msg_bcc TEXT, msg_subject TEXT, msg_snippet TEXT, msg_plain TEXT, "
                          "msg_html TEXT, internal_date INTEGER, msg_state INTEGER, msg_cache_lock INTEGER, msg_labels INTEGER)").arg(m_db_meta_prefix);
    if (!execQuery(sql_messages))
        return false;

	static QString sql_labels = QString("CREATE TABLE IF NOT EXISTS %1gmail_label(label_id TEXT PRIMARY KEY, label_name TEXT, "
								" label_type INTEGER, label_unread_messages INTEGER, label_mask INTEGER)")
		.arg(m_db_meta_prefix);
	if (!execQuery(sql_labels))
		return false;

	for (int i = 0; i < 64; i++)
		m_avail_label_base.insert(i);

	if (!loadLabelsFromDb())
		return false;

	if (!loadMessagesFromDb())
		return false;	

    m_initialized = true;
    return m_initialized;
};

bool mail_cache::GMailSQLiteStorage::loadMessagesFromDb()
{
	QString sql = QString("SELECT msg_state, msg_id, msg_from, msg_to, msg_cc, msg_bcc, "
		"msg_subject, msg_snippet, msg_plain, msg_html, internal_date, msg_labels FROM %1gmail_msg ORDER BY internal_date DESC").arg(m_db_meta_prefix);
	QSqlQuery* q = selectQuery(sql);
	if (!q)
		return false;

	int loaded_objects = 0, skipped = 0;
	bool cache_avail = true;
	while (q->next() && cache_avail)
	{
		std::shared_ptr<MessageData> md = loadMessageFromDB(q);
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

	return true;
};

static std::vector<QString>& getSysLabels()
{
    static std::vector<QString> sys_labels;
    if(sys_labels.empty()){
        sys_labels.push_back("IMPORTANT");
        sys_labels.push_back("CHAT");
        sys_labels.push_back("SENT");
        sys_labels.push_back("INBOX");
        sys_labels.push_back("TRASH");
        sys_labels.push_back("DRAFT");
        sys_labels.push_back("SPAM");
        sys_labels.push_back("STARRED");
        sys_labels.push_back("UNREAD");
        sys_labels.push_back("CATEGORY_PERSONAL");
        sys_labels.push_back("CATEGORY_SOCIAL");
        sys_labels.push_back("CATEGORY_FORUMS");
        sys_labels.push_back("CATEGORY_UPDATES");
        sys_labels.push_back("CATEGORY_PROMOTIONS");        
    }
    return sys_labels;
}

QString mail_cache::sysLabelId(SysLabel l)
{
    std::vector<QString>& labels_arr = getSysLabels();
    int idx = (int)l;
    if(idx < 0 || idx >= (int)labels_arr.size()){
        qWarning() << "Invalid SysLabel index" << idx << labels_arr.size();
        return "";
    }
    
    QString s = getSysLabels()[idx];
    return s;
};

bool mail_cache::GMailSQLiteStorage::loadLabelsFromDb()
{
	static QString sql = QString("SELECT label_id, label_name, label_type, label_unread_messages, label_mask FROM %1gmail_label ORDER BY label_id").arg(m_db_meta_prefix);
	QSqlQuery* q = selectQuery(sql);
	if (!q)
		return false;

    std::set<QString> system_labels2ensure;
    std::vector<QString>& sys_labels = getSysLabels();
    for(auto& lbl : sys_labels){
        system_labels2ensure.insert(lbl);
    }
    
	while (q->next())
	{
		QString label_id = q->value(0).toString();
		QString label_name = q->value(1).toString();
		bool label_is_system = (q->value(2).toInt() == 1);
		uint64_t unread_messages = q->value(3).toLongLong();
		int mask_base = q->value(4).toInt();

		if (createAndInsertLabel(label_id,
			label_name,
			label_is_system,
			unread_messages,
			mask_base))
		{
			system_labels2ensure.erase(label_id);
		}
	}

	if (!system_labels2ensure.empty()) {
		for (auto& s : system_labels2ensure) {
			mail_cache::LabelData* lbl = ensureLabel(s, true);
			if (!lbl) {
				qWarning() << "failed to create label" << s << m_labels.size();
			}
		}
	}

	return true;
};

mail_cache::LabelData* mail_cache::GMailSQLiteStorage::createAndInsertLabel(
	QString label_id,
	QString label_name,
	bool label_is_system,
	uint64_t unread_messages,
	int mask_base
) 
{
	std::shared_ptr<mail_cache::LabelData> lb(new LabelData(label_id,
		label_name,
		mask_base,
		label_is_system,
		unread_messages));

	m_avail_label_base.erase(mask_base);
	m_labels[lb->labelId()] = lb;
	m_maskbase2labels[mask_base] = lb;
	return lb.get();
};

bool mail_cache::GMailSQLiteStorage::updateDbLabel(QString label_id, QString name, uint64_t unread_messages)
{
	QString sql_update;
	sql_update = QString("UPDATE %1gmail_label SET label_name='%2', label_unread_messages=%3 WHERE label_id='%4'")
		.arg(m_db_meta_prefix)
		.arg(name)
		.arg(unread_messages)
		.arg(label_id);
	return execQuery(sql_update);
};

mail_cache::LabelData* mail_cache::GMailSQLiteStorage::insertDbLabel(QString label_id,
	QString label_name,
	QString label_type,
	uint64_t unread_messages) 
{
	if (m_avail_label_base.empty()) 
	{
		qWarning() << "ERROR. Exhausted labels masks. New label can not be registered." << m_avail_label_base.size() << label_id << label_name;
		return nullptr;
	}

	auto i = m_avail_label_base.begin();
	int mask_base = *i;

	QString sql_insert;
	sql_insert = QString("INSERT INTO  %1gmail_label(label_id, label_name, label_type, label_unread_messages, label_mask) VALUES(?, ?, ?, ?, ?)")
		.arg(m_db_meta_prefix);
	QSqlQuery* q = prepareQuery(sql_insert);
	if (!q)return nullptr;

	int label_type_as_int = 2;
	if (label_type.compare("system", Qt::CaseInsensitive) == 0) {
		label_type_as_int = 1;
	}

	q->addBindValue(label_id);
	q->addBindValue(label_name);
	q->addBindValue(label_type_as_int);
	q->addBindValue(unread_messages);
	q->addBindValue(mask_base);

	mail_cache::LabelData* rv = nullptr;

	if (q->exec())
	{
		rv = createAndInsertLabel(label_id,
				label_name,
				(label_type_as_int == 1),
				unread_messages,
				mask_base);
	}

	return rv;
};

std::list<QString> mail_cache::GMailSQLiteStorage::load(EDataState state, 
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
                                  "msg_subject, msg_snippet, msg_plain, msg_html, internal_date, msg_labels FROM %1gmail_msg WHERE msg_id IN(%2)")
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
                    std::shared_ptr<MessageData> md = loadMessageFromDB(q);
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

void mail_cache::GMailSQLiteStorage::update(EDataState state, CACHE_QUERY_RESULT_LIST<mail_cache::MessageData>& r)
{
    QString sql_update, sql_insert;
    switch (state)
        {
        case EDataState::snippet:
            {
                sql_update = QString("UPDATE %1gmail_msg SET msg_state=?, msg_from=?, msg_to=?, "
                                     "msg_cc=?, msg_bcc=?, msg_subject=?, msg_snippet=?, internal_date=?, msg_labels=? WHERE msg_id=?")
                    .arg(m_db_meta_prefix);
                sql_insert = QString("INSERT INTO %1gmail_msg(msg_state, msg_from, msg_to, msg_cc, msg_bcc, msg_subject, "
                                     "msg_snippet, internal_date, msg_labels, msg_id) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")
                    .arg(m_db_meta_prefix);
            }break;
        case EDataState::body: 
            {
                sql_update = QString("UPDATE %1gmail_msg SET msg_state=?, msg_from=?, msg_to=?, msg_cc=?, msg_bcc=?, "
                                     "msg_subject=?, msg_snippet=?, msg_plain=?, msg_html=?, internal_date=?, msg_labels=? WHERE msg_id=?")
                    .arg(m_db_meta_prefix);
                sql_insert = QString("INSERT INTO %1gmail_msg(msg_state, msg_from, msg_to, msg_cc, msg_bcc, msg_subject, "
                                     "msg_snippet, msg_plain, msg_html, internal_date, msg_labels, msg_id) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")
                    .arg(m_db_meta_prefix);
            }break;
        }        

    std::function<bool (QSqlQuery*, mail_cache::MessageData*)> execWithValues = 
        [&](QSqlQuery* q, 
            mail_cache::MessageData* m) -> bool
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
                    q->addBindValue(m->labelsBitMap());
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
                    q->addBindValue(m->labelsBitMap());
                    q->addBindValue(m->id());
                }break;
                }
            return q->exec();
        };

    int updated_records = 0;
    int inserted_records = 0;

    for(auto& i : r)
        {
            std::shared_ptr<mail_cache::MessageData>& m = i;
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

void mail_cache::GMailSQLiteStorage::remove(const std::set<QString>& set_of_ids2remove) 
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

mail_cache::LabelData* mail_cache::GMailSQLiteStorage::findLabel(QString label_id)
{
	mail_cache::LabelData* rv = nullptr;
	auto i = m_labels.find(label_id);
	if (i != m_labels.end()) {
		rv = i->second.get();
	}
	return rv;
};

mail_cache::LabelData* mail_cache::GMailSQLiteStorage::findLabel(SysLabel sys_label)
{
    QString lable_id = sysLabelId(sys_label);
    return findLabel(lable_id);
};

mail_cache::LabelData* mail_cache::GMailSQLiteStorage::ensureLabel(QString label_id, bool system_label)
{
	auto i = m_labels.find(label_id);
	if (i != m_labels.end()) {
		return i->second.get();
	}

	mail_cache::LabelData* rv = insertDbLabel(label_id,
                                              label_id,
                                              system_label ? "system" : "user",
                                              0);
	return rv;
};

std::list<mail_cache::LabelData*> mail_cache::GMailSQLiteStorage::getAllLabels() 
{
	std::list<mail_cache::LabelData*> rv;
	for (auto& i : m_labels) {
		rv.push_back(i.second.get());
	}
	return rv;
};

uint64_t mail_cache::GMailSQLiteStorage::packLabels(const std::list <QString>& labels)
{
	uint64_t f = 0;

	if (labels.size() > 0) {
		for (auto& label_id : labels) {
			auto i = m_labels.find(label_id);
			if (i != m_labels.end()) {
				auto lb = i->second.get();
				f |= lb->labelMask();
			}
			else {
				static int warning_count = 0;
				if (warning_count < 100)
				{
					qWarning() << "lost label (pack)" << label_id << m_labels.size();
					warning_count++;
				}
			}
		}
	}

	return f;
};

std::list<mail_cache::LabelData*> mail_cache::GMailSQLiteStorage::unpackLabels(const uint64_t& data)const
{
	std::list<mail_cache::LabelData*> labels;

	uint64_t theone = 1;
	for (int b = 0; b < 64; b++)
	{
		uint64_t m = (theone << b);
		if (m > data)
			break;
		if (m & data) {
			auto i = m_maskbase2labels.find(b);
			if (i != m_maskbase2labels.end()){
				labels.push_back(i->second.get());
			}
			else {
				static int warning_count = 0;
				if (warning_count < 100)
				{
					qWarning() << "lost label (unpack)" << b << m_maskbase2labels.size();
					warning_count++;
				}
			}

		}
	}

	return labels;
};

void mail_cache::GMailSQLiteStorage::updateMessageLabels(QString msg_id, uint64_t flags)
{
    QString sql_update;
    sql_update = QString("UPDATE %1gmail_msg SET msg_labels=%2 WHERE msg_id='%3'")
        .arg(m_db_meta_prefix)
        .arg(flags)
        .arg(msg_id);
    execQuery(sql_update);
};

std::shared_ptr<mail_cache::MessageData> mail_cache::GMailSQLiteStorage::loadMessageFromDB(QSqlQuery* q)
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
    qlonglong  msg_labels = q->value(11).toLongLong();
    
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
														msg_labels));
    return md;
};

bool mail_cache::GMailSQLiteStorage::execQuery(QString sql)
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

QSqlQuery* mail_cache::GMailSQLiteStorage::prepareQuery(QString sql)
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

QSqlQuery* mail_cache::GMailSQLiteStorage::selectQuery(QString sql)
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

