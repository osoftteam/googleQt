#include <QUrlQuery>
#include "GmailRequestArg.h"

using namespace googleQt;
using namespace gmail;


IdArg::IdArg(QString idValue)
    :m_id(idValue)
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


ListArg::ListArg():m_includeSpamTrash(false), m_maxResults(50)
{

};

void ListArg::build(const QString& link_path, QUrl& url)const
{
    UrlBuilder b(link_path, url);
    b.add("includeSpamTrash", m_includeSpamTrash)
        .add("maxResults", m_maxResults)
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
std::unique_ptr<IdArg> IdArg::EXAMPLE()
{
	std::unique_ptr<IdArg> rv(new IdArg);
	rv->setId("id123");
	rv->setFormat("metadata");
	rv->headers().push_back("Subject");
	rv->headers().push_back("From");
	return rv;
};

std::unique_ptr<ListArg> ListArg::EXAMPLE()
{ 
	std::unique_ptr<ListArg> rv(new ListArg); 
	rv->setMaxResults(10);
	rv->setPageToken("nextToken");
	rv->labels() = QString("label1 label2 label3").split(" ");
	return rv; 
};

std::unique_ptr<HistoryListArg> HistoryListArg::EXAMPLE()
{ 
	std::unique_ptr<HistoryListArg> rv(new HistoryListArg); 
	rv->setMaxResults(10);
	rv->setPageToken("nextToken");
	rv->labels() = QString("hlabel1 hlabel2 hlabel3").split(" ");
	return rv; 
};

std::unique_ptr<DraftListArg> DraftListArg::EXAMPLE()
{ 
	std::unique_ptr<DraftListArg> rv(new DraftListArg); 
	rv->setMaxResults(10);
	rv->setPageToken("nextToken");
	rv->setIncludeSpamTrash(true);
	return rv; 
};

std::unique_ptr<SendMessageArg> SendMessageArg::EXAMPLE() 
{
	std::unique_ptr<SendMessageArg> rv(new SendMessageArg);
	rv->setUploadType("media");
	return rv;
};

std::unique_ptr<InsertMessageArg> InsertMessageArg::EXAMPLE()
{
	std::unique_ptr<InsertMessageArg> rv(new InsertMessageArg);
	rv->setUploadType("media");
	return rv;
};

std::unique_ptr<ImportMessageArg> ImportMessageArg::EXAMPLE()
{
	std::unique_ptr<ImportMessageArg> rv(new ImportMessageArg);
	rv->setUploadType("media");
	return rv;
};


#endif //API_QT_AUTOTEST

