#include <QUrlQuery>
#include <QFile>
#include <QFileInfo>
#include "GmailRequestArg.h"

using namespace googleQt;
using namespace gmail;


IdArg::IdArg(QString userIdValue, QString idValue, QString format)
    :m_userId(userIdValue),
    m_id(idValue),
    m_format(format)
{

};

void IdArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/%1").arg(m_id), url);
    b.add("format", m_format);
    for (QStringList::const_iterator i = m_headers.cbegin(); i != m_headers.cend(); i++)
        {
            b.add("metadataHeaders", *i);
        }   
};


AttachmentIdArg::AttachmentIdArg(QString user_id, QString message_id, QString attachment_id)
    :m_userId(user_id),
    m_message_id(message_id),
    m_attachment_id(attachment_id)
{
};

void AttachmentIdArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("%1/attachments/%2")
                 .arg(m_message_id)
                 .arg(m_attachment_id),
                 url);
}

ModifyMessageArg::ModifyMessageArg(QString user_id, QString message_id)
{
    m_userId = user_id;
    m_message_id = message_id;
}


void ModifyMessageArg::addAddLabel(QString name)
{
    if(!name.isEmpty()){
        m_addLabels.push_back(name);
    }
};

void ModifyMessageArg::addRemoveLabel(QString name)
{
    if(!name.isEmpty()){
        m_removeLabels.push_back(name);
    }
};


void ModifyMessageArg::build(const QString& link_path, QUrl& url)const 
{
    UrlBuilder b(link_path + QString("/%1/modify").arg(m_message_id), url);

};

void ModifyMessageArg::toJson(QJsonObject& js)const 
{
    js["addLabelIds"] = ingrl_list2jsonarray(m_addLabels);
    js["removeLabelIds"] = ingrl_list2jsonarray(m_removeLabels);
};

ModifyMessageArg::operator QJsonObject()const {
    QJsonObject js;
    this->toJson(js);
    return js;
}

BatchModifyMessageArg::BatchModifyMessageArg(QString user_id)
{
    m_userId = user_id;
}

void BatchModifyMessageArg::addAddLabel(QString name)
{
    if (!name.isEmpty()) {
        m_addLabels.push_back(name);
    }
};

void BatchModifyMessageArg::addRemoveLabel(QString name)
{
    if (!name.isEmpty()) {
        m_removeLabels.push_back(name);
    }
};


void BatchModifyMessageArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/batchModify"), url);

};

void BatchModifyMessageArg::toJson(QJsonObject& js)const
{
    js["ids"] = ingrl_list2jsonarray(m_ids);
    js["addLabelIds"] = ingrl_list2jsonarray(m_addLabels);
    js["removeLabelIds"] = ingrl_list2jsonarray(m_removeLabels);
};

BatchModifyMessageArg::operator QJsonObject()const {
    QJsonObject js;
    this->toJson(js);
    return js;
}

void BatchModifyMessageArg::print()const
{
    qDebug() << "-- ids ---";
    for (auto i : m_ids) {
        qDebug() << "    " << i;
    }
    qDebug() << "-- add ---";
    for (auto i : m_addLabels) {
        qDebug() << "    " << i;
    }
    qDebug() << "-- rem ---";
    for (auto i : m_removeLabels) {
        qDebug() << "    " << i;
    }
};

ListArg::ListArg()
{

};

ListArg::ListArg(QString user_id):m_userId(user_id)
{

};

void ListArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
    b.add("includeSpamTrash", m_includeSpamTrash)
        .add("maxResults", m_maxResults)
        .add("pageToken", m_pageToken)
        .add("q", m_q);
    for (QStringList::const_iterator i = m_labelIds.cbegin(); i != m_labelIds.cend(); i++)
        {
            b.add("labelIds", *i);
        }
};


HistoryListArg::HistoryListArg(QString userId, int startHistoryId):
    m_startHistoryId(startHistoryId),
    m_userId(userId),
    m_maxResults(50)    
{

};

void HistoryListArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
    b.add("startHistoryId", m_startHistoryId)
        .add("pageToken", m_pageToken)
        .add("historyTypes", m_historyTypes)
        .add("labelId", m_labelId)
        .add("maxResults", m_maxResults);
    /*
    if (m_labelIds.size() > 0) {
        for (QStringList::const_iterator i = m_labelIds.cbegin(); i != m_labelIds.cend(); i++)
            {
                b.add("labelIds", *i);
            }
    }
    */
}


