#include "ApiEndpoint.h"

#ifdef API_QT_AUTOTEST
#include "ApiAutotest.h"
#define LOG_REQUEST     ApiAutotest::INSTANCE() << m_last_req_info;                        
#endif

using namespace googleQt;

ApiEndpoint::ApiEndpoint(ApiClient* c):m_client(c), m_reply_in_progress(NULL)
{

}

void ApiEndpoint::addAuthHeader(QNetworkRequest& request)
{
    QString bearer = QString("Bearer %1").arg(m_client->getToken());
    request.setRawHeader("Authorization", bearer.toUtf8());
};

void ApiEndpoint::execEventLoop(QNetworkReply *reply)
{
    m_reply_in_progress = reply;
    m_loop.exec();
};

void ApiEndpoint::exitEventLoop(QNetworkReply *reply)
{
    EXPECT((m_reply_in_progress == reply), QString("Expected stored reply object."));
    m_reply_in_progress = NULL;
    reply->deleteLater();
    m_loop.exit();
};

void ApiEndpoint::cancel()
{
    if(m_reply_in_progress != NULL){
        m_reply_in_progress->abort();
    }
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
    QNetworkReply *reply = m_con_mgr.get(req);
    return reply;
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
    QNetworkReply *reply = m_con_mgr.post(req, data);
    return reply;
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
    QNetworkReply *reply = m_con_mgr.put(req, data);
    return reply;
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
    QNetworkReply *reply = m_con_mgr.deleteResource(req);
    return reply;
#endif
};
