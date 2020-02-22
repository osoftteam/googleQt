#include "GmailRoutes.h"
#include <QSqlError>
#include <QDebug>
#include <ctime>
#include <QDir>
#include "Endpoint.h"
#include "GmailCacheRoutes.h"
#include "gcontact/GcontactCache.h"

#define CONFIG_VERSION "version"

using namespace googleQt;
///MessagesReceiver
mail_cache::MessagesReceiver::MessagesReceiver(GmailRoutes& r, EDataState f) 
    :m_r(r), 
     m_msg_format(f) 
{

};

GoogleTask<messages::MessageResource>* mail_cache::MessagesReceiver::routeRequest(QString message_id)
{   
    gmail::IdArg arg(m_r.endpoint()->client()->userId(), message_id);
    if (m_msg_format == EDataState::snippet)
        {
            arg.setFormat("metadata");
            arg.headers().push_back("Subject");
            arg.headers().push_back("From");
            arg.headers().push_back("To");
            arg.headers().push_back("Cc");
            arg.headers().push_back("Bcc");
            arg.headers().push_back("References");
        }
    else if (m_msg_format == EDataState::body)
        {
        
        }

#ifdef API_QT_AUTOTEST
    ApiAutotest::INSTANCE().addId("messages::MessageResource", message_id);
#endif
    return m_r.getMessages()->get_Async(arg);
}

///ThreadsReceiver
mail_cache::ThreadsReceiver::ThreadsReceiver(GmailRoutes& r)
    :m_r(r)
{
};

GoogleTask<threads::ThreadResource>* mail_cache::ThreadsReceiver::routeRequest(QString thread_id)
{
    gmail::IdArg arg(m_r.endpoint()->client()->userId(), thread_id);
    arg.setFormat("minimal");

#ifdef API_QT_AUTOTEST
    ApiAutotest::INSTANCE().addId("threads::ThreadResource", thread_id);
    for (int i = 0; i < 5; i++) {
        ApiAutotest::INSTANCE().addMemberValue("messages::MessageResource", "m_threadId", thread_id);
    }
#endif
    return m_r.getThreads()->get_Async(arg);
}

///GMailCache
mail_cache::GMailCache::GMailCache(/*ApiEndpoint& ept*/Endpoint& ept)
    :GoogleCache<MessageData, GMailCacheQueryTask>(ept), m_endpoint(ept)
{

};

mail_cache::mdata_result mail_cache::GMailCache::topCacheData(int number2load, uint64_t labelFilter)
{
    mail_cache::mdata_result r(new CacheDataResult<MessageData>());
    
    int count = 0;
    for (auto& i : m_ord) {
        auto m = i;
        if (m->hasAllLabels(labelFilter)) {
            r->result_map[m->id()] = m;
            r->result_list.push_back(m);
            if (number2load > 0) {
                if (++count >= number2load)
                    break;
            }
        }
    }
    return r;
}

///GThreadCache
mail_cache::GThreadCache::GThreadCache(ApiEndpoint& ept)
    :GoogleCache<ThreadData, GThreadCacheQueryTask>(ept)
{

};

mail_cache::tdata_result mail_cache::GThreadCache::topCacheData(int number2load, uint64_t labelFilter)
{
    mail_cache::tdata_result r(new CacheDataResult<ThreadData>());
    
    int count = 0;
    for (auto& i : m_ord) {
        auto t = i;
        if (t->hasAllLabels(labelFilter)) {
            r->result_map[t->id()] = t;
            r->result_list.push_back(t);
            if (number2load > 0) {
                if (++count >= number2load)
                    break;
            }
        }
    }
    return r;
}

void mail_cache::GThreadCache::reorder_data_on_completed_fetch(const CACHE_LIST<ThreadData>& from_cloud)
{
    std::sort(m_ord.begin(), m_ord.end(), [&](mail_cache::thread_ptr t1, mail_cache::thread_ptr t2) 
    {
        bool rv = (t1->internalDate() > t2->internalDate());
        return rv;
    });

#ifdef API_QT_AUTOTEST
    int idx = 1;
    ApiAutotest::INSTANCE() << QString("---after reorder_data_on_completed_fetch --/%1").arg(m_ord.size());
    for (auto t : from_cloud) {
        ApiAutotest::INSTANCE() << QString("%1 %2 %3").arg(idx).arg(t->id()).arg(t->internalDate());
        idx++;
    }
    idx = 0;
    ApiAutotest::INSTANCE() << QString("^^^ above came from cloud /%1").arg(from_cloud.size());
    for (auto t : m_ord) {
        ApiAutotest::INSTANCE() << QString("%1 %2 %3").arg(idx).arg(t->id()).arg(t->internalDate());
        idx++;
    }
#else
    Q_UNUSED(from_cloud);
#endif //API_QT_AUTOTEST
};

void mail_cache::GThreadCache::verifyData()
{
    std::set<QString> ids2delete;
    for (auto& i : m_ord) {
        auto t = i;
        if (!t->head()) {
            ids2delete.insert(t->id());
        }
    }

    if (!ids2delete.empty()) {
        qWarning() << "Cleaning empty threads:" << ids2delete.size();
        for (auto i : ids2delete) {
            qWarning() << "empty-thread2clean" << i;
        }

        //persistent_clear(ids2delete);
		qWarning() << "Cleaning empty threads:" << ids2delete.size() << " DISABLED-by-ykh";
    }
};

#ifdef API_QT_AUTOTEST
int g__msg_alloc_counter = 0;
int g__thread_alloc_counter = 0;
#endif

///MessageData
mail_cache::MessageData::MessageData(int accId,
                                     QString id,
                                     QString thread_id,
                                     QString from,
                                     QString to,
                                     QString cc,
                                     QString bcc,
                                     QString subject,
                                     QString snippet,
                                     qlonglong internalDate,
                                     uint64_t labels,
                                     QString references)
    :CacheData(EDataState::snippet, id),
    m_accountId(accId),
    m_thread_id(thread_id),
    m_from(from),
    m_to(to),
    m_cc(cc),
    m_bcc(bcc),
    m_subject(subject),
    m_snippet(snippet),
    m_internalDate(internalDate),
    m_references(references)
{
    m_labels = labels;
#ifdef API_QT_AUTOTEST
	g__msg_alloc_counter++;
#endif
};

mail_cache::MessageData::MessageData(int accId,
                                     QString id,
                                     QString thread_id,
                                     QString from,
                                     QString to,
                                     QString cc,
                                     QString bcc,
                                     QString subject,
                                     QString snippet,
                                     QString plain,
                                     QString html,
                                     qlonglong internalDate,
                                     uint64_t labels,
                                     QString references)
:CacheData(EDataState::body, id),
    m_accountId(accId),
    m_thread_id(thread_id),
    m_from(from),
    m_to(to),
    m_cc(cc),
    m_bcc(bcc),
    m_subject(subject),
    m_snippet(snippet),
    m_plain(plain),
    m_html(html),
    m_internalDate(internalDate),
    m_references(references)
{
    m_labels = labels;
#ifdef API_QT_AUTOTEST
	g__msg_alloc_counter++;
#endif
};

mail_cache::MessageData::MessageData(int accId,
                                     QString thread_id,
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
                                     uint64_t labels,
                                     QString references)
:CacheData(EDataState::body, id),
m_accountId(accId),
    m_thread_id(thread_id),
    m_from(from),
    m_to(to),
    m_cc(cc),
    m_bcc(bcc),
    m_subject(subject),
    m_snippet(snippet),
    m_plain(plain),
    m_html(html),
    m_internalDate(internalDate),
    m_references(references)
{
    m_flags.agg_state = static_cast<unsigned int>(agg_state);
    m_labels = labels;
#ifdef API_QT_AUTOTEST
	g__msg_alloc_counter++;
#endif
};

mail_cache::MessageData::MessageData(int accId,
    QString id,
    QString thread_id,
    uint64_t labels)
    : CacheData(EDataState::labels, id),
    m_accountId(accId),
    m_thread_id(thread_id)
{
    m_flags.agg_state = 0;
    m_labels = labels;
#ifdef API_QT_AUTOTEST
	g__msg_alloc_counter++;
#endif
};

mail_cache::MessageData::~MessageData()
{
#ifdef API_QT_AUTOTEST
	g__msg_alloc_counter--;
#endif
};

void mail_cache::MessageData::updateSnippet(QString from,
                                            QString to,
                                            QString cc,
                                            QString bcc,
                                            QString subject,
                                            QString snippet,
                                            uint64_t labels,
                                            QString references)
{
    m_flags.agg_state |= static_cast<int>(EDataState::snippet);
    m_from = from;
    m_to = to;
    m_cc = cc;
    m_bcc = bcc;
    m_subject = subject;
    m_snippet = snippet;
    m_labels = labels;
    m_references = references;
};

void mail_cache::MessageData::updateBody(QString plain, QString html)
{
    m_flags.agg_state |= static_cast<int>(EDataState::body);
    m_plain = plain;
    m_html = html;
};

void mail_cache::MessageData::updateLabels(uint64_t labels) 
{
    m_labels = labels;
};

bool mail_cache::MessageData::hasReservedSysLabel(SysLabel l)const 
{
    auto lmask = reservedSysLabelMask(l);
    return hasLabel(lmask);
};

bool mail_cache::MessageData::hasLabel(uint64_t data)const
{
    bool rv = true;
    if (data != 0) {
        rv = (data & m_labels) != 0;
    }
    return rv;
};

bool mail_cache::MessageData::hasAllLabels(uint64_t data)const
{
    bool rv = true;
    if (data != 0) {
        uint64_t f = data & m_labels;
        rv = (data == f);
    }
    return rv;
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
                    m_flags.agg_state |= static_cast<int>(EDataState::snippet);
                }
        }

    if(!isLoaded(EDataState::body))
        {
            if(md->isLoaded(EDataState::body))
                {
                    m_plain = md->plain();
                    m_html = md->html();
                    m_flags.agg_state |= static_cast<int>(EDataState::body);
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

bool mail_cache::LabelData::isPersonalCategory()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::CATEGORY_PERSONAL), Qt::CaseInsensitive) == 0);
    return rv;
};


bool mail_cache::LabelData::isSocialCategory()const
{
    bool rv = (m_label_id.compare(sysLabelId(SysLabel::CATEGORY_SOCIAL), Qt::CaseInsensitive) == 0);
    return rv;
};

mail_cache::ThreadData::ThreadData(QString id,
                                   quint64 history_id,
                                   int      messages_count,
                                   QString snippet,
                                   uint64_t lbmap):
    CacheDataWithHistory(id, history_id),
    m_messages_count(messages_count),
    m_snippet(snippet),
    m_labels(lbmap)
{
#ifdef API_QT_AUTOTEST	
	g__thread_alloc_counter++;
#endif
};

mail_cache::ThreadData::~ThreadData() 
{
#ifdef API_QT_AUTOTEST
	g__thread_alloc_counter--;
#endif
};

void mail_cache::ThreadData::merge(CacheData* ) 
{
    qWarning() << "NA - mail_cache::ThreadData::merge";
};

void mail_cache::ThreadData::add_msg(msg_ptr m) 
{
#ifdef _DEBUG
    auto i = m_mmap.find(m->id());
    if (i != m_mmap.end()) {
        qWarning() << "duplicate message in thread" << m->id();
    }
#endif

    m_messages.push_back(m);
    m_mmap[m->id()] = m;
    m_labels |= m->labelsBitMap();
    
    if (m_head) {
        if (m->internalDate() > m_head->internalDate()) {
            m_head = m;
        }
    }
    else {
        m_head = m;
    }
};

void mail_cache::ThreadData::rebuildLabelsMap() 
{
    m_labels = 0;
    m_limbo_labels = 0;
    for (auto& m : m_messages) {
        m_labels |= m->labelsBitMap();
    }
};

void mail_cache::ThreadData::setupLimboLabels(const label_list& labels2add)
{
    m_limbo_labels = 0;
    for (auto& lb : labels2add) {
        m_limbo_labels |= lb->labelMask();
    }
};

mail_cache::msg_ptr mail_cache::ThreadData::findMessage(QString id)
{
    mail_cache::msg_ptr r;
    auto i = m_mmap.find(id);
    if (i != m_mmap.end()) {
        r = i->second;
    }
    return r;
};

