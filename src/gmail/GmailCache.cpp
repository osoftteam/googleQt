#include "GmailRoutes.h"
#include <QSqlError>
#include <QDebug>
#include <ctime>
#include <QDir>

using namespace googleQt;

mail_cache::MessagesReceiver::MessagesReceiver(GmailRoutes& r, QString userId, EDataState f) 
	:m_r(r), 
     m_userId(userId),
     m_msg_format(f) 
{

};

GoogleTask<messages::MessageResource>* mail_cache::MessagesReceiver::routeSingleBatchRequest(QString message_id)
{   
    gmail::IdArg arg(m_userId, message_id);
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
    :GoogleCache<MessageData, GMailCacheQueryTask>(ept)
{
	m_valid = true;
	m_userId = ept.apiClient()->userId();
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

void mail_cache::GMailCache::topCacheData(GMailCacheQueryTask* rfetcher, int number2load, uint64_t labelFilter)
{
    int count = 0;
    for (auto& i : m_order_cache){
        auto m = i;
        if (m->inLabelFilter(labelFilter)){
            rfetcher->add(i);
            if (number2load > 0){
                if (++count >= number2load)
                    break;
            }
        }
    }
    rfetcher->notifyOnCompletedFromCache();
}


///MessageData
mail_cache::MessageData::MessageData(QString userid, 
									 QString id,
                                     QString from,
                                     QString to,
                                     QString cc,
                                     QString bcc,
                                     QString subject,
                                     QString snippet,
                                     qlonglong internalDate,
                                     qlonglong labels)
    :CacheData(EDataState::snippet, id),
     m_userId(userid),
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

mail_cache::MessageData::MessageData(QString userid,
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
 m_userId(userid),
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

mail_cache::MessageData::MessageData(QString userid, 
									 int agg_state,
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
 m_userId(userid),
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
    bool rv = true;
    if (data != 0) {
        rv = (data & m_labels) != 0;
    }
    return rv;
};

bool mail_cache::MessageData::inLabelFilter(const std::set<uint64_t>& ANDfilter)const 
{
    for (auto i = ANDfilter.begin(); i != ANDfilter.end(); i++) {
        if (*i != 0) {
            bool in_filter = (*i & m_labels) != 0;
            if (!in_filter) {
                return false;
            }
        }
    }
    return true;
};

const mail_cache::ATTACHMENTS_LIST& mail_cache::MessageData::getAttachments(GMailSQLiteStorage* storage)
{
    if (!m_attachments.empty()) {
        return m_attachments;
    }

    storage->loadAttachmentsFromDb(*this);
    return m_attachments;
};

void mail_cache::MessageData::merge(CacheData* other)
{
    mail_cache::MessageData* md = dynamic_cast<mail_cache::MessageData*>(other);
    if(!md)
        {
            qWarning() << "ERROR. Expected MessageData";
            return;
        }
    if(m_id != md->m_id)
        {
            qWarning() << "ERROR. Expected ID-identity MessageData" << m_id << md->m_id;
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

mail_cache::AttachmentData::AttachmentData() 
{
};

mail_cache::AttachmentData::AttachmentData(QString att_id,
                                           QString mimetype,
                                           QString filename,
                                           quint64 size)
    :m_attachment_id(att_id),
     m_mimetype(mimetype),
     m_filename(filename),
     m_size(size) 
{

};

mail_cache::AttachmentData::AttachmentData(QString att_id,
                                           QString mimetype,
                                           QString filename,
                                           QString local_filename,
                                           quint64 size,
                                           EStatus status)
    :m_attachment_id(att_id),
     m_mimetype(mimetype),
     m_filename(filename),
     m_local_filename(local_filename),
     m_size(size),
     m_status(status)
{

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


///GMailCacheQueryTask
mail_cache::GMailCacheQueryTask::GMailCacheQueryTask(QString userId, 
                                                     EDataState state, 
                                                     ApiEndpoint& ept, 
                                                     GmailRoutes& r, 
                                                     std::shared_ptr<GMailCache> c)
    :CacheQueryTask<MessageData>(state, ept, c), m_userId(userId), m_r(r)
{

};

void mail_cache::GMailCacheQueryTask::fetchFromCloud_Async(const std::list<QString>& id_list)
{
    if (id_list.empty())
        return;
    
    UserBatchRunner<QString,
                    mail_cache::MessagesReceiver,
                    messages::MessageResource>* par_runner = NULL;

    par_runner = m_r.getUserBatchMessages_Async(m_userId, m_completed->state, id_list);
    
    connect(par_runner, &EndpointRunnable::finished, [=]()
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
                notifyFetchCompleted(m_completed->result_map, id_set);
                par_runner->deleteLater();
            }); 
};

void mail_cache::GMailCacheQueryTask::loadHeaders(messages::MessageResource* m,
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

void mail_cache::GMailCacheQueryTask::loadLabels(messages::MessageResource* m, uint64_t& f)
{
    const std::list <QString>& labels = m->labelids();
    if(labels.size() > 0){
		f = dynamic_cast<mail_cache::GMailCache*>(m_cache.get())->sqlite_storage()->packLabels(labels);        
    }
};

void mail_cache::GMailCacheQueryTask::loadAttachments(messages::MessageResource* m, ATTACHMENTS_LIST& lst)
{
    auto p = m->payload();
    auto& parts = p.parts();
    for (auto& pt : parts) {
        auto& b = pt.body();
        if (b.size() > 0 && !b.attachmentid().isEmpty()) {
            att_ptr att = std::make_shared<AttachmentData>(b.attachmentid(), pt.mimetype(), pt.filename(), b.size());
            lst.push_back(att);
        }
    }
};

void mail_cache::GMailCacheQueryTask::fetchMessage(messages::MessageResource* m)
{
    switch (m_completed->state)
        {
        case googleQt::EDataState::snippet:
            {
                uint64_t labels;
                loadLabels(m, labels);
                QString from, to, cc, bcc, subject;
                loadHeaders(m, from, to, cc, bcc, subject);
                std::shared_ptr<MessageData> md = std::make_shared<MessageData>(m_userId,
																			    m->id(),
                                                                                from, 
                                                                                to, 
                                                                                cc,
                                                                                bcc,
                                                                                subject, 
                                                                                m->snippet(), 
                                                                                m->internaldate(),
                                                                                labels);
                //snipped - there will be no attachments here and no body..
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

                auto i = m_completed->result_map.find(m->id());
                if (i == m_completed->result_map.end())
                    {
                        uint64_t labels;
                        loadLabels(m, labels);
                        QString from, to, cc, bcc, subject;
                        loadHeaders(m, from, to, cc, bcc, subject);
                        std::shared_ptr<MessageData> md = std::make_shared<MessageData>(m_userId,
																						m->id(),
                                                                                        from, 
                                                                                        to, 
                                                                                        cc, 
                                                                                        bcc, 
                                                                                        subject, 
                                                                                        m->snippet(), 
                                                                                        m->internaldate(),
                                                                                        labels);
                        add(md);
                        loadAttachments(m, md->m_attachments);
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
                        loadAttachments(m, md->m_attachments);
                        md->updateBody(plain_text, html_text);
                    }
            }break;//body
        }
};

static bool compare_internalDate(mail_cache::msg_ptr& f,
                                 mail_cache::msg_ptr& s)
{
    return (f->internalDate() > s->internalDate());
};

std::unique_ptr<CacheDataList<mail_cache::MessageData>> mail_cache::GMailCacheQueryTask::waitForResultAndRelease()
{
    if (!isFinished())
        {
            m_in_wait_loop = true;
            waitUntillFinishedOrCancelled();
        }
    m_completed->result_list.sort(compare_internalDate);
    return std::move(m_completed);
};

///GMailSQLiteStorage
mail_cache::GMailSQLiteStorage::GMailSQLiteStorage(cache_ptr c)
{
	m_mem_cache = c;
};

mail_cache::cache_ptr mail_cache::GMailSQLiteStorage::lock_cache()
{
	mail_cache::cache_ptr c = m_mem_cache.lock();
	if (!c) {
		return c;
	}
	if (!c->isValid()) {
		qWarning() << "ERROR. Invalid cache state. Possible user/context switched." 
                   << c->userId() 
                   << c->endpoint().apiClient()->userId();
		c.reset();
		return c;
	}
	return c;
};

bool mail_cache::GMailSQLiteStorage::init_db(QString dbPath, 
                                             QString downloadPath,
                                             QString dbName, 
                                             QString db_meta_prefix)
{
	m_accId = 0;
	cache_ptr cache = lock_cache();
	if (!cache) {
		return false;
	}

	QString userId = cache->endpoint().apiClient()->userId();
	if (userId.isEmpty()) {
		qWarning() << "ERROR. Expected userid (email) for gmail cache";
		return false;
	}
	qDebug() << "Initializing cache for userid: " << userId;


    m_dbPath = dbPath;
    m_downloadDir = downloadPath;
    m_dbName = dbName;
    m_metaPrefix = db_meta_prefix;

    QDir dest_dir;
    if (!dest_dir.exists(m_downloadDir)) {
        if (!dest_dir.mkpath(m_downloadDir)) {
            qWarning() << "ERROR. Failed to create directory" << m_downloadDir;
            return false;
        };
    }


    m_initialized = false;  

    m_db = QSqlDatabase::addDatabase("QSQLITE", dbName);
    m_db.setDatabaseName(dbPath);
    if (!m_db.open()) {
        qWarning() << "ERROR. Failed to connect" << dbName << dbPath;
        return false;
    }

    m_query.reset(new QSqlQuery(m_db));

    /// accounts ///
    QString sql_accounts = QString("CREATE TABLE IF NOT EXISTS %1gmail_account(acc_id INTEGER PRIMARY KEY, userid TEXT NOT NULL)").arg(m_metaPrefix);
    if (!execQuery(sql_accounts))
        return false;

	if (!execQuery(QString("CREATE UNIQUE INDEX IF NOT EXISTS %1acc_userid_idx ON %1gmail_account(userid)").arg(m_metaPrefix)))
		return false;


    /// messages ///
    QString sql_messages = QString("CREATE TABLE IF NOT EXISTS %1gmail_msg(acc_id INTEGER NOT NULL, msg_id TEXT NOT NULL, msg_from TEXT, "
                                   "msg_to TEXT, msg_cc TEXT, msg_bcc TEXT, msg_subject TEXT, msg_snippet TEXT, msg_plain TEXT, "
                                   "msg_html TEXT, internal_date INTEGER, msg_state INTEGER, msg_cache_lock INTEGER, msg_labels INTEGER)").arg(m_metaPrefix);
    if (!execQuery(sql_messages))
        return false;

    if (!execQuery(QString("CREATE UNIQUE INDEX IF NOT EXISTS %1msg_accid_idx ON %1gmail_msg(acc_id, msg_id)").arg(m_metaPrefix)))
        return false;


    //// labels ///
    QString sql_labels = QString("CREATE TABLE IF NOT EXISTS %1gmail_label(acc_id INTEGER NOT NULL, label_id TEXT NOT NULL, label_name TEXT, "
                                 " label_type INTEGER, label_unread_messages INTEGER, label_total_messages INTEGER,"
                                 " message_list_visibility TEXT, label_list_visibility TEXT, label_mask INTEGER)")
        .arg(m_metaPrefix);
    if (!execQuery(sql_labels))
        return false;

    if (!execQuery(QString("CREATE UNIQUE INDEX IF NOT EXISTS %1lbl_accid_idx ON %1gmail_label(acc_id, label_id)").arg(m_metaPrefix)))
        return false;

    
    /// attachments ///
    QString sql_att = QString("CREATE TABLE IF NOT EXISTS %1gmail_attachments(att_id TEXT, acc_id INTEGER NOT NULL, msg_id TEXT NOT NULL, file_name TEXT, "
                              "local_file_name TEXT, mime_type TEXT, size INTEGER, PRIMARY KEY (att_id, msg_id))")
        .arg(m_metaPrefix);
    if (!execQuery(sql_att))
        return false;        
    
    if (!execQuery(QString("CREATE UNIQUE INDEX IF NOT EXISTS %1att_accid_idx ON %1gmail_attachments(acc_id, msg_id, att_id)").arg(m_metaPrefix)))
        return false;

	/// locate accountID
	QString sql = QString("SELECT acc_id FROM %1gmail_account WHERE userid='%2' COLLATE NOCASE").arg(m_metaPrefix).arg(userId);
	QSqlQuery* q = selectQuery(sql);
	if (!q)
		return false;
	if (q->next()) {
		m_accId = q->value(0).toInt();
	}
	else {
		sql = QString("INSERT INTO %1gmail_account(userid) VALUES(?)").arg(m_metaPrefix);
		QSqlQuery* q = prepareQuery(sql);
		if (!q)return false;
		q->addBindValue(userId);
		if (q->exec()) {
			m_accId = q->lastInsertId().toInt();
		}
	}

	qDebug() << "using acc-id" << m_accId << "for user" << userId;

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
	cache_ptr cache = lock_cache();
	if (!cache) {
		return false;
	}

    QString sql = QString("SELECT msg_state, msg_id, msg_from, msg_to, msg_cc, msg_bcc, "
                          "msg_subject, msg_snippet, msg_plain, msg_html, internal_date, msg_labels FROM %1gmail_msg WHERE acc_id=%2 ORDER BY internal_date DESC").arg(m_metaPrefix).arg(m_accId);
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
                    if (cache->mem_has_object(md->id()))
                        {
                            skipped++;
                        }
                    else
                        {
                            cache_avail = cache->mem_insert(md->id(), md);
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
        .arg(loaded_objects).arg(skipped).arg(cache->mem_size());
#endif

    return true;
};


static std::map<QString, QString> syslabelID2Name;

static std::vector<QString>& getSysLabels()
{
#define ADD_SYS_LABEL(L, N)sys_labels.push_back(L);syslabelID2Name[L] = N;

    static std::vector<QString> sys_labels;
    if(sys_labels.empty()){
        ADD_SYS_LABEL("IMPORTANT", "Important");
        ADD_SYS_LABEL("CHAT", "Chat");
        ADD_SYS_LABEL("SENT", "Sent");
        ADD_SYS_LABEL("INBOX", "Inbox");
        ADD_SYS_LABEL("TRASH", "Trash");
        ADD_SYS_LABEL("DRAFT", "Draft");
        ADD_SYS_LABEL("SPAM", "Spam");
        ADD_SYS_LABEL("STARRED", "Starred");
        ADD_SYS_LABEL("UNREAD", "Unread");
        ADD_SYS_LABEL("CATEGORY_PERSONAL", "Personal");
        ADD_SYS_LABEL("CATEGORY_SOCIAL", "Social");
        ADD_SYS_LABEL("CATEGORY_FORUMS", "Forum");
        ADD_SYS_LABEL("CATEGORY_UPDATES", "Updates");
        ADD_SYS_LABEL("CATEGORY_PROMOTIONS", "Promotions");
    }
    return sys_labels;

#undef ADD_SYS_LABEL
}

QString mail_cache::sysLabelId(SysLabel l)
{
    std::vector<QString>& labels_arr = getSysLabels();
    int idx = (int)l;
    if(idx < 0 || idx >= (int)labels_arr.size()){
        qWarning() << "ERROR. Invalid SysLabel index" << idx << labels_arr.size();
        return "";
    }
    
    QString s = getSysLabels()[idx];
    return s;
};

bool mail_cache::GMailSQLiteStorage::loadLabelsFromDb()
{
    QString sql = QString("SELECT label_id, label_name, label_type, label_unread_messages, label_mask FROM %1gmail_label WHERE acc_id=%2 ORDER BY label_id").arg(m_metaPrefix).arg(m_accId);
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
                qWarning() << "ERROR. Failed to create label" << s << m_labels.size();
            }
        }
    }

    return true;
};

bool mail_cache::GMailSQLiteStorage::loadAttachmentsFromDb(MessageData& m) 
{
    QString sql = QString("SELECT att_id, file_name, local_file_name, mime_type, size FROM %1gmail_attachments WHERE msg_id='%2' AND acc_id=%3 ORDER BY att_id")
        .arg(m_metaPrefix)
        .arg(m.id())
		.arg(m_accId);
    QSqlQuery* q = selectQuery(sql);
    if (!q)
        return false;

    while (q->next())
        {
            QString att_id = q->value(0).toString();
            QString filename = q->value(1).toString();
            QString local_filename = q->value(2).toString();
            QString mimetype = q->value(3).toString();      
            quint64 size = q->value(4).toInt();
            AttachmentData::EStatus status = AttachmentData::statusNotDownloaded;
            if (!local_filename.isEmpty()) {
                QString local_path = downloadDir() + "/" + local_filename;
                if (QFile::exists(local_path)) {
                    status = AttachmentData::statusDownloaded;
                }
            }

            att_ptr att = std::make_shared<AttachmentData>(att_id, mimetype, filename, local_filename, size, status);
            m.m_attachments.push_back(att);
        }

    return true;
};

QString mail_cache::GMailSQLiteStorage::findAttachmentFile(att_ptr att)const 
{
    QString rv;
    if (att->status() == AttachmentData::statusDownloaded &&
        !att->localFilename().isEmpty()) 
        {
            QString local_path = downloadDir() + "/" + att->localFilename();
            if (QFile::exists(local_path)) {
                rv = local_path;
            }
        }
    return rv;
};

mail_cache::LabelData* mail_cache::GMailSQLiteStorage::createAndInsertLabel(
                                                                            QString label_id,
                                                                            QString label_name,
                                                                            bool label_is_system,
                                                                            uint64_t unread_messages,
                                                                            int mask_base
                                                                            ) 
{
    label_ptr lb(new LabelData(label_id,
                               label_name,
                               mask_base,
                               label_is_system,
                               unread_messages));

    m_avail_label_base.erase(mask_base);
    m_labels[lb->labelId()] = lb;
    m_maskbase2labels[mask_base] = lb;
    return lb.get();
};

bool mail_cache::GMailSQLiteStorage::updateDbLabel(const labels::LabelResource& lbl)
{
    QString name = lbl.name();
    if(lbl.type().compare("system", Qt::CaseInsensitive) == 0){
        auto i = syslabelID2Name.find(lbl.id());
        if (i != syslabelID2Name.end()) {
            name = i->second;
        }        
    }
    
    QString sql_update;
    sql_update = QString("UPDATE %1gmail_label SET label_name='%2', label_unread_messages=%3 WHERE label_id='%4' AND acc_id='%5'")
        .arg(m_metaPrefix)
        .arg(name)
        .arg(lbl.messagesunread())
        .arg(lbl.id())
		.arg(m_accId);
    return execQuery(sql_update);
};

mail_cache::LabelData* mail_cache::GMailSQLiteStorage::insertDbLabel(const labels::LabelResource& lbl) 
{
    if (m_avail_label_base.empty())
        {
            qWarning() << "ERROR. Exhausted labels masks. New label can not be registered." << m_avail_label_base.size() << lbl.id() << lbl.name();
            return nullptr;
        }

    auto i = m_avail_label_base.begin();
    int mask_base = *i;

    QString sql_insert;
    sql_insert = QString("INSERT INTO  %1gmail_label(acc_id, label_id, label_name, label_type, label_unread_messages,"
                         "label_total_messages, message_list_visibility, label_list_visibility, label_mask) VALUES(%2, ?, ?, ?, ?, ?, ?, ?, ?)")
        .arg(m_metaPrefix)
		.arg(m_accId);
    QSqlQuery* q = prepareQuery(sql_insert);
    if (!q)return nullptr;

    int label_type_as_int = 2;
    if (lbl.type().compare("system", Qt::CaseInsensitive) == 0) {
        label_type_as_int = 1;
    }

    q->addBindValue(lbl.id());
    q->addBindValue(lbl.name());
    q->addBindValue(label_type_as_int);
    q->addBindValue(lbl.messagesunread());
    q->addBindValue(lbl.messagestotal());
    q->addBindValue(lbl.messagelistvisibility());
    q->addBindValue(lbl.labellistvisibility());
    q->addBindValue(mask_base);

    mail_cache::LabelData* rv = nullptr;

    if (q->exec())
        {
            rv = createAndInsertLabel(lbl.id(),
                                      lbl.name(),
                                      (label_type_as_int == 1),
                                      lbl.messagesunread(),
                                      mask_base);
        }
	else {
		qWarning() << "ERROR. Failed to store label" << sql_insert;

	}

    return rv;
};

void mail_cache::GMailSQLiteStorage::insertDbAttachmentData(const mail_cache::MessageData& m)
{
	cache_ptr cache = lock_cache();
	if (!cache) {
		return;
	}

    if (m.m_attachments.size() == 0) {
        qWarning() << "ERROR. Not attachments to store for" << m.id();
        return;
    }

    QString sql_insert;
    sql_insert = QString("INSERT INTO  %1gmail_attachments(att_id, acc_id, msg_id, file_name, mime_type, size)"
                         " VALUES(?, %2, ?, ?, ?, ?)")
        .arg(m_metaPrefix)
		.arg(m_accId);

    for (auto& a : m.m_attachments)
        {
            QSqlQuery* q = prepareQuery(sql_insert);
            if (!q) {
                QString error = m_query->lastError().text();
                qWarning() << "ERROR. Failed to prepare SQL" << sql_insert << error;
                return;
            }

            q->addBindValue(a->attachmentId());
            q->addBindValue(m.id());
            q->addBindValue(a->filename());
            q->addBindValue(a->mimetype());
            q->addBindValue(a->size());

            if (!q->exec()) {
                QString error = m_query->lastError().text();
                qWarning() << "ERROR. Failed to store attachment" << cache->endpoint().apiClient()->userId() << m.id() << a->attachmentId() << error;
                continue;
            }
        }
};

mail_cache::LabelData* mail_cache::GMailSQLiteStorage::insertDbLabel(QString label_id,
                                                                     QString label_name,
                                                                     QString label_type) 
{
    if (m_avail_label_base.empty()) 
        {
            qWarning() << "ERROR. Exhausted labels masks. New label can not be registered." << m_avail_label_base.size() << label_id << label_name;
            return nullptr;
        }

    auto i = m_avail_label_base.begin();
    int mask_base = *i;

    QString sql_insert;
    sql_insert = QString("INSERT INTO  %1gmail_label(acc_id, label_id, label_name, label_type, label_mask) VALUES(%2, ?, ?, ?, ?)")
        .arg(m_metaPrefix)
		.arg(m_accId);
    QSqlQuery* q = prepareQuery(sql_insert);
    if (!q)return nullptr;

    int label_type_as_int = 2;
    if (label_type.compare("system", Qt::CaseInsensitive) == 0) {
        label_type_as_int = 1;
    }

    q->addBindValue(label_id);
    q->addBindValue(label_name);
    q->addBindValue(label_type_as_int);
    q->addBindValue(mask_base);

    mail_cache::LabelData* rv = nullptr;

    if (q->exec())
        {
            rv = createAndInsertLabel(label_id,
                                      label_name,
                                      (label_type_as_int == 1),
                                      0,
                                      mask_base);
        }
	else {
		qWarning() << "ERROR. Failed to store label" << sql_insert << q->lastError().text();
	}

    return rv;
};

std::list<QString> mail_cache::GMailSQLiteStorage::load_db(EDataState state, 
                                                           const std::list<QString>& id_list,
                                                           GMailCacheQueryTask* cr)
{   
	std::list<QString> rv;

	cache_ptr cache = lock_cache();
	if (!cache) {
		return rv;
	}
    
    std::set<QString> db_loaded;
    bool cache_avail = true;
    std::function<bool(const std::list<QString>& lst)> loadSublist = [&](const std::list<QString>& lst) -> bool
        {
            QString comma_ids = slist2commalist_decorated(lst);
            QString sql = QString("SELECT msg_state, msg_id, msg_from, msg_to, msg_cc, msg_bcc, "
                                  "msg_subject, msg_snippet, msg_plain, msg_html, internal_date, msg_labels FROM %1gmail_msg WHERE msg_id IN(%2) AND acc_id=%3")
            .arg(m_metaPrefix)
            .arg(comma_ids)
			.arg(m_accId);
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
                                    cache_avail = cache->mem_insert(md->id(), md);
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

void mail_cache::GMailSQLiteStorage::update_db(EDataState state, CACHE_QUERY_RESULT_LIST<mail_cache::MessageData>& r)
{
	cache_ptr cache = lock_cache();
	if (!cache) {
		return;
	}

    QString sql_update, sql_insert;
    switch (state)
        {
        case EDataState::snippet:
            {
                sql_update = QString("UPDATE %1gmail_msg SET msg_state=?, msg_from=?, msg_to=?, "
                                     "msg_cc=?, msg_bcc=?, msg_subject=?, msg_snippet=?, internal_date=?, msg_labels=? WHERE msg_id=? AND acc_id=?")
                    .arg(m_metaPrefix);
                sql_insert = QString("INSERT INTO %1gmail_msg(msg_state, msg_from, msg_to, msg_cc, msg_bcc, msg_subject, "
                                     "msg_snippet, internal_date, msg_labels, msg_id, acc_id) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")
                    .arg(m_metaPrefix);
            }break;
        case EDataState::body: 
            {
                sql_update = QString("UPDATE %1gmail_msg SET msg_state=?, msg_from=?, msg_to=?, msg_cc=?, msg_bcc=?, "
                                     "msg_subject=?, msg_snippet=?, msg_plain=?, msg_html=?, internal_date=?, msg_labels=? WHERE msg_id=? AND acc_id=?")
                    .arg(m_metaPrefix);
                sql_insert = QString("INSERT INTO %1gmail_msg(msg_state, msg_from, msg_to, msg_cc, msg_bcc, msg_subject, "
                                     "msg_snippet, msg_plain, msg_html, internal_date, msg_labels, msg_id, acc_id) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)")
                    .arg(m_metaPrefix);
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
					q->addBindValue(m_accId);
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
					q->addBindValue(m_accId);
                }break;
                }
            return q->exec();
        };

    int updated_records = 0;
    int inserted_records = 0;

    for(auto& i : r)
        {
            msg_ptr& m = i;
            QSqlQuery* q = prepareQuery(sql_update);
            if (!q)return;
            bool ok = execWithValues(q, m.get());
            if(!ok) 
                {                   
                    qWarning() << "ERROR. SQL update failed" << q->lastError().text() << i->id();
                }
            int rows_updated = q->numRowsAffected();
            if (rows_updated <= 0)
                {
                    q = prepareQuery(sql_insert);
                    if (!q)return;
                    ok = execWithValues(q, m.get());
                    if (!ok)
                        {
                            qWarning() << "ERROR. SQL insert failed" << q->lastError().text() << i->id();
                        }
                    else
                        {
                            rows_updated = q->numRowsAffected();
                            if (rows_updated > 0)
                                inserted_records++;
                            int att_count = m->m_attachments.size();
                            qDebug() << "gmail-db/insert " 
									 << cache->endpoint().apiClient()->userId()
									 << i->id()
                                     << " upd=" << rows_updated
                                     << "/ins=" << inserted_records
                                     << "/size=" << r.size()
                                     << " att=" << att_count;
                            if (att_count > 0) {
                                insertDbAttachmentData(*m.get());
                            }
                        }
                }
            else 
                {
                    updated_records++;
                    int att_count = m->m_attachments.size();
                    qDebug() << "gmail-db/update " 
							 << cache->endpoint().apiClient()->userId()
							 << i->id()
                             << " upd=" << rows_updated
                             << "/ins=" << updated_records
                             << "/size" << r.size()
                             << " att=" << att_count;
                    if (att_count > 0) {
                        insertDbAttachmentData(*m.get());
                    }                    
                }
        }
};

void mail_cache::GMailSQLiteStorage::remove_db(const std::set<QString>& set_of_ids2remove) 
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
            .arg(m_metaPrefix)
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
                    qWarning() << "ERROR. Failed to remove list of records.";
                }
        }
};

bool mail_cache::GMailSQLiteStorage::deleteAttachmentsFromDb(QString msg_id)
{
    QString sql = QString("DELETE FROM %1gmail_attachments WHERE msg_id IN('%2')")
        .arg(m_metaPrefix)
        .arg(msg_id);
    QSqlQuery* q = prepareQuery(sql);
    if (!q)return false;
    if (!q->exec()) return false;
    return true;
};

std::list<mail_cache::acc_uptr> mail_cache::GMailSQLiteStorage::loadAccountsFromDb()
{
	std::list<mail_cache::acc_uptr> rv;
	QString sql = QString("SELECT acc_id, userid FROM %1gmail_account").arg(m_metaPrefix);
	QSqlQuery* q = selectQuery(sql);
	if (!q)
		return rv;

	while (q->next())
        {
            int acc_id = q->value(0).toInt();
            QString userid = q->value(1).toString();
            mail_cache::acc_uptr p(new AccountData(acc_id, userid));
            rv.push_back(std::move(p));
        }
	return rv;
};

bool mail_cache::GMailSQLiteStorage::deleteAccountsFromDb(int accId) 
{
	QString sql = QString("DELETE FROM %1gmail_msg WHERE acc_id = %2")
		.arg(m_metaPrefix)
		.arg(accId);
    bool ok = execQuery(sql);
    if(!ok){
        qWarning() << "ERROR.Failed to delete messages" << sql;
        return false;
    }

    sql = QString("DELETE FROM %1gmail_label WHERE acc_id = %2")
		.arg(m_metaPrefix)
		.arg(accId);
    ok = execQuery(sql);
    if(!ok){
        qWarning() << "ERROR.Failed to delete labels" << sql;
        return false;
    }

    sql = QString("DELETE FROM %1gmail_attachments WHERE acc_id = %2")
		.arg(m_metaPrefix)
		.arg(accId);
    ok = execQuery(sql);
    if(!ok){
        qWarning() << "ERROR.Failed to delete attachments" << sql;
        return false;
    }
    
	sql = QString("DELETE FROM %1gmail_account WHERE acc_id = %2")
		.arg(m_metaPrefix)
		.arg(accId);
    ok = execQuery(sql);
    if(!ok){
        qWarning() << "ERROR.Failed to delete account" << sql;
        return false;
    }    

	return true;
};

bool mail_cache::GMailSQLiteStorage::canRenameAccount(int accId)
{
    QString sql = QString("SELECT count(*) n FROM %1gmail_msg WHERE acc_id = %2"
                          "UNION"
                          "SELECT count(*) n FROM %1gmail_label WHERE acc_id = %2"
                          "UNION"
                          "SELECT count(*) n FROM %1gmail_attachments WHERE acc_id = %2")
        .arg(m_metaPrefix)
        .arg(accId);
    QSqlQuery* q = selectQuery(sql);
    if (!q){
        qWarning() << "ERROR. Failed to prepare SQL" << sql;
        return false;
    }
    while (q->next())
        {
            int num = q->value(0).toInt();
            if(num > 0)
                return false;
        }

    return true;
};

bool mail_cache::GMailSQLiteStorage::updateAccountsDb(int accId, QString userId) 
{
    if(!canRenameAccount(accId)){
        return false;
    }
    
	QString sql = QString("UPDATE %1gmail_account SET userid='%2' WHERE acc_id = %3")
		.arg(m_metaPrefix)
		.arg(userId)
		.arg(accId);
	QSqlQuery* q = prepareQuery(sql);
	if (!q)return false;
	if (!q->exec()) return false;
	return true;
};

int mail_cache::GMailSQLiteStorage::addAccountDb(QString userId) 
{
	int accId = -1;
	QString sql = QString("INSERT INTO %1gmail_account(userid) VALUES(?)").arg(m_metaPrefix);
	QSqlQuery* q = prepareQuery(sql);
	if (!q)return -1;
	q->addBindValue(userId);
	if (q->exec()) {
		accId = q->lastInsertId().toInt();
	}
	return accId;
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

    QString label_name = label_id;
    if (system_label) {
        auto i = syslabelID2Name.find(label_id);
        if (i != syslabelID2Name.end()) {
            label_name = i->second;
        }
    }

    mail_cache::LabelData* rv = insertDbLabel(label_id,
                                              label_name,
                                              system_label ? "system" : "user");
    return rv;
};

std::list<mail_cache::LabelData*> mail_cache::GMailSQLiteStorage::getLabelsInSet(std::set<QString>* in_optional_idset /*= nullptr*/)
{
    std::list<mail_cache::LabelData*> rv;
    for (auto& i : m_labels) {
        bool add_label = true;
        if (in_optional_idset) {
            add_label = (in_optional_idset->find(i.first) != in_optional_idset->end());
        }
        if (add_label) {
            rv.push_back(i.second.get());
        }
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
                        qWarning() << "ERROR. Lost label (pack)" << label_id << m_labels.size();
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
                            qWarning() << "ERROR. Lost label (unpack)" << b << m_maskbase2labels.size();
                            warning_count++;
                        }
                }

            }
        }

    return labels;
};

