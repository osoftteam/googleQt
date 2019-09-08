#include <QUrl>
#include <QUrlQuery>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <iostream>
#include "GoogleWebAuth.h"
#include "GoogleHost.h"
#include "ApiException.h"

using namespace googleQt;

QUrl GoogleWebAuth::getCodeAuthorizeUrl(const ApiAppInfo* appInfo, QString scope)
{
    QUrl url(QString("https://%1/%2").arg(GoogleHost::DEFAULT().getAuth()).arg("o/oauth2/auth"));
    QUrlQuery q;
    q.addQueryItem("response_type", "code");
    q.addQueryItem("client_id", appInfo->getKey());
    q.addQueryItem("redirect_uri", "urn:ietf:wg:oauth:2.0:oob");
    q.addQueryItem("scope", scope);
    
    url.setQuery(q);
    return url;
};

QUrl GoogleWebAuth::getCodeAuthorizeUrl(const ApiAppInfo* appInfo, const STRING_LIST& scopes)
{
    QString scope_summary;

    for(STRING_LIST::const_iterator i = scopes.begin(); i != scopes.end();i++)
        {
            scope_summary += *i;
            scope_summary += "+";
        }
    scope_summary = scope_summary.left(scope_summary.length() - 1);

    return getCodeAuthorizeUrl(appInfo, scope_summary);
};

bool GoogleWebAuth::updateToken(const QUrl& url, ApiAuthInfo* auth, const QString& str)
{
#ifdef API_QT_AUTOTEST
    Q_UNUSED(url);
    Q_UNUSED(str);

    QJsonObject js;
    js["access_token"]  = "access_token_value_123";
    js["refresh_token"] = "refresh_token_value_456";
    js["token_type"]    = "my_token_type";
    js["expires_in"]    = QDateTime::currentDateTime().toString(Qt::ISODate);;
    js["expire_time"]   = QDateTime::currentDateTime().toString(Qt::ISODate);;
    js["update_time"]   = QDateTime::currentDateTime().toString(Qt::ISODate);

    bool rv = auth->updateToken(js);
    return rv;
#else
    QNetworkAccessManager mgr;
    QEventLoop            loop;    
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    
    bool ok = false, rv = false;
    std::string errorInfo;
    int status_code = 0;    
    QNetworkReply *reply = mgr.post(req, str.toUtf8());
    QObject::connect(reply, &QNetworkReply::finished, [&]()
                     {
                         status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                         switch(status_code)
                             {
                             case 200:
                                 {
                                     QByteArray data = reply->readAll();
                                     if(!data.isEmpty())
                                         {
                                             QJsonDocument doc = QJsonDocument::fromJson(data);
                                             QJsonObject js_in = doc.object();
                                             rv = auth->updateToken(js_in);
                                             ok = true;
                                         }              
                                 }break;
                             default:
                                 {
                                     QByteArray data = reply->readAll();
                                     QString tmp = QString("ERROR. Unexpected status %1").arg(status_code);
                                     tmp += data;
                                     errorInfo = tmp.toStdString();                
                                 }break;
                             }
                         reply->deleteLater();
                         loop.exit();
                     });
    loop.exec();
    if(!ok)
      {
    throw GoogleException(errorInfo, status_code, "");
      }        
    return rv;
#endif
}

bool GoogleWebAuth::getTokenFromCode(const ApiAppInfo* appInfo, QString code, ApiAuthInfo* auth)
{
    QUrl url(QString("https://%1/%2").arg(GoogleHost::DEFAULT().getAuth()).arg("o/oauth2/token"));
    QString str = QString("code=%1&client_id=%2&client_secret=%3&grant_type=%4&redirect_uri=%5")
        .arg(code)
        .arg(appInfo->getKey())
        .arg(appInfo->getSecret())
        .arg("authorization_code")
        .arg("urn:ietf:wg:oauth:2.0:oob");

    return updateToken(url, auth, str);
};

bool GoogleWebAuth::refreshToken(const ApiAppInfo* appInfo, ApiAuthInfo* auth)
{
    QUrl url(QString("https://%1/%2").arg(GoogleHost::DEFAULT().getAuth()).arg("o/oauth2/token"));
    QString str = QString("refresh_token=%1&client_id=%2&client_secret=%3&grant_type=%4")
        .arg(auth->getRefreshToken())
        .arg(appInfo->getKey())
        .arg(appInfo->getSecret())
        .arg("refresh_token");

    return updateToken(url, auth, str);
};

#define DEFINE_SCOPE(N, L) QString GoogleWebAuth::N(){return L;};

DEFINE_SCOPE(authScope_gmail_labels,    "https://www.googleapis.com/auth/gmail.labels");
DEFINE_SCOPE(authScope_gmail_readonly,  "https://www.googleapis.com/auth/gmail.readonly");
DEFINE_SCOPE(authScope_gmail_compose,   "https://www.googleapis.com/auth/gmail.compose");
DEFINE_SCOPE(authScope_gmail_send,      "https://www.googleapis.com/auth/gmail.send");
DEFINE_SCOPE(authScope_gmail_modify,    "https://www.googleapis.com/auth/gmail.modify");
DEFINE_SCOPE(authScope_full_access,     "https://mail.google.com/");
DEFINE_SCOPE(authScope_tasks,           "https://www.googleapis.com/auth/tasks");
DEFINE_SCOPE(authScope_tasks_readonly,  "https://www.googleapis.com/auth/tasks.readonly");
DEFINE_SCOPE(authScope_gdrive,          "https://www.googleapis.com/auth/drive");
DEFINE_SCOPE(authScope_gdrive_readonly, "https://www.googleapis.com/auth/drive.readonly");
DEFINE_SCOPE(authScope_gdrive_appdata,  "https://www.googleapis.com/auth/drive.appdata");
DEFINE_SCOPE(authScope_contacts_modify,  "https://www.google.com/m8/feeds");
DEFINE_SCOPE(authScope_contacts_read_only,  "https://www.googleapis.com/auth/contacts.readonly");

#undef DEFINE_SCOPE