void mail_cache::ThreadData::resortMessages() 
{
    std::sort(m_messages.begin(), m_messages.end(), [&](mail_cache::msg_ptr m1, mail_cache::msg_ptr m2)
    {
        bool rv = (m1->internalDate() > m2->internalDate());
        return rv;
    });

};

qlonglong mail_cache::ThreadData::internalDate()const
{
    if (m_head) {
        return m_head->internalDate();
    }
    return 0;
};

bool mail_cache::ThreadData::hasLabel(uint64_t data)const
{
    bool rv = true;
    if (data != 0) {
        rv = (data & m_labels) != 0;
    }
    return rv;
};

bool mail_cache::ThreadData::hasLimboLabel(uint64_t data)const 
{
    bool rv = false;
    if (data != 0) {
        rv = (data & m_limbo_labels) != 0;
    }
    return rv;
};

bool mail_cache::ThreadData::hasAllLabels(uint64_t data)const
{
    bool rv = true;
    if (data != 0) {
        uint64_t f = data & m_labels;
        rv = (data == f);
    }
    return rv;
};

///QueryData
mail_cache::QueryData::QueryData(int dbid, QString qstr, QString lbid):
    m_db_id(dbid), m_q(qstr), m_labelid(lbid)
{

};

QString mail_cache::QueryData::format_qhash(QString qstr, QString lblid) 
{
  QString rv = qstr + ":" + lblid;
  return rv;
};

///GMailCacheQueryTask
mail_cache::GMailCacheQueryTask::GMailCacheQueryTask(EDataState state, 
													 ApiClient* cl,
                                                     googleQt::mail_cache::GmailCacheRoutes& r,
                                                     GMailCache* c)
    :CacheQueryTask<MessageData>(state, cl, c), m_r(r)
{

};

void mail_cache::GMailCacheQueryTask::fetchFromCloud_Async(const STRING_LIST& id_list)
{
    if (id_list.empty())
        return;
    
    auto p = progressNotifier();
    if (p) {
        p->setMaximum(id_list.size(), "requested [msg-id]->G->[messages]");
    }

    auto par_runner = m_r.getUserBatchMessages_Async(m_completed->state, id_list);
    
    connect(par_runner, &EndpointRunnable::finished, [=]()
            {
                RESULT_LIST<messages::MessageResource>&& res = par_runner->detachResult();
                for (auto& m : res)
                    {
                        fetchMessage(m.get());                        
                    }
                auto s = m_r.storage();
                if (s) {
                    s->updateMessagesDiagnostic(1, m_completed->result_list.size());
                }
                par_runner->disposeLater();
                notifyFetchCompletedWithMergeRequest(m_completed->result_list);             
            }); 
};

void mail_cache::GMailCacheQueryTask::loadHeaders(messages::MessageResource* m,
                                                  QString& from,
                                                  QString& to,
                                                  QString& cc,
                                                  QString& bcc,
                                                  QString& subject,
                                                  QString& references)
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
            else if (h.name().compare("References", Qt::CaseInsensitive) == 0)
            {
                references = h.value();
            }
        }
};

void mail_cache::GMailCacheQueryTask::loadLabels(messages::MessageResource* m, uint64_t& f)
{
    auto& labels = m->labelids();
    if(labels.size() > 0){
        auto storage = m_r.storage();
        if (storage) {
            f = storage->packLabels(labels);
        }
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
    if (!m_r.storage()) {
        qWarning() << "expected storage";
        return;
    }

    msg_ptr md;
    
    switch (m_completed->state)
        {
        case googleQt::EDataState::labels:
            qWarning() << "Unexpected snippet state in message fetch";
        break;
        case googleQt::EDataState::snippet:
            {
                uint64_t labels;
                loadLabels(m, labels);
                QString from, to, cc, bcc, subject, references;
                loadHeaders(m, from, to, cc, bcc, subject, references);
                md.reset(new MessageData(m_r.storage()->currentAccountId(),
                                           m->id(),
                                           m->threadid(),
                                           from, 
                                           to, 
                                           cc,
                                           bcc,
                                           subject, 
                                           m->snippet(), 
                                           m->internaldate(),
                                           labels,
                                           references));
                //snipped - there will be no attachments here and no body..
                add_result(md, true);
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
                        QString from, to, cc, bcc, subject, references;
                        loadHeaders(m, from, to, cc, bcc, subject, references);
                        md.reset(new MessageData(m_r.storage()->currentAccountId(),
                                            m->id(),
                                            m->threadid(),
                                            from, 
                                            to, 
                                            cc, 
                                            bcc, 
                                            subject, 
                                            m->snippet(), 
                                            m->internaldate(),
                                            labels, 
                                            references));
                        add_result(md, true);
                        loadAttachments(m, md->m_attachments);
                        md->updateBody(plain_text, html_text);
                    }
                else
                    {
                        md = i->second;
                        if (!md->isLoaded(googleQt::EDataState::snippet)) 
                            {
                                uint64_t labels;
                                loadLabels(m, labels);
                                QString from, to, cc, bcc, subject, references;
                                loadHeaders(m, from, to, cc, bcc, subject, references);
                                md->updateSnippet(from, to, cc, bcc, subject, m->snippet(), labels, references);
                            }
                        loadAttachments(m, md->m_attachments);
                        md->updateBody(plain_text, html_text);
                    }
            }break;//body
        }


    if(md){
        auto t = m_r.tcache()->mem_object(md->threadId());
        if(t){
            auto m2 = t->findMessage(md->id());
            if (!m2) {
                t->add_msg(md);
            }
        }
        else{
            qWarning() << "failed to locate thread for message"
                       << md->id()
                       << md->threadId();
        }
    }    
};

static bool compare_internalDate(mail_cache::msg_ptr& f,
                                 mail_cache::msg_ptr& s)
{
    return (f->internalDate() > s->internalDate());
};

mail_cache::mdata_result mail_cache::GMailCacheQueryTask::waitForResultAndRelease()
{
    if (!isFinished())
        {
            m_in_wait_loop = true;
            waitUntillFinishedOrCancelled();
        }
    //m_completed->result_list.sort(compare_internalDate);
    std::sort(m_completed->result_list.begin(), m_completed->result_list.end(), compare_internalDate);
    return std::move(m_completed);
};

///GThreadCacheSyncTask
mail_cache::GThreadCacheQueryTask::GThreadCacheQueryTask(
                                                         googleQt::mail_cache::GmailCacheRoutes& r,
                                                         GThreadCache* c,
                                                         query_ptr q)
    :CacheQueryTask<ThreadData>(EDataState::snippet, r.endpoint().apiClient(), c),
    m_r(r), m_q(q)
{

};

void mail_cache::GThreadCacheQueryTask::fetchFromCloud_Async(const STRING_LIST& id_list)
{
    if (id_list.empty())
        return;

    auto p = progressNotifier();
    if (p) {
        p->setMaximum(id_list.size(), "requested [thread-id]->G->[threads]");
    }

    std::unique_ptr<mail_cache::ThreadsReceiver> tr(new mail_cache::ThreadsReceiver(m_r.mroutes()));
    auto par_runner = new ConcurrentValueRunner<QString,
        mail_cache::ThreadsReceiver,
        threads::ThreadResource>(id_list, std::move(tr), m_client.get());
    par_runner->run();

    connect(par_runner, &EndpointRunnable::finished, [=]()
    {
        STRING_LIST msg_list2resolve;

        RESULT_LIST<threads::ThreadResource>&& res = par_runner->detachResult();
        for (auto& t : res)
        {
            auto td = m_cache->mem_object(t->id());
            if (td) {
                bool is_updated = false;
                ///we have thread in cache, check on new mesages
                auto mlst = t->messages();
                if (!mlst.empty()) {
                    for (const auto& m : mlst) {
                        auto md = td->findMessage(m.id());
                        if (!md) {
                            msg_list2resolve.push_back(m.id());
                            is_updated = true;
                        }
                    }
                }

                if (is_updated) {
                    m_updated_threads.push_back(td);
                }
            }
            else{
                ///new thread, get all mesages
                thread_ptr td(new ThreadData(t->id(), t->historyid(), t->messages().size(), t->snipped(), 0));
                m_cache->mem_insert(t->id(), td);

                auto mlst = t->messages();
                if (!mlst.empty()) {
                    for (const auto& m : mlst) {
                        msg_list2resolve.push_back(m.id());
                    }
                }
                m_new_threads.push_back(td);
            }
        }
        
        auto mfetcher = m_r.newMessageResultFetcher(EDataState::snippet);
        if (p) {
            mfetcher->delegateProgressNotifier(p);
        }
        auto r = m_r.getCacheMessages_Async(EDataState::snippet, msg_list2resolve, mfetcher);
        r->then([=](std::unique_ptr<googleQt::CacheDataResult<googleQt::mail_cache::MessageData>> mr)
        {
            if (!m_updated_threads.empty()) {
                m_cache->update_persistent_storage(EDataState::snippet, m_updated_threads);
                for (auto t : m_updated_threads) {
                    if (t->head()) {
                        add_result(t, true);
                    }
                }
            }
            if (!m_new_threads.empty()) {
                m_cache->insert_persistent_storage(EDataState::snippet, m_new_threads);
                for (auto t : m_new_threads) {
                    if (t->head()) {
                        add_result(t, true);
                    }
                }
            }
            if (!m_completed->from_cloud.empty()) {
                m_cache->reorder_data_on_completed_fetch(m_completed->from_cloud);
            }

            if (!mr->from_cloud.empty()) {
                for (auto& m : mr->from_cloud) {
                    auto t = m_r.tcache()->mem_object(m->threadId());
                    if (t) {
                        t->resortMessages();
                    }
                }
            }

            notifyOnCompletedFromCache();
        },
            [=](std::unique_ptr<GoogleException> ex)
        {
            failed_callback(std::move(ex));
        });

        par_runner->disposeLater();
    });
};

mail_cache::tdata_result mail_cache::GThreadCacheQueryTask::waitForResultAndRelease()
{
    if (!isFinished())
    {
        m_in_wait_loop = true;
        waitUntillFinishedOrCancelled();
    }
    return std::move(m_completed);
};

void mail_cache::GThreadCacheQueryTask::notifyOnCompletedFromCache()
{
    if (m_q && m_q->hasNewUnsavedThreads()) {
        m_r.storage()->qstorage()->insert_db_threads(m_q);
    }

    CacheQueryTask<ThreadData>::notifyOnCompletedFromCache();
};

///GMailSQLiteStorage
mail_cache::GMailSQLiteStorage::GMailSQLiteStorage(GMailCache* mc,
	GThreadCache* tc/*,
	gcontact::GContactCacheBase* cc*/)
{
    m_msg_cache = mc;
    m_thread_cache = tc;
    //m_contact_cache = cc;
};



int mail_cache::GMailSQLiteStorage::findAccount(QString userId) 
{
    int accId = -1;
    auto i = m_user2acc.find(userId);
    if (i != m_user2acc.end()) {
        accId = i->second->accountId();
    }
    return accId;
};

QString mail_cache::GMailSQLiteStorage::findUser(int accId)
{
    QString userId;
    auto i = m_id2acc.find(accId);
    if (i != m_id2acc.end()) {
        userId = i->second->userId();
    }
    return userId;
};

mail_cache::acc_ptr mail_cache::GMailSQLiteStorage::currentAccount()
{
    mail_cache::acc_ptr rv = nullptr;
    auto i = m_id2acc.find(m_accId);
    if (i != m_id2acc.end()) {
        rv = i->second;
    }
    return rv;
};

