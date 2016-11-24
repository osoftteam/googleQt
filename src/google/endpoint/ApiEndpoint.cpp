#include "ApiEndpoint.h"

#ifdef API_QT_AUTOTEST
#include "ApiAutotest.h"
#define LOG_REQUEST     ApiAutotest::INSTANCE() << m_last_req_info;                        
#endif

using namespace googleQt;

ApiEndpoint::ApiEndpoint(ApiClient* c):m_client(c)//, m_reply_in_progress(NULL)
{

}

void ApiEndpoint::addAuthHeader(QNetworkRequest& request)
{
    QString bearer = QString("Bearer %1").arg(m_client->getToken());
    request.setRawHeader("Authorization", bearer.toUtf8());
};

void ApiEndpoint::runEventsLoop()const
{
    m_loop.exec();
};

void ApiEndpoint::exitEventsLoop()const
{
    m_loop.exit();
};


void ApiEndpoint::cancelAll()
{
    NET_REPLIES_IN_PROGRESS copy_of_replies = m_replies_in_progress;
    std::for_each(copy_of_replies.begin(), copy_of_replies.end(), [](std::pair<QNetworkReply*, std::shared_ptr<FINISHED_REQ>> p)
    {
        p.first->abort();
    });
};

void ApiEndpoint::registerReply(QNetworkReply* r, std::shared_ptr<FINISHED_REQ> finishedLambda)
{
    QObject::connect(r, &QNetworkReply::downloadProgress, [&](qint64 bytesProcessed, qint64 total) {
        emit m_client->downloadProgress(bytesProcessed, total);
    });

    QObject::connect(r, &QNetworkReply::uploadProgress, [&](qint64 bytesProcessed, qint64 total) {
        emit m_client->uploadProgress(bytesProcessed, total);
    });

    NET_REPLIES_IN_PROGRESS::iterator i = m_replies_in_progress.find(r);
    if (i != m_replies_in_progress.end())
    {
        qWarning() << "Duplicate reply objects registered, map size:" << m_replies_in_progress.size() << r;
    }

    m_replies_in_progress[r] = finishedLambda;
   // m_replies_in_progress.insert(std::make_pair(r, nullptr));
};

void ApiEndpoint::unregisterReply(QNetworkReply* r)
{
    NET_REPLIES_IN_PROGRESS::iterator i = m_replies_in_progress.find(r);
    if (i == m_replies_in_progress.end())
    {
        qWarning() << "Failed to locate reply objects in registered map, map size:" << m_replies_in_progress.size() << r;
    }
    else
    {
        m_replies_in_progress.erase(i);
    }
    r->deleteLater();
};


void ApiEndpoint::updateLastRequestInfo(QString s)
{
    m_last_req_info = s;
};

QNetworkReply* ApiEndpoint::getData(const QNetworkRequest &req)
{
    QString rinfo = "GET " + req.url().toString() + "\n";
    QList<QByteArray> lst = req.rawHeaderList();
    for(QList<QByteArray>::iterator i = lst.begin(); i != lst.end(); i++)
        rinfo += QString("--header %1 : %2 \n").arg(i->constData()).arg(req.rawHeader(*i).constData());

    updateLastRequestInfo(rinfo);
    
#ifdef API_QT_AUTOTEST
    LOG_REQUEST;
    return nullptr;
#else
    QNetworkReply *r = m_con_mgr.get(req);
//    registerReply(r);
    return r;
#endif
};

QNetworkReply* ApiEndpoint::postData(const QNetworkRequest &req, const QByteArray& data)
{
    QString rinfo = "POST " + req.url().toString() + "\n";
    QList<QByteArray> lst = req.rawHeaderList();
    for(QList<QByteArray>::iterator i = lst.begin(); i != lst.end(); i++)
        rinfo += QString("--header %1 : %2 \n").arg(i->constData()).arg(req.rawHeader(*i).constData());
    rinfo += QString("--data %1").arg(data.constData());

    updateLastRequestInfo(rinfo);
    
#ifdef API_QT_AUTOTEST
    LOG_REQUEST;
    return nullptr;
#else
    QNetworkReply *r = m_con_mgr.post(req, data);
//    registerReply(r);
    return r;
#endif
};

QNetworkReply* ApiEndpoint::putData(const QNetworkRequest &req, const QByteArray& data)
{    
    QString rinfo = "PUT " + req.url().toString() + "\n";
    QList<QByteArray> lst = req.rawHeaderList();
    for(QList<QByteArray>::iterator i = lst.begin(); i != lst.end(); i++)
        rinfo += QString("--header %1 : %2 \n").arg(i->constData()).arg(req.rawHeader(*i).constData());
    rinfo += QString("--data %1").arg(data.constData());
    updateLastRequestInfo(rinfo);
    
#ifdef API_QT_AUTOTEST
    LOG_REQUEST;
    return nullptr;
#else
    QNetworkReply *r = m_con_mgr.put(req, data);
//    registerReply(r);
    return r;
#endif
};

QNetworkReply* ApiEndpoint::deleteData(const QNetworkRequest &req)
{    
    QString rinfo = "DELETE " + req.url().toString() + "\n";
    QList<QByteArray> lst = req.rawHeaderList();
    for(QList<QByteArray>::iterator i = lst.begin(); i != lst.end(); i++)
        rinfo += QString("--header %1 : %2 \n").arg(i->constData()).arg(req.rawHeader(*i).constData());

    updateLastRequestInfo(rinfo);
    
#ifdef API_QT_AUTOTEST
    LOG_REQUEST;
    return nullptr;
#else
    QNetworkReply *r = m_con_mgr.deleteResource(req);
//    registerReply(r);
    return r;
#endif
};
