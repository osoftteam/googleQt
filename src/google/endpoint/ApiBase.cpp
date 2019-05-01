#include <QFile>
#include <QJsonParseError>
#include <QNetworkInterface>
#include <functional>
#include "ApiUtil.h"

using namespace googleQt;

bool googleQt::loadJsonFromFile(QString path, QJsonObject& js)
{
    QFile jf(path);
    if (!jf.open(QFile::ReadOnly)) {
        return false;
    }
    QJsonParseError  err;
    QJsonDocument jd = QJsonDocument().fromJson(jf.readAll(), &err);
    if (err.error == QJsonParseError::NoError) {
        js = jd.object();
        return true;
    }
    return false;
};

bool googleQt::storeJsonToFile(QString path, const QJsonObject js)
{
    QFile jf(path);
    if (!jf.open(QFile::WriteOnly)) {
        return false;
    }

    QJsonDocument jd(js);
    jf.write(jd.toJson());
    jf.close();
    return true;
};

bool googleQt::isConnectedToNetwork()
{
    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    for (int i = 0; i < ifaces.count(); i++)
    {
        QNetworkInterface iface = ifaces.at(i);
        if (iface.flags().testFlag(QNetworkInterface::IsUp)
            && !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            if (iface.addressEntries().count() > 0)
            {
                return true;
            }
        }
    }
    return false;
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
    if (!value.isEmpty())
        m_q.addQueryItem(name, value);
    return *this;
};

UrlBuilder& UrlBuilder::add(QString name, const char* value)
{
    if (value != nullptr) {
        return add(name, QString(value));
    }
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
    if (value.isValid())
        m_q.addQueryItem(name, value.toString("yyyy-MM-ddThh:mm:ssZ"));
    return *this;
};

UrlBuilder& UrlBuilder::add(QString name, uint64_t value) 
{
    m_q.addQueryItem(name, QString("%1").arg(value));
    return *this;
};

void QParamArg::ResponseFields2Builder(UrlBuilder& b)const
{
    if (!m_Fields.isEmpty()) {
        b.add("fields", m_Fields);
    }
};

QString googleQt::slist2commalist(const STRING_LIST& lst)
{
    QString rv = "";
    if (!lst.empty()) {
        for (STRING_LIST::const_iterator i = lst.begin();
            i != lst.end();
            i++)
        {
            rv += *i;
            rv += ",";
        }
        rv = rv.left(rv.length() - 1);
    }
    return rv;
};

QString googleQt::slist2commalist_decorated(const STRING_LIST& lst, char deco)
{
    QString rv = "";
    if (!lst.empty())
    {
        for (STRING_LIST::const_iterator i = lst.begin();
            i != lst.end();
            i++)
        {
            rv += QString("%1%2%3").arg(deco).arg(*i).arg(deco);
            rv += ",";
        }
        rv = rv.left(rv.length() - 1);
    }
    return rv;
};


STRING_LIST googleQt::split_string(QString s)
{
    QStringList s_list = s.split(" ", QString::SkipEmptyParts);
    STRING_LIST rv;
    for (QStringList::iterator i = s_list.begin(); i != s_list.end(); i++)
    {
        rv.push_back(*i);
    }
    return rv;
};

QString googleQt::size_human(qreal num)
{
    QStringList list;
    list << "KB" << "MB" << "GB" << "TB";

    QStringListIterator i(list);
    QString unit("bytes");

    while (num >= 1024.0 && i.hasNext())
    {
        unit = i.next();
        num /= 1024.0;
    }
    return QString().setNum(num, 'f', 2) + " " + unit;
}

QString googleQt::trim_alpha_label(QString lbl)
{
    QString rv = lbl.trimmed().toLower().remove(QRegExp("[^a-zA-Z\\d\\s]")).remove(QRegExp("\\s"));
    return rv;
};

/**
xml_util
*/
QDomElement googleQt::xml_util::addNode(QDomDocument& doc, QDomNode& parent_node, QString name)
{
    QDomElement t = doc.createElement(QString(name));
    parent_node.appendChild(t);
    return t;
};

QDomElement googleQt::xml_util::ensureNode(QDomDocument& doc, QDomNode& parent_node, QString name)
{
    QDomElement t = parent_node.firstChildElement(name);
    if (t.isNull()) {
        t = doc.createElement(QString(name));
        parent_node.appendChild(t);
    }
    return t;
};

void googleQt::xml_util::removeNodes(QDomNode& parent_node, QString name)
{
    QDomElement t = parent_node.firstChildElement(name);
    while (!t.isNull()) {
        parent_node.removeChild(t);
        t = parent_node.firstChildElement(name);
    }
};

void googleQt::xml_util::updateNode(QDomDocument& doc, QDomNode& parent_node, QString child_name, QString child_value)
{
    auto t = ensureNode(doc, parent_node, child_name);
    auto c = t.firstChild();
    if (!c.isNull()) {
        c.setNodeValue(child_value);
    }
    else {
        QDomText tn = doc.createTextNode(QString(child_value));
        t.appendChild(tn);
    }
};

QDomText googleQt::xml_util::addText(QDomDocument& doc, QDomElement parent_elem, QString text)
{
    QDomText tn = doc.createTextNode(QString(text));
    parent_elem.appendChild(tn);
    return tn;
}