DraftListArg::DraftListArg()
{

};

DraftListArg::DraftListArg(QString user_id) 
{
    m_userId = user_id;
};

void DraftListArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
    b.add("includeSpamTrash", m_includeSpamTrash)
        .add("maxResults", m_maxResults)
        .add("q", m_q)
        .add("pageToken", m_pageToken);
}

MimeBodyPart MimeBodyPart::makePlainPart(QString text) 
{
    MimeBodyPart pt;
    pt.setContent(text, "text/plain; charset=UTF-8");
    pt.m_part_type = ptypePlain;
    return pt;
};

MimeBodyPart MimeBodyPart::makeHtmlPart(QString text) 
{
    MimeBodyPart pt;
    pt.setContent(text, "text/html; charset=UTF-8");
    pt.m_part_type = ptypeHtml;
    return pt;
};

MimeBodyPart MimeBodyPart::makeFilePart(QString file_path) 
{
    QFileInfo fi(file_path);
    QString file_name = fi.fileName();

    MimeBodyPart pt;
    pt.setContent(file_path, QString("application/octet-stream; name=%1").arg(file_name));
    pt.m_part_type = ptypeFile;
    return pt;
};


void MimeBodyPart::setContent(QString content, QString _type)
{
    m_content = content;
    m_content_type = _type;
};

QByteArray MimeBodyPart::toRfc822()const
{
    QByteArray rv = QString("Content-Type: %1\r\n").arg(m_content_type).toLocal8Bit();
    rv += "Content-Transfer-Encoding: base64\r\n";
    switch(m_part_type)
        {
        case ptypePlain:
        case ptypeHtml: 
            {       
                QByteArray ba(m_content.toStdString().c_str());
                rv += QString("%1\r\n").arg(ba.toBase64(QByteArray::Base64Encoding).constData()).toLocal8Bit();
            }break;
        case ptypeNone:break;
        case ptypeFile: 
            {
                /// put the whole file here
                QFile mf(m_content);
                if (!mf.open(QFile::ReadOnly)) {
                    qWarning() << "Error, failed to open attachment file: " << m_content;
                }
                else 
                {
                    
                    auto x_attachment_id = QString("f_%1").arg(qHash(m_content), 0, 16);
                    auto content_id = QString("<%1>").arg(x_attachment_id);
                    /*
                    rv += QString("Content-ID: %1\r\n").arg(content_id);
                    */
                    rv += QString("X-Attachment-Id: %1\r\n").arg(x_attachment_id).toLocal8Bit();

                    QFileInfo fi(m_content);
                    QString file_name = fi.fileName();
                    rv += QString("Content-Disposition: attachment; filename=%1\r\n\r\n").arg(file_name).toLocal8Bit();

                    QByteArray ba = mf.readAll().toBase64(QByteArray::Base64Encoding);
                    rv += QString("%1\r\n").arg(ba.constData()).toLocal8Bit();
                }       
            }break;
        }
    return rv;
};

SendMimeMessageArg::SendMimeMessageArg()
{
};

SendMimeMessageArg::SendMimeMessageArg(QString from, 
                                       QString to, 
                                       QString subject, 
                                       QString text)
    :m_From(from),
     m_To(to),
     m_Subject(subject)
{
    MimeBodyPart pt = MimeBodyPart::makePlainPart(text);
    addBodyPart(pt);
};

SendMimeMessageArg::SendMimeMessageArg(QString from,
                                       QString to,
                                       QString subject,
                                       QString text_plain,
                                       QString text_html)
    :m_From(from),
     m_To(to),
     m_Subject(subject)
{
    MimeBodyPart pt = MimeBodyPart::makePlainPart(text_plain);
    addBodyPart(pt);

    MimeBodyPart pt_html = MimeBodyPart::makeHtmlPart(text_html);
    addBodyPart(pt_html);
};

void SendMimeMessageArg::addAttachments(const STRING_LIST& attachments) 
{
    for (auto& i : attachments) {
        if (QFile::exists(i)) {
            MimeBodyPart pt = MimeBodyPart::makeFilePart(i);
            addBodyPart(pt);
        }
    }
};

void SendMimeMessageArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/send"), url);
};

