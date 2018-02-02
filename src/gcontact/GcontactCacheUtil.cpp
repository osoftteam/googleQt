#include "GcontactCacheUtil.h"

using namespace googleQt;
using namespace gcontact;

/**
ContactXmlPersistant
*/
ContactXmlPersistant::ContactXmlPersistant() :m_status(localCopy)
{

};

bool ContactXmlPersistant::parseXml(const QByteArray & data)
{
    m_is_null = true;

    QDomDocument doc;
    QString errorMsg = 0;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qWarning() << "Failed to parse contacts XML document: " << errorMsg << "line=" << errorLine << "column=" << errorColumn;
        qWarning() << "-- begin data " << data.size();
        qWarning() << data;
        qWarning() << "-- end data";
        return false;
    }

    QDomNodeList entries = doc.elementsByTagName("entry");
    if (entries.size() > 0) {
        QDomNode n = entries.item(0);
        if (!parseEntryNode(n)) {
            return false;
        }
    }

    m_is_null = false;

    return true;
};

bool ContactXmlPersistant::parseXml(const QString & xml)
{
    QByteArray d(xml.toStdString().c_str());
    return parseXml(d);
};

QString ContactXmlPersistant::mergedXml(QString mergeOrigin)const
{
    if (mergeOrigin.isEmpty()) {
        qWarning() << "incomming XML string is empty";
        return "";
    }

    /// try to merge data into original xml
    QByteArray data(mergeOrigin.toStdString().c_str());
    QDomDocument doc;
    QString errorMsg = 0;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        qWarning() << "Failed to parse original contacts XML document: " << errorMsg << "line=" << errorLine << "column=" << errorColumn;
        qWarning() << "-- begin data " << data.size();
        qWarning() << data;
        qWarning() << "-- end data";
        return "";
    }

    QDomNodeList entries = doc.elementsByTagName("entry");
    if (entries.size() == 0) {
        qWarning() << "Invalid original xml contact data (1)";
        return "";
    }

    QDomNode entry_node = entries.item(0);
    if (entry_node.isNull()) {
        qWarning() << "Invalid original xml contact data (2)";
        return "";
    }

    mergeEntryNode(doc, entry_node);

    QString rv = "";
    QTextStream ss(&rv);
    entry_node.save(ss, 4);
    ss.flush();
    return rv;
}

ContactXmlPersistant::EStatus ContactXmlPersistant::validatedStatus(int val, bool* ok /*= nullptr*/)
{
    EStatus rv = localCopy;
    if (rv < ContactXmlPersistant::localCopy || rv > ContactXmlPersistant::localRetired) {
        rv = ContactXmlPersistant::localCopy;
        if (ok) {
            *ok = false;
        }
    }
    else {
        if (ok) {
            *ok = true;
        }
        rv = (ContactXmlPersistant::EStatus)val;
    }
    return rv;
};

QString ContactXmlPersistant::batch2xml(googleQt::EBatchId bid)
{
    static QString xml_template = "<batch:id>%1</batch:id>\n<batch:operation type='%2'/>";

    QString rv;
    switch (bid)
    {
    case googleQt::EBatchId::none:break;
    case googleQt::EBatchId::retrieve:
    {
        rv = QString(xml_template).arg("retrieve").arg("query");
    }break;
    case googleQt::EBatchId::create:
    {
        rv = QString(xml_template).arg("create").arg("insert");
    }break;
    case googleQt::EBatchId::update:
    {
        rv = QString(xml_template).arg("update").arg("update");
    }break;
    case googleQt::EBatchId::delete_operation:
    {
        rv = QString(xml_template).arg("delete").arg("delete");
    }break;
    }
    return rv;
}


void ContactXmlPersistant::assignContent(const ContactXmlPersistant& src) 
{
    m_etag = src.m_etag;
    m_id = src.m_id;
    m_title = src.m_title;        
    m_content = src.m_content;
    m_updated = src.m_updated;
    m_original_xml_string = src.m_original_xml_string;
    m_status = src.m_status;
};

