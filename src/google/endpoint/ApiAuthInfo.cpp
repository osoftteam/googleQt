#include <QJsonObject>
#include <QFile>
#include <iostream>
#include "ApiAuthInfo.h"
#include "ApiUtil.h"

using namespace googleQt;

ApiAuthInfo::ApiAuthInfo() 
{
#ifdef API_QT_AUTOTEST
    m_accessToken = "TEST-TOKEN";
#endif //API_QT_AUTOTEST
};

ApiAuthInfo::ApiAuthInfo(QString token_file, int scope):m_token_file(token_file), m_token_scope(scope)
{
};

void ApiAuthInfo::purge()
{
    if (!m_token_file.isEmpty())
        QFile::remove(m_token_file);
    m_accessToken.clear();
    m_refreshToken.clear();
    m_type.clear();
    m_expires_in = 0;
    m_expire_time = QDateTime();
    m_token_scope = 0;
};

bool ApiAuthInfo::readFromFile(QString path)
{
    QJsonObject js;
    if(!loadJsonFromFile(path, js))
        return false;
    m_accessToken = js["access_token"].toString();
    m_refreshToken = js["refresh_token"].toString();
    m_type = js["token_type"].toString();
    m_expires_in = js["expires_in"].toInt();
    m_expire_time = QDateTime::fromString(js["expire_time"].toString(), Qt::ISODate);
    m_token_scope = js["scope"].toInt();
    return true;
};

bool ApiAuthInfo::storeToFile(QString path)const
{
    QJsonObject js;
    js["access_token"] = m_accessToken;
    js["refresh_token"] =  m_refreshToken;
    js["token_type"] = m_type;
    js["expires_in"] = m_expires_in;
    js["expire_time"] = m_expire_time.toString(Qt::ISODate);
    js["scope"] = m_token_scope;
    
    if(!storeJsonToFile(path, js))
        return false;
    return true;

};

bool ApiAuthInfo::reload()
{
    if(!m_token_file.isEmpty())
        {
            return readFromFile(m_token_file);
        }
    
    return false;
};

bool ApiAuthInfo::updateToken(const QJsonObject& js_in)
{
    m_accessToken = js_in["access_token"].toString();
    QString refreshToken = js_in["refresh_token"].toString();
    if(!refreshToken.isEmpty())
        {
            m_refreshToken = refreshToken;
        }
    m_type = js_in["token_type"].toString();
    m_expires_in = js_in["expires_in"].toString().toInt();
    m_expire_time = QDateTime::currentDateTime().addSecs(m_expires_in);
    //do don't update email - it will be empty on refresh
    //email/userid is something that is setup on client side
    
    if(!m_token_file.isEmpty())
        {
            return storeToFile(m_token_file);
        }
    
    return true;
};