QByteArray SendMimeMessageArg::toRfc822()const
{
    if (!m_rawRfc822MessageFile.isEmpty()) {
        QFile mf(m_rawRfc822MessageFile);
        if (!mf.open(QFile::ReadOnly)) {
            qWarning() << "Error, failed to open prepared message file: " << m_rawRfc822MessageFile;
            return QByteArray();
        }

        QByteArray ba = mf.readAll();
        return ba;
    }

    static QString boundary("OooOOoo17gqt");

    QByteArray rv;
    rv =  QString("From: %1\r\n").arg(m_From).toLocal8Bit();
    rv += QString("To: %1\r\n").arg(m_To).toLocal8Bit();
    rv += QString("Subject: %1\r\n").arg(m_Subject).toLocal8Bit();
    QString ref_str = m_references;
    if (!m_InReplyToMsgId.isEmpty()) {
        rv += QString("In-Reply-To: <%1@mail.gmail.com>\r\n").arg(m_InReplyToMsgId).toLocal8Bit();
        ref_str += QString("<%1@mail.gmail.com>").arg(m_InReplyToMsgId).toLocal8Bit();
    }
    
    if (!ref_str.isEmpty()) {
        rv += QString("References: %1\r\n").arg(ref_str).toLocal8Bit();
    }
    rv += "MIME-Version: 1.0\r\n";
    rv += QString("Content-Type: multipart/mixed; boundary=\"%1\"\r\n\r\n").arg(boundary).toLocal8Bit();
    //rv += QString("Content-Type: multipart/alternative; boundary=\"%1\"\r\n\r\n").arg(boundary).toLocal8Bit();
    for (auto& p : m_body_parts)
        {
            rv += QString("--%1\r\n").arg(boundary).toLocal8Bit();
            QString part_content = p.toRfc822();
            rv += part_content.toLocal8Bit();
        }

    rv += QString("--%1--").arg(boundary).toLocal8Bit();

    return rv;
};

bool SendMimeMessageArg::saveAsRfc822(QString filePathName)const
{
    QFile file_in(filePathName);
    if (!file_in.open(QFile::WriteOnly|QIODevice::Truncate)) {
        qWarning() << "Error opening file: " << filePathName;
        return false;
    }
    file_in.write(toRfc822());
    file_in.close();
    return true;
};

void SendMimeMessageArg::toJson(QJsonObject& js)const
{
    QByteArray data(toRfc822());
    QString res = data.toBase64(QByteArray::Base64UrlEncoding);
    js["raw"] = res;
    if (!m_threadId.isEmpty()) {
        js["threadId"] = m_threadId;
    }
};

SendMimeMessageArg::operator QJsonObject()const {
    QJsonObject js;
    this->toJson(js);
    return js;
}

InsertMessageArg::InsertMessageArg() 
{
    m_deleted = false;
    m_internalDateSource = "receivedTime";
    m_uploadType = "media";
};


void InsertMessageArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/%1").arg(path_insert::path()), url);
    b.add("uploadType", m_uploadType)
        .add("deleted", m_deleted)
        .add("internalDateSource", m_internalDateSource);
};


ImportMessageArg::ImportMessageArg()
{
    m_deleted = false;
    m_internalDateSource = "receivedTime";
    m_neverMarkSpam = false;
    m_processForCalendar = false;
    m_uploadType = "media";
};

void ImportMessageArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path + QString("/%1").arg(path_import::path()), url);
    b.add("uploadType", m_uploadType)
        .add("deleted", m_deleted)
        .add("internalDateSource", m_internalDateSource)
        .add("neverMarkSpam", m_neverMarkSpam)
        .add("processForCalendar", m_processForCalendar);
};


#ifdef API_QT_AUTOTEST
std::unique_ptr<IdArg> IdArg::EXAMPLE(int, int)
{
    std::unique_ptr<IdArg> rv(new IdArg);
    rv->setId("id123");
    rv->setFormat("metadata");
    rv->headers().push_back("Subject");
    rv->headers().push_back("From");
    return rv;
};

std::unique_ptr<AttachmentIdArg> AttachmentIdArg::EXAMPLE(int, int)
{
    std::unique_ptr<AttachmentIdArg> rv(new AttachmentIdArg);
    rv->setMessageId("id123");
    rv->setAttachmentId("id_attch456");
    return rv;
};


