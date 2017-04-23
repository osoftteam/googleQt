#include <QUrlQuery>
#include <QFile>
#include "GmailRequestArg.h"

using namespace googleQt;
using namespace gmail;


IdArg::IdArg(QString idValue, QString format)
    :m_id(idValue),
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


AttachmentIdArg::AttachmentIdArg(QString message_id, QString attachment_id) 
	:m_message_id(message_id),
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

ModifyMessageArg::ModifyMessageArg(QString idValue,
                                   QString add_label /*= ""*/,
                                   QString remove_label /*= ""*/)
{
    m_message_id = idValue;
    if(!add_label.isEmpty()){
        m_addLabels.push_back(add_label);
    }

    if(!remove_label.isEmpty()){
        m_removeLabels.push_back(remove_label);
    }
}


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


ListArg::ListArg():m_includeSpamTrash(false), m_maxResults(50)
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


HistoryListArg::HistoryListArg(int startHistoryId):
    m_maxResults(50),
    m_startHistoryId(startHistoryId)
{

};

void HistoryListArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
    b.add("startHistoryId", m_startHistoryId)
        .add("pageToken", m_pageToken);
    if (m_labelIds.size() > 0) {
        for (QStringList::const_iterator i = m_labelIds.cbegin(); i != m_labelIds.cend(); i++)
            {
                b.add("labelIds", *i);
            }
    }
}

DraftListArg::DraftListArg() :m_maxResults(50)
{

};

void DraftListArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
    b.add("includeSpamTrash", m_includeSpamTrash)
        .add("maxResults", m_maxResults)
        .add("q", m_q)
        .add("pageToken", m_pageToken);
}

SendMessageArg::SendMessageArg()
{
    m_uploadType = "media";
};

void SendMessageArg::build(const QString& link_path, QUrl& url)const 
{
    UrlBuilder b(link_path + QString("/%1").arg(path_send::path()), url);
    b.add("uploadType", m_uploadType);
};

//...
void MimeBodyPart::setContent(QString val, QString _type)
{
	m_type = _type;
	m_content = val;
};

SendMimeMessageArg::SendMimeMessageArg()
{
	m_uploadType = "media";
};

SendMimeMessageArg::SendMimeMessageArg(QString from, 
	QString to, 
	QString subject, 
	QString text)
	:m_From(from),
	m_To(to),
	m_Subject(subject)
{
	MimeBodyPart pt;
	pt.setContent(text, "text/plain");
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
	MimeBodyPart pt1;
	pt1.setContent(text_plain, "text/plain");
	addBodyPart(pt1);

	MimeBodyPart pt2;
	pt2.setContent(text_plain, "text/html");
	addBodyPart(pt2);
};


void SendMimeMessageArg::build(const QString& link_path, QUrl& url)const
{
	UrlBuilder b(link_path + QString("/send"), url);
	b.add("uploadType", m_uploadType);
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
	rv =  QString("From: %1\r\n").arg(m_From).toStdString().c_str();
	rv += QString("To: %1\r\n").arg(m_To);
	rv += QString("Subject: %1\r\n").arg(m_Subject);
	rv += QString("MIME-Version: 1.0\r\n");
	rv += QString("Content-Type: multipart/alternative; boundary=\"%1\"\r\n\r\n").arg(boundary);
	for (auto& p : m_body_parts)
	{
		rv += QString("--%1\r\n").arg(boundary);
		rv += QString("Content-Type: %1; charset=UTF-8\r\n").arg(p.m_type);
		rv += QString("Content-Transfer-Encoding: base64\r\n\r\n");
		QByteArray ba(p.m_content.toStdString().c_str());
		rv += QString("%1\r\n").arg(ba.toBase64(QByteArray::Base64Encoding).constData());
	}

	rv += QString("--%1--").arg(boundary);

	return rv;
};

void SendMimeMessageArg::toJson(QJsonObject& js)const
{
	QByteArray data(toRfc822());
	QString res = data.toBase64(QByteArray::Base64UrlEncoding);
	js["raw"] = res;
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
	std::unique_ptr<ModifyMessageArg> rv(new ModifyMessageArg("id123"));
	std::list <QString> add_label, remove_label;
	add_label.push_back("LABEL_ADD_1");
	add_label.push_back("LABEL_ADD_2");
	remove_label.push_back("LABEL_DEL_1");
	remove_label.push_back("LABEL_DEL_2");
	rv->setAddlabels(add_label);
	rv->setRemovelabels(remove_label);
	return rv;
};

std::unique_ptr<HistoryListArg> HistoryListArg::EXAMPLE(int, int)
{ 
    std::unique_ptr<HistoryListArg> rv(new HistoryListArg); 
    rv->setMaxResults(10);
    rv->setPageToken("nextToken");
    rv->labels() = QString("hlabel1 hlabel2 hlabel3").split(" ");
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

std::unique_ptr<SendMessageArg> SendMessageArg::EXAMPLE(int, int)
{
    std::unique_ptr<SendMessageArg> rv(new SendMessageArg);
    rv->setUploadType("media");
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
	rv->setFrom("from_me@gmail.com");
	rv->setTo("to_somebody@gmail.com");
	rv->setCC("cc_somebody@gmail.com");
	rv->setBCC("bcc_somebody@gmail.com");
	rv->setUploadType("multipart");

	MimeBodyPart pt1;
	pt1.setContent("*My example message text*", "text/plain");
	rv->addBodyPart(pt1);

	MimeBodyPart pt2;
	pt2.setContent("<b>My example message text</b>", "text/html");
	rv->addBodyPart(pt2);

	return rv;
};

#endif //API_QT_AUTOTEST