void mail_cache::GMailSQLiteStorage::update_message_labels_db(QString msg_id, uint64_t flags)
{
    QString sql_update;
    sql_update = QString("UPDATE %1gmail_msg SET msg_labels=%2 WHERE msg_id='%3' AND acc_id='%4'")
        .arg(m_metaPrefix)
        .arg(flags)
        .arg(msg_id)
		.arg(m_accId);
    execQuery(sql_update);
};

void mail_cache::GMailSQLiteStorage::update_attachment_local_file_db(googleQt::mail_cache::msg_ptr m, 
                                                                     googleQt::mail_cache::att_ptr a, 
                                                                     QString file_name)
{
    QString sql_update;
    sql_update = QString("UPDATE %1gmail_attachments SET local_file_name='%2' WHERE msg_id='%3' AND att_id='%4' AND acc_id=%5")
        .arg(m_metaPrefix)
        .arg(file_name)
        .arg(m->id())
        .arg(a->attachmentId())
		.arg(m_accId);
    bool ok = execQuery(sql_update);
    if (ok) {
        a->m_local_filename = file_name;
        a->m_status = AttachmentData::statusDownloaded;
    }
};

mail_cache::msg_ptr mail_cache::GMailSQLiteStorage::loadMessageFromDB(QSqlQuery* q)
{
	mail_cache::msg_ptr md;

	cache_ptr cache = lock_cache();
	if (!cache) {
		return md;
	}

    int agg_state = q->value(0).toInt();
    if (agg_state < 1 || agg_state > 3)
        {
            qWarning() << "ERROR. Invalid DB state" << agg_state << q->value(1).toString();
            return nullptr;
        }
    QString msg_id = q->value(1).toString();
    if (msg_id.isEmpty())
        {
            qWarning() << "ERROR. Invalid(empty) DBID";
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

    md = std::shared_ptr<MessageData>(new MessageData(cache->endpoint().apiClient()->userId(),
													  agg_state, 
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
            qWarning() << "ERROR. Expected internal query";
            return false;
        }
    
    if(!m_query->prepare(sql))
        {
            QString error = m_query->lastError().text();
            qWarning() << "ERROR. Failed to prepare sql query" 
                       << error 
                       << sql;
            return false;
        };    
    if(!m_query->exec(sql))
        {
            QString error = m_query->lastError().text();
            qWarning() << "ERROR. Failed to execute query" 
                       << error 
                       << sql;
            return false;
        };

    return true;
};

QSqlQuery* mail_cache::GMailSQLiteStorage::prepareQuery(QString sql)
{
    if (!m_query)
        {
            qWarning() << "ERROR. Expected internal query";
            return nullptr;
        }
    if (!m_query->prepare(sql))
        {
            QString error = m_query->lastError().text();
            qWarning() << "ERROR. Failed to prepare sql query" << error << sql;
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
            qWarning() << "ERROR. Failed to execute query" << error << sql;
            return nullptr;
        };
    return q;
};