std::unique_ptr<ListArg> ListArg::EXAMPLE(int, int)
{ 
    std::unique_ptr<ListArg> rv(new ListArg); 
    rv->setMaxResults(10);
    rv->setPageToken("nextToken");
    rv->labels() = QString("label1 label2 label3").split(" ");
    return rv; 
};

std::unique_ptr<ModifyMessageArg> ModifyMessageArg::EXAMPLE(int, int)
{
    std::unique_ptr<ModifyMessageArg> rv(new ModifyMessageArg(ApiAutotest::INSTANCE().userId(), "id123"));
    std::vector <QString> add_label, remove_label;
    add_label.push_back("LABEL_ADD_1");
    add_label.push_back("LABEL_ADD_2");
    remove_label.push_back("LABEL_DEL_1");
    remove_label.push_back("LABEL_DEL_2");
    rv->setAddlabels(add_label);
    rv->setRemovelabels(remove_label);
    return rv;
};

std::unique_ptr<BatchModifyMessageArg> BatchModifyMessageArg::EXAMPLE(int, int)
{
    std::unique_ptr<BatchModifyMessageArg> rv(new BatchModifyMessageArg(ApiAutotest::INSTANCE().userId()));
    std::vector <QString> ids, add_label, remove_label;
    ids.push_back("id1");
    ids.push_back("id2");
    add_label.push_back("LABEL_ADD_1");
    add_label.push_back("LABEL_ADD_2");
    remove_label.push_back("LABEL_DEL_1");
    remove_label.push_back("LABEL_DEL_2");
    rv->setIds(ids);
    rv->setAddlabels(add_label);
    rv->setRemovelabels(remove_label);
    return rv;
};


std::unique_ptr<HistoryListArg> HistoryListArg::EXAMPLE(int, int)
{ 
    std::unique_ptr<HistoryListArg> rv(new HistoryListArg(ApiAutotest::INSTANCE().userId()));
    rv->setMaxResults(10);
    rv->setPageToken("nextToken");
    rv->setLabelId("hlabel1");
    rv->setMaxResults(100);
    rv->setHistoryTypes("messageAdded");
    return rv; 
};

std::unique_ptr<DraftListArg> DraftListArg::EXAMPLE(int, int)
{ 
    std::unique_ptr<DraftListArg> rv(new DraftListArg); 
    rv->setMaxResults(10);
    rv->setPageToken("nextToken");
    rv->setIncludeSpamTrash(true);
    return rv; 
};

std::unique_ptr<InsertMessageArg> InsertMessageArg::EXAMPLE(int, int)
{
    std::unique_ptr<InsertMessageArg> rv(new InsertMessageArg);
    rv->setUploadType("media");
    return rv;
};

std::unique_ptr<ImportMessageArg> ImportMessageArg::EXAMPLE(int, int)
{
    std::unique_ptr<ImportMessageArg> rv(new ImportMessageArg);
    rv->setUploadType("media");
    return rv;
};

std::unique_ptr<SendMimeMessageArg> SendMimeMessageArg::EXAMPLE(int, int)
{
    std::unique_ptr<SendMimeMessageArg> rv(new SendMimeMessageArg);
    rv->setSubject("subject sample");
    rv->setFrom(ApiAutotest::INSTANCE().userId());
    rv->setTo("to_somebody@gmail.com");
    rv->setCC("cc_somebody@gmail.com");
    rv->setBCC("bcc_somebody@gmail.com");

    MimeBodyPart pt_plain = MimeBodyPart::makePlainPart("*My example message text*");
    rv->addBodyPart(pt_plain);

    MimeBodyPart pt_html = MimeBodyPart::makeHtmlPart("<b>My example message text</b>");
    rv->addBodyPart(pt_html);

    return rv;
};

std::unique_ptr<TrashMessageArg> TrashMessageArg::EXAMPLE(int, int)
{ 
    std::unique_ptr<TrashMessageArg> rv(new TrashMessageArg(ApiAutotest::INSTANCE().userId(), "100"));
    return rv; 
};

std::unique_ptr<UntrashMessageArg> UntrashMessageArg::EXAMPLE(int, int)
{ 
    std::unique_ptr<UntrashMessageArg> rv(new UntrashMessageArg(ApiAutotest::INSTANCE().userId(), "100"));
    return rv; 
};

#endif //API_QT_AUTOTEST