bool mail_cache::GMailSQLiteStorage::ensureMailTables() 
{
    /// generic config ///
    QString sql_config = QString("CREATE TABLE IF NOT EXISTS %1config(config_name TEXT, config_value TEXT)").arg(m_metaPrefix);
    if (!execQuery(sql_config))
        return false;

    if(!reloadDbConfig()){
        return false;
    }

    /// accounts ///
    QString sql_accounts = QString("CREATE TABLE IF NOT EXISTS %1gmail_account(acc_id INTEGER PRIMARY KEY, userid TEXT NOT NULL COLLATE NOCASE, msg_cloud_batch_queried INTEGER, msg_cloud_email_loaded INTEGER)").arg(m_metaPrefix);
    if (!execQuery(sql_accounts))
        return false;

    if (!execQuery(QString("CREATE UNIQUE INDEX IF NOT EXISTS %1acc_userid_idx ON %1gmail_account(userid)").arg(m_metaPrefix)))
        return false;


    /// messages ///
    QString sql_messages = QString("CREATE TABLE IF NOT EXISTS %1gmail_msg(acc_id INTEGER NOT NULL, thread_id TEXT NOT NULL, msg_id TEXT NOT NULL, msg_from TEXT, "
        "msg_to TEXT, msg_cc TEXT, msg_bcc TEXT, msg_subject TEXT, msg_snippet TEXT, msg_plain TEXT, "
        "msg_html TEXT, internal_date INTEGER, msg_state INTEGER, msg_cache_lock INTEGER, msg_labels INTEGER, msg_references TEXT)").arg(m_metaPrefix);
    if (!execQuery(sql_messages))
        return false;

    if (!execQuery(QString("CREATE UNIQUE INDEX IF NOT EXISTS %1msg_accid_idx ON %1gmail_msg(acc_id, msg_id)").arg(m_metaPrefix)))
        return false;

	//ykh+1
	if (!execQuery(QString("CREATE INDEX IF NOT EXISTS %1msg_thread_idx ON %1gmail_msg(thread_id)").arg(m_metaPrefix)))
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

    //// threads ///
    QString sql_threads = QString("CREATE TABLE IF NOT EXISTS %1gmail_thread(acc_id INTEGER NOT NULL, thread_id TEXT NOT NULL, history_id INTEGER "
        " ,snippet TEXT, messages_count INTEGER, t_labels INTEGER, internal_date INTEGER)")
        .arg(m_metaPrefix);
    if (!execQuery(sql_threads))
        return false;

    if (!execQuery(QString("CREATE UNIQUE INDEX IF NOT EXISTS %1thread_accid_idx ON %1gmail_thread(acc_id, thread_id)").arg(m_metaPrefix)))
        return false;

    /// queries ///
    QString sql_q = QString("CREATE TABLE IF NOT EXISTS %1gmail_q(q_id INTEGER PRIMARY KEY, acc_id INTEGER NOT NULL, q_query TEXT, labelid TEXT)")
        .arg(m_metaPrefix);
    if (!execQuery(sql_q))
        return false;

    if (!execQuery(QString("CREATE INDEX IF NOT EXISTS %1gmail_q_query_idx ON %1gmail_q(acc_id, q_query, labelid)").arg(m_metaPrefix)))
        return false;

    /// query-results ///
    QString sql_qres = QString("CREATE TABLE IF NOT EXISTS %1gmail_qres(acc_id INTEGER NOT NULL, q_id INTEGER NOT NULL, thread_id TEXT NOT NULL)")
        .arg(m_metaPrefix);
    if (!execQuery(sql_qres))
        return false;

    if (!execQuery(QString("CREATE INDEX IF NOT EXISTS %1gmail_qres_accid_idx ON %1gmail_qres(acc_id, q_id)").arg(m_metaPrefix)))
        return false;

    if (!execQuery(QString("CREATE UNIQUE INDEX IF NOT EXISTS %1gmail_qres_u_idx ON %1gmail_qres(acc_id, q_id, thread_id)").arg(m_metaPrefix)))
        return false;

    return true;
};

bool mail_cache::GMailSQLiteStorage::init_db(QString dbPath, 
                                             QString downloadPath,
                                             QString contactCachePath,
                                             QString dbName, 
                                             QString db_meta_prefix)
{
    m_accId = -1;
	if (!m_msg_cache) {
		return false;
	}	

    if (!m_thread_cache) {
        return false;
    }    
    
    m_mstorage.reset(new GMessagesStorage(this));
    m_tstorage.reset(new GThreadsStorage(this));
    m_qstorage.reset(new GQueryStorage(m_tstorage.get(), m_mstorage.get()));

    QString userId = m_msg_cache->endpoint().apiClient()->userId();
    if (userId.isEmpty()) {
        qWarning() << "ERROR. Expected userid (email) for gmail cache";
        return false;
    }

    m_dbPath = dbPath;
    m_downloadDir = downloadPath;
    m_contactCacheDir = contactCachePath;
    m_dbName = dbName;
    m_metaPrefix = db_meta_prefix;

    QDir dest_dir;
    if (!dest_dir.exists(m_downloadDir)) {
        if (!dest_dir.mkpath(m_downloadDir)) {
            qWarning() << "ERROR. Failed to create directory" << m_downloadDir;
            return false;
        };
    }

    if (!dest_dir.exists(m_contactCacheDir)) {
        if (!dest_dir.mkpath(m_contactCacheDir)) {
            qWarning() << "ERROR. Failed to create directory" << m_contactCacheDir;
            return false;
        };
    }

    m_initialized = false;  

	m_gmail_db = QSqlDatabase::addDatabase("QSQLITE", dbName);
	m_gmail_db.setDatabaseName(dbPath);
    if (!m_gmail_db.open()) {
        qWarning() << "ERROR. Failed to connect" << dbName << dbPath;
        return false;
    }

    m_query.reset(new QSqlQuery(m_gmail_db));
	m_contact_query.reset(new QSqlQuery(m_gmail_db));

    if (!ensureMailTables()) {
        qWarning() << "ERROR. Failed to create GMail cache tables" << dbName << dbPath;
        return false;
    }

    //auto cc = m_contact_cache.lock();
	auto cc = m_msg_cache->endpoint().client()->contacts_cache();
    if (!cc) {
        qWarning() << "ERROR. Expected GContact cache. Failed to init Gcontact cache tables" << dbName << dbPath;
        return false;
    }


    if (!cc->ensureContactTables()) {
        qWarning() << "ERROR. Failed to create GContacts cache tables" << dbName << dbPath;
        return false;
    }

    /// locate accountID
    reloadDbAccounts();
    m_accId = findAccount(userId);
    if(m_accId == -1)
        {
            QString sql = QString("INSERT INTO %1gmail_account(userid) VALUES(?)").arg(m_metaPrefix);
            QSqlQuery* q = prepareQuery(sql);
            if (!q)return false;
            q->addBindValue(userId);
            if (q->exec()) {
                m_accId = q->lastInsertId().toInt();
                reloadDbAccounts();
            }
        }
    
    m_acc_labels.clear();
    m_maskbase2label.clear();
    m_maskbase2label.reserve(64);       

    for (int i = 0; i < 64; i++) {
        m_avail_label_base.insert(i);
        m_maskbase2label.push_back(nullptr);
    }

    if (!loadLabelsFromDb()){
        qWarning() << "ERROR. Failed to load labels from DB";
        return false;
    }

    if (!m_tstorage->loadThreadsFromDb()) {
        qWarning() << "ERROR. Failed to load threads from DB";
        return false;
    }

    if (!m_mstorage->loadMessagesFromDb()) {
        qWarning() << "ERROR. Failed to load messages from DB";
        return false;
    }   

    if (!m_qstorage->loadQueriesFromDb()) {
        qWarning() << "ERROR. Failed to load queries from DB";
        return false;
    }

    if(!cc->loadContactsFromDb()){
        qWarning() << "ERROR. Failed to load contacts from DB";
        return false;
    };

	m_msg_cache->setupLocalStorage(m_mstorage.get());
	m_thread_cache->setupLocalStorage(m_tstorage.get());
    
    m_tstorage->verifyThreads();

    m_initialized = true;
    return m_initialized;
};

void mail_cache::GMailSQLiteStorage::close_db() 
{
    if (m_gmail_db.isOpen()) {
		auto name = m_gmail_db.connectionName();
		m_gmail_db.close();
		QSqlDatabase::removeDatabase(name);
    }
    m_initialized = false;
};


static std::map<QString, QString> syslabelID2Name;
static std::map<mail_cache::SysLabel, QString> syslabel2Name;
static std::map<QString, mail_cache::SysLabel> reservedLabelID2SysLabel;

static std::vector<QString>& getSysLabels()
{
#define ADD_SYS_LABEL(L, N)sys_labels.push_back(#L);syslabelID2Name[#L] = N;syslabel2Name[mail_cache::SysLabel::L] = N;reservedLabelID2SysLabel[#L] = mail_cache::SysLabel::L;

    static std::vector<QString> sys_labels;
    if(sys_labels.empty()){
        ADD_SYS_LABEL(IMPORTANT, "Important");        
        ADD_SYS_LABEL(CHAT, "Chat");
        ADD_SYS_LABEL(SENT, "Sent");
        ADD_SYS_LABEL(INBOX, "Inbox");
        ADD_SYS_LABEL(TRASH, "Trash");
        ADD_SYS_LABEL(DRAFT, "Draft");
        ADD_SYS_LABEL(SPAM, "Spam");
        ADD_SYS_LABEL(STARRED, "Starred");
        ADD_SYS_LABEL(UNREAD, "Unread");
        ADD_SYS_LABEL(CATEGORY_PERSONAL, "Personal");
        ADD_SYS_LABEL(CATEGORY_SOCIAL, "Social");
        ADD_SYS_LABEL(CATEGORY_FORUMS, "Forum");
        ADD_SYS_LABEL(CATEGORY_UPDATES, "Updates");
        ADD_SYS_LABEL(CATEGORY_PROMOTIONS, "Promotions");        
    }
    return sys_labels;

#undef ADD_SYS_LABEL
}

uint64_t mail_cache::reservedSysLabelMask(SysLabel l) 
{
    std::vector<QString>& labels_arr = getSysLabels();
    int idx = (int)l;
    if (idx < 0 || idx >= static_cast<int>(labels_arr.size())) {
        qWarning() << "ERROR. Invalid SysLabel index" << idx << labels_arr.size();
        return 0;
    }
    static uint64_t theone = 1;
    uint64_t label_mask = (theone << idx);
    return label_mask;
};

QString mail_cache::sysLabelId(SysLabel l)
{
    std::vector<QString>& labels_arr = getSysLabels();
    int idx = (int)l;
    if(idx < 0 || idx >= static_cast<int>(labels_arr.size())){
        qWarning() << "ERROR. Invalid SysLabel index" << idx << labels_arr.size();
        return "";
    }
    
    QString s = getSysLabels()[idx];
    return s;
};

QString mail_cache::sysLabelName(SysLabel l)
{
    if (syslabel2Name.empty()) {
        getSysLabels();
    }

    auto idx = syslabel2Name.find(l);
    if (idx == syslabel2Name.end()) {
        qWarning() << "ERROR. Invalid SysLabel" << static_cast<int>(l) << syslabel2Name.size();
        return "";
    }

    return idx->second;
};

bool mail_cache::GMailSQLiteStorage::loadLabelsFromDb()
{
    QString sql = QString("SELECT label_id, label_name, label_type, label_unread_messages, label_mask FROM %1gmail_label WHERE acc_id=%2 ORDER BY label_id")
        .arg(m_metaPrefix)
        .arg(m_accId);
    QSqlQuery* q = selectQuery(sql);
    if (!q)
        return false;

    ///init static table
    std::vector<QString>& sys_labels = getSysLabels();
    if (sys_labels.empty()) {
        qWarning() << "ERROR. Static table of syslabels empty";
        return false;
    }


    std::map<QString, int> system_labels2ensure;
    for(auto& i : reservedLabelID2SysLabel){
        auto lbId = i.first;
        int mask_base = static_cast<int>(i.second);
        system_labels2ensure[lbId] = mask_base;
    }
    /*
    std::vector<QString>& sys_labels = getSysLabels();
    for(auto& lbl : sys_labels){
        system_labels2ensure.insert(lbl);
    }
    */
    
#ifdef API_QT_AUTOTEST
    ///default autotest labels
    system_labels2ensure["id_1"] = -1;
    system_labels2ensure["id_2"] = -1;
    system_labels2ensure["id_3"] = -1;
    system_labels2ensure["id_4"] = -1;
    system_labels2ensure["id_5"] = -1;
#endif

    while (q->next())
        {
            QString label_id = q->value(0).toString();
            QString label_name = q->value(1).toString();
            bool label_is_system = (q->value(2).toInt() == 1);
            uint64_t unread_messages = q->value(3).toULongLong();
            int mask_base = q->value(4).toInt();

            auto lbl = createAndInsertLabel(label_id,
                                            label_name,
                                            label_is_system,
                                            unread_messages,
                                            mask_base);
            if (lbl)
                {
                    system_labels2ensure.erase(label_id);
                }
        }

    if (!system_labels2ensure.empty()) {
        for (auto& i : system_labels2ensure) {
            auto lblId = i.first;
            auto mask_base = i.second;
            mail_cache::LabelData* lbl = ensureLabel(m_accId, lblId, true, mask_base);
            if (!lbl) {
                qWarning() << "ERROR. Failed to create label" 
                    << lblId 
                    << m_acc_labels.size();
            }            
        }
    }

    return true;
};

