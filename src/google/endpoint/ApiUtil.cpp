#include <QFile>
#include <QJsonParseError>
#include <functional>
#include "ApiUtil.h"

using namespace googleQt;

bool googleQt::loadJsonFromFile(QString path, QJsonObject& js)
{
    QFile jf(path);
    if(!jf.open(QFile::ReadOnly)){
        return false;
    }
    QJsonParseError  err;
    QJsonDocument jd = QJsonDocument().fromJson(jf.readAll(), &err);
    if(err.error == QJsonParseError::NoError){
        js = jd.object();
        return true;
    }
    return false;
};

bool googleQt::storeJsonToFile(QString path, const QJsonObject js)
{
    QFile jf(path);
    if(!jf.open(QFile::WriteOnly)){
        return false;
    }

    QJsonDocument jd(js);
    jf.write(jd.toJson());
    jf.close();
    return true;
};


VoidType& VoidType::instance()
{
    static VoidType void_type;
    return void_type;
};

VoidType::operator QJsonObject ()const
{
    static QJsonObject js;
    return js;
};

/**
    UrlBuilder
*/
UrlBuilder::UrlBuilder(const QString& link_path, QUrl& url) :m_url(url)
{
    url.setUrl(link_path);
};

UrlBuilder::~UrlBuilder() 
{
    m_url.setQuery(m_q);
};

UrlBuilder& UrlBuilder::add(QString name, QString value) 
{
    if(!value.isEmpty())
        m_q.addQueryItem(name, value);
    return *this;
};

UrlBuilder& UrlBuilder::add(QString name, bool value) 
{
    m_q.addQueryItem(name, value ? "true" : "false");
    return *this;
};

UrlBuilder& UrlBuilder::add(QString name, int value) 
{
    m_q.addQueryItem(name, QString("%1").arg(value));
    return *this;
};

UrlBuilder& UrlBuilder::add(QString name, const QDateTime& value) 
{
    if(value.isValid())
        m_q.addQueryItem(name, value.toString("yyyy-MM-ddThh:mm:ssZ"));
    return *this;
};

void QParamArg::ResponseFields2Builder(UrlBuilder& b)const
{
    if(!m_partResponseFields.empty()){
        QString fields = "";
        for(QString f : m_partResponseFields){
            fields += f;
            fields += ",";
        }
        fields = fields.left(fields.length() - 1);
        b.add("fields", fields);
    }
};
