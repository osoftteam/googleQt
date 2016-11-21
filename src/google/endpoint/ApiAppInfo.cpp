#include <QFile>
#include <QJsonObject>
#include "ApiAppInfo.h"
#include "ApiUtil.h"

using namespace googleQt;

ApiAppInfo::ApiAppInfo():
    m_key(""),
    m_secret("")
{
    
};

void ApiAppInfo::setKeySecret(QString key_val, QString secret_val)
{
    m_key = key_val;
    m_secret = secret_val;
};

bool ApiAppInfo::readFromFile(QString path)
{
    QJsonObject js;
    if(!loadJsonFromFile(path, js))
        return false;
    m_key = js["key"].toString();
    m_secret = js["secret"].toString();
    return true;
};

bool ApiAppInfo::storeToFile(QString path)
{
    QJsonObject js;
    js["key"] = m_key;
    js["secret"] = m_secret;

    if(!storeJsonToFile(path, js))
        return false;
    return true;
};