bool mail_cache::GMailSQLiteStorage::loadAttachmentsFromDb(MessageData& m) 
{
    int accId = m.accountId();
    if (accId == -1) {
        qWarning() << "ERROR. Invalid accountId" << m.id();
        return false;
    }

    QString sql = QString("SELECT att_id, file_name, local_file_name, mime_type, size FROM %1gmail_attachments WHERE msg_id='%2' AND acc_id=%3 ORDER BY att_id")
        .arg(m_metaPrefix)
        .arg(m.id())
        .arg(accId);
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

bool mail_cache::GMailSQLiteStorage::markMailAsTrashedInDb(MessageData& m) 
{
    int accId = m.accountId();
    if (accId == -1) {
        qWarning() << "ERROR. Invalid accountId" << m.id();
        return false;
    }

    auto lb = findLabel(mail_cache::SysLabel::TRASH);
    if (!lb) {
        qWarning() << "ERROR. Failed to locate label by id" << m.id();
        return false;
    }

    m.m_labels |= lb->labelMask();

    auto sql_update = QString("UPDATE %1gmail_msg SET msg_labels=? WHERE msg_id=? AND acc_id=?")
        .arg(m_metaPrefix);

    QSqlQuery* q = prepareQuery(sql_update);
    if (!q)return false;

    q->addBindValue(static_cast<qint64>(m.labelsBitMap()));
    q->addBindValue(m.id());
    q->addBindValue(accId);

    return true;
};

void mail_cache::GMailSQLiteStorage::updateMessagesDiagnostic(int inc_batch, int inc_msg) 
{
    if (m_accId != -1)
    {
        auto i = m_id2acc.find(m_accId);
        if (i != m_id2acc.end()) {
            auto a = i->second;
            a->m_diagnostic.m_msg_cloud_batch_queried += inc_batch;
            a->m_diagnostic.m_msg_cloud_email_loaded += inc_msg;

            QString sql = QString("UPDATE %1gmail_account SET msg_cloud_batch_queried=%2, msg_cloud_email_loaded=%3 WHERE acc_id = %4")
                .arg(m_metaPrefix)
                .arg(a->m_diagnostic.m_msg_cloud_batch_queried)
                .arg(a->m_diagnostic.m_msg_cloud_email_loaded)
                .arg(m_accId);
            QSqlQuery* q = prepareQuery(sql);
            if (!q)
            {
                qWarning() << "Failed to prepare query" << sql;
                return;
            };
            if (!q->exec())
            {
                qWarning() << "Failed to execute query" << sql;
                return;
            };
        }
    }
};

int mail_cache::GMailSQLiteStorage::getCacheMessagesCount(mail_cache::label_ptr lbl)
{
    int rv = 0;

    QString sql;
    sql = QString("SELECT COUNT(*) FROM %1gmail_msg WHERE acc_id=%2 AND (msg_labels&%3 = %3)")
        .arg(m_metaPrefix)
        .arg(m_accId)
        .arg(lbl->labelMask());

    QSqlQuery* q = selectQuery(sql);
    if (!q) {
        qWarning() << "failed to select msg count for label" << lbl->labelId();
        return 0;
    }
    if (q->next())
    {
        rv = q->value(0).toInt();
    }
    return rv;
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

void mail_cache::GMailSQLiteStorage::invalidateAttachmentLocalCacheFile(att_ptr att)
{
    att->m_status = AttachmentData::statusNotDownloaded;
    QString local_path = downloadDir() + "/" + att->localFilename();
    if (QFile::exists(local_path)) {
        bool ok = QFile::remove(local_path);
        if(!ok){
            qWarning() << "failed to delete cache file" << local_path;
        }
    }
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
    m_acc_labels[lb->labelId()] = lb;
#ifdef _DEBUG
    if (mask_base > 64) {
        qWarning() << "ERROR. Too big label mask" << mask_base << m_maskbase2label.size();
    }
#endif
    m_maskbase2label[mask_base] = lb;
    return lb.get();
};

bool mail_cache::GMailSQLiteStorage::updateDbLabel(const labels::LabelResource& lbl, LabelData* db_lbl)
{
    if (db_lbl->labelId() != lbl.id()) {
        qWarning() << "Expected equal label IDs" << db_lbl->labelId() << lbl.id();
        return false;
    }

    QString name = lbl.name();
    if(lbl.type().compare("system", Qt::CaseInsensitive) == 0){
        auto i = syslabelID2Name.find(lbl.id());
        if (i != syslabelID2Name.end()) {
            name = i->second;
        }        
    }
    
    db_lbl->m_label_name = name;
    db_lbl->m_unread_messages = lbl.messagesunread();

    QString sql_update;
    sql_update = QString("UPDATE %1gmail_label SET label_name='%2', label_unread_messages=%3 WHERE label_id='%4' AND acc_id=%5")
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
        qWarning() << "ERROR. Failed to store label " << lbl.id() << sql_insert << q->lastError().text();

    }

    return rv;
};

mail_cache::LabelData* mail_cache::GMailSQLiteStorage::insertDbLabel(int accId, 
                                                                     QString label_id,
                                                                     QString label_name,
                                                                     bool system_label/*QString label_type*/,
                                                                     int mask_base) 
{
    if (m_avail_label_base.empty()) 
        {
            qWarning() << "ERROR. Exhausted labels masks. New label can not be registered." << m_avail_label_base.size() << label_id << label_name;
            return nullptr;
        }

    if(mask_base == -1){
        auto i = m_avail_label_base.begin();
        mask_base = *i;
    }

    QString sql_insert;
    sql_insert = QString("INSERT INTO  %1gmail_label(acc_id, label_id, label_name, label_type, label_mask) VALUES(%2, ?, ?, ?, ?)")
        .arg(m_metaPrefix)
        .arg(accId);
    QSqlQuery* q = prepareQuery(sql_insert);
    if (!q)return nullptr;

    int label_type_as_int = system_label ? 1 : 2;
    /*
    if (label_type.compare("system", Qt::CaseInsensitive) == 0) {
        label_type_as_int = 1;
        }*/

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

bool mail_cache::GMailSQLiteStorage::deleteDbLabel(QString labelId) 
{
    auto i = m_acc_labels.find(labelId);
    if (i == m_acc_labels.end()) {
        return true;
    }
    
    auto lb = i->second;

    uint64_t flags = lb->labelMask();

    QString sql_update;
    sql_update = QString("UPDATE %1gmail_msg SET msg_labels=(~(msg_labels&%2))&(msg_labels|%2) WHERE acc_id=%3 AND (msg_labels&%2 = %2)")
        .arg(m_metaPrefix)
        .arg(flags)
        .arg(m_accId);
    if (!execQuery(sql_update)) {
        qWarning() << "SQL update failed" << sql_update;
        return false;
    };

    QString sql_delete;
    sql_delete = QString("DELETE FROM %1gmail_label WHERE acc_id = %2 AND label_id='%3'")
        .arg(m_metaPrefix)
        .arg(m_accId)
        .arg(labelId);
    if (!execQuery(sql_delete)) {
        qWarning() << "SQL delete failed" << sql_delete;
        return false;
    };

    m_acc_labels.erase(i);

    return true;
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

void mail_cache::GMailSQLiteStorage::reloadDbAccounts() 
{
    m_id2acc.clear();
    m_user2acc.clear();

    QString sql = QString("SELECT acc_id, userid, msg_cloud_batch_queried, msg_cloud_email_loaded FROM %1gmail_account").arg(m_metaPrefix);
    QSqlQuery* q = selectQuery(sql);
    if (!q) {
        qWarning() << "ERROR. Failed to load account data from DB" << sql;
        return ;
    }

    while (q->next())
    {
        int acc_id = q->value(0).toInt();
        QString userid = q->value(1).toString();
        int msg_cloud_batch_queried = q->value(2).toInt();
        int msg_cloud_email_loaded = q->value(3).toInt();
        mail_cache::acc_ptr p(new AccountData(acc_id, userid));
        m_id2acc[acc_id] = p;
        m_user2acc[userid] = p;

        p->m_diagnostic.m_msg_cloud_batch_queried = msg_cloud_batch_queried;
        p->m_diagnostic.m_msg_cloud_email_loaded = msg_cloud_email_loaded;
    }
};

bool mail_cache::GMailSQLiteStorage::reloadDbConfig()
{
    m_configs.clear();
    QString sql = QString("SELECT config_name, config_value FROM %1config")
        .arg(m_metaPrefix);
    QSqlQuery* q = selectQuery(sql);
    if (!q)
        return false;
    
    int db_version = 0;

    while (q->next())
    {
        QString name = q->value(0).toString();
        QString value = q->value(1).toString();
        m_configs[name] = value;

       if (name.compare(CONFIG_VERSION, Qt::CaseInsensitive) == 0) {
           db_version = value.toInt();
        }
    }

    if(db_version == 0){
        QString sql = QString("INSERT OR REPLACE INTO %1config(config_name, config_value) VALUES(%2, %3)")
            .arg(m_metaPrefix)
            .arg("'version'")
            .arg('1');
        QSqlQuery* q = prepareQuery(sql);
        if (!q){
            qWarning() << "ERROR. Failed to prepare query for cache version setup";
            return false;
        }
        if (!q->exec()) {
            qWarning() << "ERROR. Failed to setup cache DB version";
            return false;
        }
    }

    return true;
};

std::vector<mail_cache::acc_ptr> mail_cache::GMailSQLiteStorage::getAccounts()
{
    std::vector<mail_cache::acc_ptr> rv;
    for (auto& i : m_user2acc) {
        rv.push_back(i.second);
    }
    return rv;
};

bool mail_cache::GMailSQLiteStorage::deleteAccountFromDb(int accId) 
{
    QString sql = QString("DELETE FROM %1gmail_msg WHERE acc_id = %2")
        .arg(m_metaPrefix)
        .arg(accId);
    bool ok = execQuery(sql);
    if(!ok){
        qWarning() << "ERROR.Failed to delete messages" << sql;
        return false;
    }

    //..
    sql = QString("DELETE FROM %1gmail_thread WHERE acc_id = %2")
        .arg(m_metaPrefix)
        .arg(accId);
    ok = execQuery(sql);
    if(!ok){
        qWarning() << "ERROR.Failed to delete threads" << sql;
        return false;
    }
    //...

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

    sql = QString("DELETE FROM %1gmail_qres WHERE acc_id = %2")
        .arg(m_metaPrefix)
        .arg(accId);
    ok = execQuery(sql);
    if (!ok) {
        qWarning() << "ERROR.Failed to delete gmail_qres" << sql;
        return false;
    }

    sql = QString("DELETE FROM %1gmail_q WHERE acc_id = %2")
        .arg(m_metaPrefix)
        .arg(accId);
    ok = execQuery(sql);
    if (!ok) {
        qWarning() << "ERROR.Failed to delete gmail_q" << sql;
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
    reloadDbAccounts();
    return true;
};

bool mail_cache::GMailSQLiteStorage::canRenameAccount(int accId)
{
    QString sql = QString("SELECT count(*) n FROM %1gmail_msg WHERE acc_id = %2"
                          " UNION "
                          "SELECT count(*) n FROM %1gmail_label WHERE acc_id = %2"
                          " UNION "
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

bool mail_cache::GMailSQLiteStorage::updateAccountInDb(int accId, QString userId) 
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
    reloadDbAccounts();
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
        reloadDbAccounts();
    }
    return accId;
};

mail_cache::label_ptr mail_cache::GMailSQLiteStorage::findLabel(QString label_id)
{
    auto i = m_acc_labels.find(label_id);
    if (i != m_acc_labels.end()) {
        return i->second;
    }
    return nullptr;
};

mail_cache::label_ptr mail_cache::GMailSQLiteStorage::findLabel(SysLabel sys_label)
{
    QString lable_id = sysLabelId(sys_label);
    return findLabel(lable_id);
};

mail_cache::label_ptr mail_cache::GMailSQLiteStorage::findLabelByName(QString name)
{
    for (auto& i : m_acc_labels) {
        auto lb = i.second;
        if (lb->labelName().compare(name, Qt::CaseInsensitive) == 0) {
            return lb;
        }
    }
    return nullptr;
};

mail_cache::LabelData* mail_cache::GMailSQLiteStorage::ensureLabel(int accId, QString label_id, bool system_label, int mask_base)
{
    auto i = m_acc_labels.find(label_id);
    if (i != m_acc_labels.end()) {
        return i->second.get();
    }

    QString label_name = label_id;
    if (system_label) {
        auto i = syslabelID2Name.find(label_id);
        if (i != syslabelID2Name.end()) {
            label_name = i->second;
        }
    }

    mail_cache::LabelData* rv = insertDbLabel(accId,
                                              label_id,
                                              label_name,
                                              system_label,
                                              mask_base);
    return rv;
};


std::vector<mail_cache::label_ptr> mail_cache::GMailSQLiteStorage::getLabelsInSet(std::set<QString>* in_optional_idset /*= nullptr*/)
{
    std::vector<mail_cache::label_ptr> rv;
    for (auto& i : m_acc_labels) {
        bool add_label = true;
        if (in_optional_idset) {
            add_label = (in_optional_idset->find(i.first) != in_optional_idset->end());
        }
        if (add_label) {
            rv.push_back(i.second);
        }
    }
    return rv;
};

uint64_t mail_cache::GMailSQLiteStorage::packLabels(const std::vector<QString>& labels)
{
    uint64_t f = 0;

    if (labels.size() > 0) {
        for (auto& label_id : labels) {
            auto i = m_acc_labels.find(label_id);
            if (i != m_acc_labels.end()) {
                auto lb = i->second.get();
                f |= lb->labelMask();
            }
            else {
                static int warning_count = 0;
                if (warning_count < 100)
                    {
                        qWarning() << "ERROR. Lost label (pack)" << label_id << m_acc_labels.size();
                        warning_count++;
                    }
            }
        }
    }

    return f;
};

std::vector<mail_cache::label_ptr> mail_cache::GMailSQLiteStorage::unpackLabels(const uint64_t& data)const
{
    std::vector<mail_cache::label_ptr> labels;

    uint64_t theone = 1;
    for (int b = 0; b < 64; b++)
        {
            uint64_t m = (theone << b);
            if (m > data)
                break;
            if (m & data) {
                auto lbl = m_maskbase2label[b];
                if (lbl) {
                    labels.push_back(lbl);
                }
                else {
                    static int warning_count = 0;
                    if (warning_count < 100)
                    {
                        qWarning() << "ERROR. Lost label (unpack)" << b << m_maskbase2label.size();
                        warning_count++;
                    }
                }
                /*
                auto i = m_maskbase2label.find(b);
                if (i != m_maskbase2label.end()){
                    labels.push_back(i->second);
                }
                else {
                    static int warning_count = 0;
                    if (warning_count < 100)
                        {
                            qWarning() << "ERROR. Lost label (unpack)" << b << m_maskbase2label.size();
                            warning_count++;
                        }
                }
                */
            }
        }

    return labels;
};

void mail_cache::GMailSQLiteStorage::update_message_labels_db(int accId, QString msg_id, uint64_t flags)
{
    QString sql_update;
    sql_update = QString("UPDATE %1gmail_msg SET msg_labels=%2 WHERE msg_id='%3' AND acc_id=%4")
        .arg(m_metaPrefix)
        .arg(flags)
        .arg(msg_id)
        .arg(accId);
    execQuery(sql_update);
};

void mail_cache::GMailSQLiteStorage::update_attachment_local_file_db(googleQt::mail_cache::msg_ptr m, 
                                                                     googleQt::mail_cache::att_ptr a, 
                                                                     QString file_name)
{
    int accId = m->accountId();
    if (accId == -1) {
        qWarning() << "ERROR. Failed to locate accountId" << m->id();
        return;
    }

    QString sql_update;
    sql_update = QString("UPDATE %1gmail_attachments SET local_file_name='%2' WHERE msg_id='%3' AND att_id='%4' AND acc_id=%5")
        .arg(m_metaPrefix)
        .arg(file_name)
        .arg(m->id())
        .arg(a->attachmentId())
        .arg(accId);
    bool ok = execQuery(sql_update);
    if (ok) {
        a->m_local_filename = file_name;
        a->m_status = AttachmentData::statusDownloaded;
    }
};

bool mail_cache::GMailSQLiteStorage::doExecQuery(std::unique_ptr<QSqlQuery>& q, QString sql) 
{
	if (!q) {
		qWarning() << "ERROR. Expected internal query";
		return false;
	}

	if (!q->prepare(sql)) {
		QString error = q->lastError().text();
		qWarning() << "ERROR. Failed to prepare sql query"
			<< error
			<< sql;
		return false;
	};
	if (!q->exec(sql)) {
		QString error = q->lastError().text();
		qWarning() << "ERROR. Failed to execute query"
			<< error
			<< sql;
		return false;
	}
	return true;
};

bool mail_cache::GMailSQLiteStorage::execQuery(QString sql)
{
	return doExecQuery(m_query, sql);
};

bool mail_cache::GMailSQLiteStorage::execContactQuery(QString sql)
{
	return doExecQuery(m_contact_query, sql);
}

QSqlQuery* mail_cache::GMailSQLiteStorage::startTransaction(QString sql) 
{
    QSqlQuery* q = nullptr;
    if (startTransaction()) {
        q = prepareQuery(sql);
        if (!q) {
            rollbackTransaction();
        }
    }
    return q;
};

bool mail_cache::GMailSQLiteStorage::startTransaction()
{
    return m_gmail_db.transaction();
};

bool mail_cache::GMailSQLiteStorage::rollbackTransaction()
{
    return m_gmail_db.rollback();
};

bool mail_cache::GMailSQLiteStorage::commitTransaction()
{
    return m_gmail_db.commit();
};

QSqlQuery* mail_cache::GMailSQLiteStorage::doPrepareQuery(std::unique_ptr<QSqlQuery>& q, QString sql)
{
	if (!q)
	{
		qWarning() << "ERROR. Expected internal query";
		return nullptr;
	}
	if (!q->prepare(sql))
	{
		QString error = q->lastError().text();
		qWarning() << "ERROR. Failed to prepare sql query" << error << sql;
		return nullptr;
	};
	return q.get();
};

QSqlQuery* mail_cache::GMailSQLiteStorage::prepareQuery(QString sql)
{
    return doPrepareQuery(m_query, sql);
};

QSqlQuery* mail_cache::GMailSQLiteStorage::prepareContactQuery(QString sql)
{
	return doPrepareQuery(m_contact_query, sql);
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

QSqlQuery* mail_cache::GMailSQLiteStorage::selectContactQuery(QString sql)
{
	QSqlQuery* q = prepareContactQuery(sql);
	if (!q)return nullptr;
	if (!q->exec(sql))
	{
		QString error = q->lastError().text();
		qWarning() << "ERROR. Failed to execute contact query" << error << sql;
		return nullptr;
	};
	return q;
};

QString mail_cache::GMailSQLiteStorage::lastSqlError()const 
{
    if (!m_query) {
        return "";
    }
    QString error = m_query->lastError().text();
    return error;
};

mail_cache::thread_ptr mail_cache::GMailSQLiteStorage::findThread(QString thread_id)
{
    mail_cache::thread_ptr rv;
    if (!m_thread_cache) {
        return nullptr;
    }
    rv = m_thread_cache->mem_object(thread_id);
    return rv;
};


googleQt::mail_cache::thread_arr mail_cache::GMailSQLiteStorage::loadThreadsByIdsFromDb(const std::vector<QString>& thread_ids)
{
	return m_tstorage->loadThreadsByIdsFromDb(thread_ids);
};
/**
    GMessagesStorage
*/
mail_cache::GMessagesStorage::GMessagesStorage(GMailSQLiteStorage* s)
{
    m_storage = s;
};

QString mail_cache::GMessagesStorage::insertSnippetSQL()const 
{
    QString sql = QString("INSERT INTO %1gmail_msg(msg_state, msg_from, msg_to, msg_cc, msg_bcc, msg_subject, "
        "msg_snippet, internal_date, msg_references, msg_labels, msg_id, acc_id, thread_id) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, %2, ?)")
        .arg(m_storage->metaPrefix())
        .arg(m_storage->currentAccountId());
    return sql;
};

QString mail_cache::GMessagesStorage::insertBodySQL()const 
{
    QString sql = QString("INSERT INTO %1gmail_msg(msg_state, msg_from, msg_to, msg_cc, msg_bcc, msg_subject, "
        "msg_snippet, msg_plain, msg_html, internal_date, msg_references, msg_labels, msg_id, acc_id, thread_id) VALUES(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, %2, ?)")
        .arg(m_storage->metaPrefix())
        .arg(m_storage->currentAccountId());
    return sql;
};

QString mail_cache::GMessagesStorage::updateSnippetSQL()const 
{
    QString sql = QString("UPDATE %1gmail_msg SET msg_state=?, msg_from=?, msg_to=?, "
        "msg_cc=?, msg_bcc=?, msg_subject=?, msg_snippet=?, internal_date=?, msg_references=?, msg_labels=? WHERE msg_id=? AND acc_id=%2 AND thread_id=?")
        .arg(m_storage->metaPrefix())
        .arg(m_storage->currentAccountId());
    return sql;
};

QString mail_cache::GMessagesStorage::updateBodySQL()const 
{
    QString sql = QString("UPDATE %1gmail_msg SET msg_state=?, msg_from=?, msg_to=?, msg_cc=?, msg_bcc=?, "
        "msg_subject=?, msg_snippet=?, msg_plain=?, msg_html=?, internal_date=?, msg_references=?, msg_labels=? WHERE msg_id=? AND acc_id=%2 AND thread_id=?")
        .arg(m_storage->metaPrefix())
        .arg(m_storage->currentAccountId());
    return sql;
};

QString mail_cache::GMessagesStorage::insertLabelsSQL()const
{
    QString sql = QString("INSERT INTO %1gmail_msg(msg_labels, msg_id, acc_id, thread_id) VALUES(?, ?, %2, ?)")
        .arg(m_storage->metaPrefix())
        .arg(m_storage->currentAccountId());
    return sql;
};


QString mail_cache::GMessagesStorage::updateLabelsSQL()const 
{
    QString sql = QString("UPDATE %1gmail_msg SET msg_labels=? WHERE msg_id=? AND acc_id=%2 AND thread_id=?")
        .arg(m_storage->metaPrefix())
        .arg(m_storage->currentAccountId());
    return sql;
};

void mail_cache::GMessagesStorage::bindSQL(EDataState state, QSqlQuery* q, CACHE_LIST<MessageData>& r)
{
    QVariantList aggState, from, to, cc, bcc, subject, snippet,
        plain, html, internalDate, labelsBitMap, references, id, threadId;
    for (auto& i : r)
    {
        aggState << i->aggState();
        from << i->from();
        to << i->to();
        cc << i->cc();
        bcc << i->bcc();
        subject << i->subject();
        snippet << i->snippet();
        if (state == EDataState::body) {
            plain << i->plain();
            html << i->html();
        }
        internalDate << i->internalDate();
        references << i->references();
        labelsBitMap << static_cast<qint64>(i->labelsBitMap());     
        id << i->id();
        threadId << i->threadId();
    }

    if (state == EDataState::body || state == EDataState::snippet)
    {
        q->addBindValue(aggState);
        q->addBindValue(from);
        q->addBindValue(to);
        q->addBindValue(cc);
        q->addBindValue(bcc);
        q->addBindValue(subject);
        q->addBindValue(snippet);
        if (state == EDataState::body) {
            q->addBindValue(plain);
            q->addBindValue(html);
        }
        q->addBindValue(internalDate);
        q->addBindValue(references);
    }
    q->addBindValue(labelsBitMap);
    q->addBindValue(id);
    q->addBindValue(threadId);
};

bool mail_cache::GMessagesStorage::execOutOfBatchSQL(EDataState state, QSqlQuery* q, mail_cache::MessageData* m) 
{
    if (state == EDataState::body || state == EDataState::snippet)
    {
        q->addBindValue(m->aggState());
        q->addBindValue(m->from());
        q->addBindValue(m->to());
        q->addBindValue(m->cc());
        q->addBindValue(m->bcc());
        q->addBindValue(m->subject());
        q->addBindValue(m->snippet());
        if (state == EDataState::body) {
            q->addBindValue(m->plain());
            q->addBindValue(m->html());
        }
        q->addBindValue(m->internalDate());
        q->addBindValue(m->references());
    }
    q->addBindValue(static_cast<qint64>(m->labelsBitMap()));    
    q->addBindValue(m->id());
    q->addBindValue(m->threadId());
    return q->exec();
};

bool mail_cache::GMessagesStorage::insertDbInBatch(EDataState state, CACHE_LIST<MessageData>& r)
{
    QString sql_insert;
    switch (state)
    {
    case EDataState::labels:
    {
        sql_insert = insertLabelsSQL();
    }break;
    case EDataState::snippet:
    {
        sql_insert = insertSnippetSQL();
    }break;
    case EDataState::body:
    {
        sql_insert = insertBodySQL();
    }break;
    }

    auto q = m_storage->startTransaction(sql_insert);
    if (!q) {
        m_storage->rollbackTransaction();
        return false;
    }

    bindSQL(state, q, r);

    if (!q->execBatch())
    {
        m_storage->rollbackTransaction();
        qWarning() << "ERROR. SQL message/insert batch failed"
            << "err-type:" << q->lastError().type()
            << "native-code:" << q->lastError().nativeErrorCode()
            << "errtext:" << q->lastError().text();
        return false;
    }

    bool rv = m_storage->commitTransaction();
    if (rv) {
        for (auto& i : r)
        {
            i->markDbRecord();
        }
    }

    return rv;
};

bool mail_cache::GMessagesStorage::updateDbInBatch(EDataState state, CACHE_LIST<MessageData>& r)
{
    QString sql;
    switch (state)
    {
    case EDataState::labels:
    {
        sql = updateLabelsSQL();
    }break;
    case EDataState::snippet:
    {
        sql = updateSnippetSQL();
    }break;
    case EDataState::body:
    {
        sql = updateBodySQL();
    }break;
    }

    auto q = m_storage->startTransaction(sql);
    if (!q) {
        m_storage->rollbackTransaction();
        return false;
    }

    bindSQL(state, q, r);

    if (!q->execBatch())
    {
        m_storage->rollbackTransaction();
        qWarning() << "ERROR. SQL message/update batch failed"
            << "err-type:" << q->lastError().type()
            << "native-code:" << q->lastError().nativeErrorCode()
            << "errtext:" << q->lastError().text();
        return false;
    }

    bool rv = m_storage->commitTransaction();
    if (rv) {
        for (auto& i : r)
        {
            i->markDbRecord();
        }
    }

    return rv;
};


void mail_cache::GMessagesStorage::update_db(
    EDataState state,
    CACHE_LIST<mail_cache::MessageData>& r)
{
    if (!updateDbInBatch(state, r)) {
        QString sql_update, sql_insert;
        switch (state)
        {
        case EDataState::labels:
        {
            sql_update = updateLabelsSQL();
            sql_insert = insertLabelsSQL();
        }break;
        case EDataState::snippet:
        {
            sql_update = updateSnippetSQL();
            sql_insert = insertSnippetSQL();
        }break;
        case EDataState::body:
        {
            sql_update = updateBodySQL();
            sql_insert = insertBodySQL();
        }break;
        }


        for (auto& i : r)
        {
            QSqlQuery* q = m_storage->prepareQuery(sql_update);
            if (!q)return;
            bool ok = execOutOfBatchSQL(state, q, i.get());
            if (!ok)
            {
                qWarning() << "ERROR. SQL message/update failed"
                    << "err-type:" << q->lastError().type()
                    << "native-code:" << q->lastError().nativeErrorCode()
                    << "errtext:" << q->lastError().text()
                    << "threadid=" << i->id();
            }
            int rows_updated = q->numRowsAffected();
            if (rows_updated <= 0)
            {
                q = m_storage->prepareQuery(sql_insert);
                if (!q)return;
                ok = execOutOfBatchSQL(state, q, i.get());
                if (!ok)
                {
                    qWarning() << "ERROR. SQL message/insert failed"
                        << "err-type:" << q->lastError().type()
                        << "native-code:" << q->lastError().nativeErrorCode()
                        << "errtext:" << q->lastError().text()
                        << "threadid=" << i->id();
                }
            }
        }
    }
};

void mail_cache::GMessagesStorage::insert_db(EDataState state, CACHE_LIST<MessageData>& r)
{
    if (!insertDbInBatch(state, r)) {
        QString sql_update, sql_insert;
        switch (state)
        {
        case EDataState::labels:
        {
            sql_update = updateLabelsSQL();
            sql_insert = insertLabelsSQL();
        }break;
        case EDataState::snippet:
        {
            sql_update = updateSnippetSQL();
            sql_insert = insertSnippetSQL();
        }break;
        case EDataState::body:
        {
            sql_update = updateBodySQL();
            sql_insert = insertBodySQL();
        }break;
        }

        for (auto& i : r)
        {
            QSqlQuery* q = m_storage->prepareQuery(sql_insert);
            if (!q)return;
            bool ok = execOutOfBatchSQL(state, q, i.get());
            if (!ok) {
                if (q->lastError().nativeErrorCode() == "19") {
                    ///constraint error - duplicate record, do update instead
                    q = m_storage->prepareQuery(sql_update);
                    if (!q)return;
                    ok = execOutOfBatchSQL(state, q, i.get());
                    if (!ok)
                    {
                        qWarning() << "ERROR. SQL message/update failed"
                            << "err-type:" << q->lastError().type()
                            << "native-code:" << q->lastError().nativeErrorCode()
                            << "errtext:" << q->lastError().text()
                            << "ID:" << i->id();
                    }
                }
                else {
                    qWarning() << "ERROR. SQL message/insert failed"
                        << "err-type:" << q->lastError().type()
                        << "native-code:" << q->lastError().nativeErrorCode()
                        << "errtext:" << q->lastError().text()
                        << "ID:" << i->id();
                }
            }
        }
    }
};

void mail_cache::GMessagesStorage::insertDbAttachmentData(const mail_cache::MessageData& m)
{
    if (m.m_attachments.size() == 0) {
        qWarning() << "ERROR. Not attachments to store for" << m.id();
        return;
    }

    int accId = m.accountId();
    if (accId == -1) {
        qWarning() << "ERROR. Invalid accountId" << m.id();
        return;
    }

    QString sql_insert;
    sql_insert = QString("INSERT INTO  %1gmail_attachments(att_id, acc_id, msg_id, file_name, mime_type, size)"
        " VALUES(?, %2, ?, ?, ?, ?)")
        .arg(m_storage->metaPrefix())
        .arg(accId);

    for (auto& a : m.m_attachments)
    {
        QSqlQuery* q = m_storage->prepareQuery(sql_insert);
        if (!q) {
            QString error = m_storage->lastSqlError();
            qWarning() << "ERROR. Failed to prepare SQL" << sql_insert << error;
            return;
        }

        q->addBindValue(a->attachmentId());
        q->addBindValue(m.id());
        q->addBindValue(a->filename());
        q->addBindValue(a->mimetype());
        q->addBindValue(a->size());

        if (!q->exec()) {
            QString error = m_storage->lastSqlError();
            qWarning() << "ERROR. Failed to store attachment accId=" << m_storage->currentAccountId() << m.id() << a->attachmentId() << error;
            continue;
        }
    }
};

void mail_cache::GMessagesStorage::remove_db(const std::set<QString>& set_of_ids2remove)
{
    STRING_LIST ids2remove;
    for (auto& i : set_of_ids2remove)
    {
        ids2remove.push_back(i);
    }

    std::function<bool(const STRING_LIST& lst)> removeSublist = [&](const STRING_LIST& lst) -> bool
    {
        QString comma_ids = slist2commalist_decorated(lst);
        QString sql = QString("DELETE FROM %1gmail_msg WHERE msg_id IN(%2) AND acc_id=%3")
            .arg(m_storage->metaPrefix())
            .arg(comma_ids)
            .arg(m_storage->currentAccountId());
        QSqlQuery* q = m_storage->prepareQuery(sql);
        if (!q)return false;
        if (!q->exec()) return false;
        return true;
    };

    if (isValid())
    {
        if (!chunk_list_execution(ids2remove, removeSublist))
        {
            qWarning() << "ERROR. Failed to remove list of records.";
        }
    }
};

bool mail_cache::GMessagesStorage::isValid()const 
{
    return m_storage->isValid();
};

bool mail_cache::GMessagesStorage::loadMessagesFromDb()
{
	auto cache = m_storage->mcache();
    if (!cache) {
        return false;
    }

    thread_ptr thread_in_select = nullptr;

    QString sql = QString("SELECT msg_state, msg_id, msg_from, msg_to, msg_cc, msg_bcc, "
        "msg_subject, msg_snippet, msg_plain, msg_html, internal_date, msg_labels, msg_references, thread_id FROM %1gmail_msg WHERE acc_id=%2 AND "
		"thread_id IN(SELECT thread_id FROM %1gmail_thread WHERE acc_id=%2 AND thread_id IN(SELECT thread_id FROM %1gmail_msg WHERE acc_id=%2) ORDER BY internal_date DESC LIMIT %3) "
		"ORDER BY thread_id, internal_date DESC")
        .arg(m_storage->metaPrefix())
        .arg(m_storage->currentAccountId())
		.arg(m_storage->autoloadLimit());
    QSqlQuery* q = m_storage->selectQuery(sql);
    if (!q)
        return false;

    int loaded_objects = 0;
    while (q->next())
        {
            QString thread_id = q->value(13).toString();
            if (!thread_id.isEmpty()) {
                if (!thread_in_select ||
                    thread_in_select->id() != thread_id) 
                {
                    thread_in_select = m_storage->findThread(thread_id);
                }

                if (thread_in_select) {
                    auto md = loadMessageFromDb(thread_in_select, q);
                    if (md == nullptr)
                        continue;
                    cache->mem_insert(md->id(), md);
                    loaded_objects++;
                }
                else {
                    qWarning() << "lost thread in DB" << thread_id << "for message" << q->value(1).toString();
                }
            }//thread_id
        }	

#ifdef API_QT_AUTOTEST
	qDebug() << QString("loadMessagesFromDb/DB-loaded %1 records, mem-cache-size: %2").arg(loaded_objects).arg(cache->mem_size());
    ApiAutotest::INSTANCE() << QString("messages/DB-loaded %1 records, mem-cache-size: %2")
        .arg(loaded_objects).arg(cache->mem_size());
#endif

    return true;
};

mail_cache::msg_ptr mail_cache::GMessagesStorage::loadMessageFromDb(thread_ptr t1, QSqlQuery* q, int fidx)
{
    mail_cache::msg_ptr md;

    int agg_state = q->value(0 + fidx).toInt();
    if (agg_state < 0 || agg_state > 3)
        {
            qWarning() << "ERROR. Invalid DB state" << agg_state << q->value(1).toString();
            return nullptr;
        }
    QString msg_id = q->value(1 + fidx).toString();
    if (msg_id.isEmpty())
        {
            qWarning() << "ERROR. Invalid(empty) msg_id";
            return nullptr;
        }
    /*
    QString thread_id = q->value(2).toString();
    if (thread_id.isEmpty())
    {
        qWarning() << "ERROR. Invalid(empty) thread_id";
        return nullptr;
    } */  

    QString msg_from = q->value(2 + fidx).toString();
    QString msg_to = q->value(3 + fidx).toString();
    QString msg_cc = q->value(4 + fidx).toString();
    QString msg_bcc = q->value(5 + fidx).toString();//+
    QString msg_subject = q->value(6 + fidx).toString();
    QString msg_snippet = q->value(7 + fidx).toString();//+
    QString msg_plain = "";
    QString msg_html = "";
    
    if (agg_state > 1)
        {
            msg_plain = q->value(8 + fidx).toString();
            msg_html = q->value(9 + fidx).toString();
        }
    qlonglong  msg_internalDate = q->value(10 + fidx).toLongLong();
    qlonglong  msg_labels = q->value(11 + fidx).toLongLong();
    QString msg_references = q->value(12 + fidx).toString();

    md = std::shared_ptr<MessageData>(new MessageData(m_storage->currentAccountId(),
		t1->id(),
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
        msg_labels,
        msg_references));
    md->markDbRecord();
    if (t1) {
        t1->add_msg(md);
    }
    return md;
};

/**
    GThreadsStorage
*/
mail_cache::GThreadsStorage::GThreadsStorage(GMailSQLiteStorage* s)
{
    m_storage = s;
};

bool mail_cache::GThreadsStorage::loadThreadsFromDb()
{
  auto cache = m_storage->tcache();
    if (!cache) {
        return false;
    }

    QString sql = QString("SELECT thread_id, history_id, messages_count, snippet, t_labels FROM %1gmail_thread " 
        "WHERE acc_id=%2 AND thread_id IN(SELECT thread_id FROM %1gmail_msg WHERE acc_id=%2) ORDER BY internal_date DESC LIMIT %3")
        .arg(m_storage->metaPrefix())
        .arg(m_storage->currentAccountId())
		.arg(m_storage->autoloadLimit());
    QSqlQuery* q = m_storage->selectQuery(sql);
    if (!q)
        return false;

    int loaded_objects = 0;
    while (q->next())
    {
        auto td = loadThread(q);
        if (td == nullptr)
            continue;
        cache->mem_insert(td->id(), td);
        loaded_objects++;
    }	

#ifdef API_QT_AUTOTEST
	qDebug() << QString("loadThreadsFromDb/DB-loaded %1 records, mem-cache-size: %2").arg(loaded_objects).arg(cache->mem_size());
    ApiAutotest::INSTANCE() << QString("threads/DB-loaded %1 records, mem-cache-size: %2")
        .arg(loaded_objects).arg(cache->mem_size());
#endif

    return true;
};

googleQt::mail_cache::thread_arr mail_cache::GThreadsStorage::loadThreadsByIdsFromDb(const std::vector<QString>& thread_ids)
{
	thread_arr rv;

	auto tcache = m_storage->tcache();
	if (!tcache) {
		return rv;
	}

	auto mcache = m_storage->mcache();
	if (!mcache) {
		return rv;
	}

	auto msg_storage = m_storage->m_mstorage.get();
	if (!msg_storage) {
		return rv;
	}

	std::function<googleQt::mail_cache::thread_arr(QString)> process_list = [&](QString ids_str)
	{
		googleQt::mail_cache::thread_arr rv;

		QString sql = QString("SELECT thread_id, history_id, messages_count, snippet, t_labels FROM %1gmail_thread "
			"WHERE acc_id=%2 AND thread_id IN(%3) ORDER BY internal_date DESC")
			.arg(m_storage->metaPrefix())
			.arg(m_storage->currentAccountId())
			.arg(ids_str);
		QSqlQuery* q = m_storage->selectQuery(sql);
		if (!q)
			return rv;

		std::vector<QString> loaded_threads;
		int located_threads = 0;
		while (q->next())
		{
			QString thread_id = q->value(0).toString();
			auto t1 = tcache->mem_object(thread_id);
			if (!t1) {
				auto td = loadThread(q);
				if (td == nullptr)
					continue;
				tcache->mem_insert(td->id(), td);
				loaded_threads.push_back(td->id());
				rv.push_back(td);
			}
			else {
				rv.push_back(t1);
				located_threads++;
			}
		}
		//qWarning() << "ykh/loadThreadsByIdsFromDb/batch threads loaded=" << loaded_threads.size() << "located=" << located_threads;

		bool load_messages_for_threads = true;
		if (load_messages_for_threads && loaded_threads.size() > 0) {
			QString loaded_threads_id_str = slist2commalist_decorated(loaded_threads);

			sql = QString("SELECT msg_state, msg_id, msg_from, msg_to, msg_cc, msg_bcc, "
				"msg_subject, msg_snippet, msg_plain, msg_html, internal_date, msg_labels, msg_references, thread_id FROM %1gmail_msg WHERE acc_id=%2 AND "
				"thread_id IN(SELECT thread_id FROM %1gmail_thread WHERE acc_id=%2 AND thread_id IN(%3) ORDER BY internal_date DESC) "
				"ORDER BY thread_id, internal_date DESC")
				.arg(m_storage->metaPrefix())
				.arg(m_storage->currentAccountId())
				.arg(loaded_threads_id_str);
			QSqlQuery* q = m_storage->selectQuery(sql);
			if (!q)
				return rv;

			thread_ptr thread_in_select = nullptr;
			int loaded_msg = 0;
			while (q->next())
			{
				QString thread_id = q->value(13).toString();
				if (!thread_id.isEmpty()) {
					if (!thread_in_select ||
						thread_in_select->id() != thread_id)
					{
						thread_in_select = m_storage->findThread(thread_id);
					}

					if (thread_in_select) {
						auto md = msg_storage->loadMessageFromDb(thread_in_select, q);
						if (md == nullptr)
							continue;
						mcache->mem_insert(md->id(), md);
						loaded_msg++;
					}
					else {
						qWarning() << "loadThreadsByIdsFromDb/lost thread in DB" << thread_id << "for message" << q->value(1).toString();
					}
				}
			}//while

			//qWarning() << "ykh/loadThreadsByIdsFromDb/batch messages loaded=" << loaded_msg;
		}//load_messages_for_threads

		return rv;
	};

#define BATCH_SIZE 400

	std::vector<QString>    batch_list;
	for (const auto& id : thread_ids) {
		batch_list.push_back(id);
		if (batch_list.size() == BATCH_SIZE)
		{
			QString ids_str = slist2commalist_decorated(batch_list);
			thread_arr batch_arr = process_list(ids_str);
			rv.insert(rv.end(), batch_arr.begin(), batch_arr.end());
			batch_list.clear();
		}
	}

	if (!batch_list.empty())
	{
		QString ids_str = slist2commalist_decorated(batch_list);
		thread_arr batch_arr = process_list(ids_str);
		rv.insert(rv.end(), batch_arr.begin(), batch_arr.end());
	}

#undef BATCH_SIZE

	return rv;
};

mail_cache::thread_ptr mail_cache::GThreadsStorage::loadThread(QSqlQuery* q)
{
    mail_cache::thread_ptr td;

    QString thread_id = q->value(0).toString();
    if (thread_id.isEmpty())
    {
        qWarning() << "ERROR. Invalid(empty) thread_id";
        return nullptr;
    }
    
    quint64 history_id = q->value(1).toULongLong();
    int messages_count = q->value(2).toInt();
    QString snippet = q->value(3).toString();
    auto t_labels = q->value(4).toULongLong();
    
    td = std::shared_ptr<ThreadData>(new ThreadData(thread_id,
                                                    history_id,
                                                    messages_count,
                                                    snippet,
                                                    t_labels));
    if (m_storage->m_lastHistoryId < history_id) {
        m_storage->m_lastHistoryId = history_id;
    }

    return td;
};

void mail_cache::GThreadsStorage::verifyThreads() 
{
    auto cache = m_storage->tcache();
    if (!cache) {
        qWarning() << "cache lock failed";
        return;
    }

    cache->verifyData();
};

QString mail_cache::GThreadsStorage::insertSQL()const 
{
    QString sql_insert = QString("INSERT INTO %1gmail_thread(history_id, messages_count, snippet, t_labels, internal_date, thread_id, acc_id) VALUES(?, ?, ?, ?, ?, ?, %2)")
        .arg(m_storage->metaPrefix())
        .arg(m_storage->currentAccountId());
    return sql_insert;
};

QString mail_cache::GThreadsStorage::updateSQL()const 
{
    QString sql_update = QString("UPDATE %1gmail_thread SET history_id=?, messages_count=?, snippet=?, t_labels=?, internal_date=? WHERE thread_id=? AND acc_id=%2")
        .arg(m_storage->metaPrefix())
        .arg(m_storage->currentAccountId());
    return sql_update;
};

void mail_cache::GThreadsStorage::bindSQL(QSqlQuery* q, CACHE_LIST<ThreadData>& r)
{
    QVariantList history_id, messages_count, snippet, labelsBitMap, internalDate, id;
    for (auto& i : r)
    {
        thread_ptr& t = i;

        thread_ptr& m = i;
        if (!t->head()) {
            qWarning() << "db-insert/skipping 'headless' thread" << m->id();
            continue;
        }

        if (t->internalDate() == 0) {
            qWarning() << "db-insert/skipping invalid date thread" << m->id();
            continue;
        }

        history_id << t->m_history_id;
        messages_count << t->m_messages_count;
        snippet << t->m_snippet;
        labelsBitMap << t->labelsBitMap();
        internalDate << t->internalDate();
        id << t->m_id;

        if (m_storage->m_lastHistoryId < t->m_history_id) {
            m_storage->m_lastHistoryId = t->m_history_id;
        }
    }

    q->addBindValue(history_id);
    q->addBindValue(messages_count);
    q->addBindValue(snippet);
    q->addBindValue(labelsBitMap);
    q->addBindValue(internalDate);
    q->addBindValue(id);
};

bool mail_cache::GThreadsStorage::execOutOfBatchSQL(QSqlQuery* q, mail_cache::ThreadData* t)
{
    q->addBindValue(t->m_history_id);
    q->addBindValue(t->m_messages_count);
    q->addBindValue(t->m_snippet);
    q->addBindValue(t->labelsBitMap());
    q->addBindValue(t->internalDate());
    q->addBindValue(t->m_id);
    return q->exec();
};

bool mail_cache::GThreadsStorage::insertDbInBatch(CACHE_LIST<ThreadData>& r) 
{
    auto q = m_storage->startTransaction(insertSQL());
    if (q) {
        bindSQL(q, r);
        if (!q->execBatch())
        {
            m_storage->rollbackTransaction();
            qWarning() << "ERROR. SQL thread/insert batch failed"
                << "err-type:" << q->lastError().type()
                << "native-code:" << q->lastError().nativeErrorCode()
                << "errtext:" << q->lastError().text();
            return false;
        }

        bool rv = m_storage->commitTransaction();
        if (rv) {
            for (auto& i : r)
            {
                i->markDbRecord();
            }
        }
        return rv;
    }

    return false;
};

bool mail_cache::GThreadsStorage::updateDbInBatch(CACHE_LIST<ThreadData>& r) 
{
    auto q = m_storage->startTransaction(updateSQL());
    if (q) {
        bindSQL(q, r);

        if (!q->execBatch())
        {
            m_storage->rollbackTransaction();
            qWarning() << "ERROR. SQL update/insert batch failed"
                << "err-type:" << q->lastError().type()
                << "native-code:" << q->lastError().nativeErrorCode()
                << "errtext:" << q->lastError().text();
            return false;
        }

        return m_storage->commitTransaction();
    }
    return false;
};

void mail_cache::GThreadsStorage::update_db(
    EDataState ,
    CACHE_LIST<mail_cache::ThreadData>& r)
{
    if (!updateDbInBatch(r)) {
        QString sql_update = updateSQL();
        QString sql_insert = insertSQL();

        for (auto& i : r)
        {
            thread_ptr& m = i;
            if (!m->head()) {
                qWarning() << "db-update/skipping 'headless' thread" << m->id();
                continue;
            }

            if (m->internalDate() == 0) {
                qWarning() << "db-update/skipping invalid date thread" << m->id();
                continue;
            }

            QSqlQuery* q = m_storage->prepareQuery(sql_update);
            if (!q)return;
            bool ok = execOutOfBatchSQL(q, m.get());
            if (!ok)
            {
                qWarning() << "ERROR. SQL thread/update failed"
                    << "err-type:" << q->lastError().type()
                    << "native-code:" << q->lastError().nativeErrorCode()
                    << "errtext:" << q->lastError().text()
                    << "threadid=" << m->id();
            }
            int rows_updated = q->numRowsAffected();
            if (rows_updated <= 0)
            {
                q = m_storage->prepareQuery(sql_insert);
                if (!q)return;
                ok = execOutOfBatchSQL(q, m.get());
                if (!ok)
                {
                    qWarning() << "ERROR. SQL thread/insert failed"
                        << "err-type:" << q->lastError().type()
                        << "native-code:" << q->lastError().nativeErrorCode()
                        << "errtext:" << q->lastError().text()
                        << "threadid=" << m->id();
                }
            }
        }
    }
};

void mail_cache::GThreadsStorage::insert_db(EDataState, CACHE_LIST<ThreadData>& r)
{
    if (!insertDbInBatch(r)) {
        QString sql_insert = insertSQL();
        QString sql_update = updateSQL();

        for (auto& i : r)
        {
            thread_ptr& m = i;
            if (!m->head()) {
                qWarning() << "db-insert/skipping 'headless' thread" << m->id();
                continue;
            }

            if (m->internalDate() == 0) {
                qWarning() << "db-insert/skipping invalid date thread" << m->id();
                continue;
            }

            QSqlQuery* q = m_storage->prepareQuery(sql_insert);
            if (!q)return;
            bool ok = execOutOfBatchSQL(q, m.get());
            if (!ok) {
                if (q->lastError().nativeErrorCode() == "19") {
                    ///constraint error - duplicate record, do update instead
                    q = m_storage->prepareQuery(sql_update);
                    if (!q)return;
                    ok = execOutOfBatchSQL(q, i.get());
                    if (!ok)
                    {
                        qWarning() << "ERROR. SQL update failed"
                            << "err-type:" << q->lastError().type()
                            << "native-code:" << q->lastError().nativeErrorCode()
                            << "errtext:" << q->lastError().text()
                            << "ID:" << i->id();
                    }
                }
                else {
                    qWarning() << "ERROR. SQL insert failed"
                        << "err-type:" << q->lastError().type()
                        << "native-code:" << q->lastError().nativeErrorCode()
                        << "errtext:" << q->lastError().text()
                        << "ID:" << i->id();
                }
            }
        }
    }
};

void mail_cache::GThreadsStorage::remove_db(const std::set<QString>& set_of_ids2remove)
{
    STRING_LIST ids2remove;
    for (auto& i : set_of_ids2remove)
    {
        ids2remove.push_back(i);
    }

    std::function<bool(const STRING_LIST& lst)> removeSublist = [&](const STRING_LIST& lst) -> bool
    {
        QString comma_ids = slist2commalist_decorated(lst);
        QString sql = QString("DELETE FROM %1gmail_thread WHERE thread_id IN(%2) AND acc_id=%3")
            .arg(m_storage->metaPrefix())
            .arg(comma_ids)
            .arg(m_storage->currentAccountId());
        QSqlQuery* q = m_storage->prepareQuery(sql);
        if (!q)return false;
        if (!q->exec()) return false;
        return true;
    };

    if (isValid())
    {
        if (!chunk_list_execution(ids2remove, removeSublist))
        {
            qWarning() << "ERROR. Failed to remove list of records.";
        }
    }    
};

bool mail_cache::GThreadsStorage::isValid()const 
{
    return m_storage->isValid();
};


mail_cache::GQueryStorage::GQueryStorage(GThreadsStorage* s, GMessagesStorage* ms)
{
    m_tstorage = s; 
	m_mstorage = ms;
};

bool mail_cache::GQueryStorage::loadQueriesFromDb()
{
    QString sql = QString("SELECT q_id, q_query, labelid FROM %1gmail_q "
        "WHERE acc_id=%2")
        .arg(m_tstorage->m_storage->metaPrefix())
        .arg(m_tstorage->m_storage->currentAccountId());
    QSqlQuery* q = m_tstorage->m_storage->selectQuery(sql);
    if (!q)
        return false;

    int loaded_objects = 0;
    while (q->next())
    {
        int q_id = q->value(0).toInt();
        QString q_str = q->value(1).toString();
        QString lbid = q->value(2).toString();
        auto qd = std::shared_ptr<QueryData>(new QueryData(q_id, q_str, lbid));
        m_qmap[QueryData::format_qhash(q_str, lbid)] = qd;
        m_q_dbmap[q_id] = qd;
        loaded_objects++;
    }

#ifdef API_QT_AUTOTEST
    ApiAutotest::INSTANCE() << QString("queries/DB-loaded %1 records")
        .arg(loaded_objects);
#endif

    return true;
};

bool mail_cache::GQueryStorage::loadQueryThreadsFromDb(query_ptr q)
{
	/// actually we have to assume 
	/// q->m_qthreads is empty
	/// q->m_tmap is empty
	/// todo: put in some assert here

    if(q->m_threads_db_loaded){
        return true;
    }
    
	auto tstorage = m_tstorage->m_storage;
    auto tc = tstorage->tcache();
    if (!tc) {
        return false;
    }

	auto mstorage = m_mstorage->m_storage;
	auto mc = mstorage->mcache();
	if (!mc) {
		return false;
	}

	//ykh+block
	
	QString sql = QString("SELECT r.thread_id, t.history_id, t.messages_count, t.snippet, t.t_labels, "
		"m.msg_state, m.msg_id, m.msg_from, m.msg_to, m.msg_cc, m.msg_bcc, "
		"m.msg_subject, m.msg_snippet, m.msg_plain, m.msg_html, m.internal_date, m.msg_labels, m.msg_references "
		"FROM %1gmail_qres r, %1gmail_thread t, %1gmail_msg m "
		"WHERE t.thread_id = r.thread_id AND m.thread_id = t.thread_id AND r.q_id = %2 "
		"LIMIT 10000")
		.arg(tstorage->metaPrefix())
		.arg(q->m_db_id);

	QSqlQuery* qres = tstorage->selectQuery(sql);
	if (!qres)
		return false;

	int loaded_threads = 0;
	int located_threads = 0;
	int loaded_msg = 0;
	int located_msg = 0;
	while (qres->next())
	{		
		QString thread_id = qres->value(0).toString();
		QString msg_id = qres->value(6).toString();
		auto t = tc->mem_object(thread_id);
		if (t) {
			located_threads++;
		}
		else {
			t = m_tstorage->loadThread(qres);
			if (t == nullptr)
				continue;
			tc->mem_insert(t->id(), t);
			loaded_threads++;
		}

		if (t) {

			auto it2 = q->m_tmap.find(thread_id);
			if (it2 == q->m_tmap.end()) {
				q->m_qthreads.push_back(t);
				q->m_tmap[thread_id] = t;
			}

			/// link messages to it
			auto m = mc->mem_object(msg_id);
			if (m) {
				located_msg++;
			}else{
				m = m_mstorage->loadMessageFromDb(t, qres, 5);
				if (m) {
					mc->mem_insert(m->id(), m);
					loaded_msg++;
				}
			}
		}
	}

#ifdef API_QT_AUTOTEST
	QString sdbg = QString("Q/ +t=%1 !t=%2 +m=%3 !m=%4 t-in-q=%5")
		.arg(loaded_threads)
		.arg(located_threads)
		.arg(loaded_msg)
		.arg(located_msg)
		.arg(q->m_qthreads.size());

	ApiAutotest::INSTANCE() << sdbg;
	qWarning() << sdbg;
#endif
	// sort on thead date
	if (!q->m_qthreads.empty()) {
		std::sort(q->m_qthreads.begin(), q->m_qthreads.end(), [&](mail_cache::thread_ptr t1, mail_cache::thread_ptr t2)
		{
			if (!t1)
				return false;
			if (!t2)
				return false;

			bool rv = (t1->internalDate() > t2->internalDate());
			return rv;
		});
	}
	q->m_threads_db_loaded = true;
	return true;
};

mail_cache::query_ptr mail_cache::GQueryStorage::ensure_q(QString q_str, QString labelid)
{
    auto i = m_qmap.find(QueryData::format_qhash(q_str, labelid));
    if(i != m_qmap.end()){
        auto qd = i->second;
        if(!qd->m_threads_db_loaded){
            if(!loadQueryThreadsFromDb(qd)){
                qWarning() << "Failed to load query from DB" << q_str;
                return nullptr;
            };
        }
        return qd;
    }

    QString sql = QString("INSERT INTO %1gmail_q(acc_id, q_query, labelid) VALUES(%2,?,?)")
        .arg(m_tstorage->m_storage->m_metaPrefix)
        .arg(m_tstorage->m_storage->currentAccountId());
    QSqlQuery* q = m_tstorage->m_storage->prepareQuery(sql);
    if (!q)return nullptr;
    q->addBindValue(q_str);
    q->addBindValue(labelid);
    if (q->exec()) {
        auto q_id = q->lastInsertId().toInt();
        auto qd = std::shared_ptr<QueryData>(new QueryData(q_id, q_str, labelid));
        m_qmap[QueryData::format_qhash(q_str, labelid)] = qd;
        //m_qmap[q_str] = qd;
        m_q_dbmap[q_id] = qd;
        return qd;
    }
    else {
        qWarning() << "ERROR. SQL ensure_q failed"
            << "err-type:" << q->lastError().type()
            << "native-code:" << q->lastError().nativeErrorCode()
            << "errtext:" << q->lastError().text()
            << "q_query=" << q_str;        
    }

    return nullptr;
};

mail_cache::query_ptr mail_cache::GQueryStorage::lookup_q(QString q_str, QString labelid)
{
    auto i = m_qmap.find(QueryData::format_qhash(q_str, labelid));
    if (i == m_qmap.end()) {
        return nullptr;
    }

    auto q = i->second;
    if (q->m_threads_db_loaded) {
        if (!loadQueryThreadsFromDb(q)) {
            qWarning() << "Failed to load query from DB" << q_str << labelid;
            return nullptr;
        };
    }
    return q;
};


QString mail_cache::GQueryStorage::insertSQLthreads(query_ptr q)const
{
    QString sql_insert = QString("INSERT INTO %1gmail_qres(thread_id, acc_id, q_id) VALUES(?, %2, %3)")
        .arg(m_tstorage->m_storage->metaPrefix())
        .arg(m_tstorage->m_storage->currentAccountId())
        .arg(q->m_db_id);
    return sql_insert;
};

void mail_cache::GQueryStorage::bindSQL(QSqlQuery* q, STRING_LIST& r)
{
    QVariantList threadId;
    for (auto& i : r)
    {
        threadId << i;
    }
    q->addBindValue(threadId);
};

void mail_cache::GQueryStorage::insert_db_threads(query_ptr qd)
{
    auto tc = m_tstorage->m_storage->tcache();
    if (!tc) {
        qWarning() << "expected thread storage cache ptr";
        return;
    }

    STRING_LIST  new_threads;
    for (auto& i : qd->m_qnew_thread_ids) {
        if (qd->m_tmap.find(i) == qd->m_tmap.end()) {
            auto t = tc->mem_object(i);
            if (t) {
                new_threads.push_back(i);
                qd->m_qthreads.push_back(t);
                qd->m_tmap[i] = t;
            }
            else {
                qWarning() << "insert_db_threads/failed to locate thread in cache" << i;
            }
        }
    }

    if (qd && !new_threads.empty()) {
        auto q = m_tstorage->m_storage->startTransaction(insertSQLthreads(qd));
        if (q) {
            bindSQL(q, new_threads);
            if (!q->execBatch())
            {
                m_tstorage->m_storage->rollbackTransaction();
                qWarning() << "ERROR. SQL Q/insert-threads batch failed"
                    << "err-type:" << q->lastError().type()
                    << "native-code:" << q->lastError().nativeErrorCode()
                    << "errtext:" << q->lastError().text();
                return;
            }

            bool rv = m_tstorage->m_storage->commitTransaction();
            if (!rv) {
                qWarning() << "Failed commit Q-threads transaction";
            }
        }

        if (!qd->m_qthreads.empty()) {          
            std::sort(qd->m_qthreads.begin(), qd->m_qthreads.end(), [&](mail_cache::thread_ptr t1, mail_cache::thread_ptr t2)
            {
                bool rv = (t1->internalDate() > t2->internalDate());
                return rv;
            });
        }
    }
};

bool mail_cache::GQueryStorage::remove_q(query_ptr q) 
{
    QString sql_res = QString("DELETE FROM %1gmail_qres WHERE q_id=%2 AND acc_id=%3")
        .arg(m_tstorage->m_storage->metaPrefix())
        .arg(q->m_db_id)
        .arg(m_tstorage->m_storage->currentAccountId());
    QSqlQuery* qr = m_tstorage->m_storage->prepareQuery(sql_res);
    if (!qr)return false;
    if (!qr->exec()) return false;

    QString sql_q = QString("DELETE FROM %1gmail_q WHERE q_id=%2 AND acc_id=%3")
        .arg(m_tstorage->m_storage->metaPrefix())
        .arg(q->m_db_id)
        .arg(m_tstorage->m_storage->currentAccountId());
    QSqlQuery* qq = m_tstorage->m_storage->prepareQuery(sql_q);
    if (!qq)return false;
    if (!qq->exec()) return false;

    //        m_qmap[QueryData::format_qhash(q_str, labelid)] = qd;
    //m_qmap.erase(q->qStr());
    m_qmap.erase(QueryData::format_qhash(q->qStr(), q->labelid()));
    m_q_dbmap.erase(q->m_db_id);
    return true;
};
