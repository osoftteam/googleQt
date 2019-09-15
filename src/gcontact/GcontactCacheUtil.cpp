#include <QXmlInputSource>
#include "GcontactCacheUtil.h"

using namespace googleQt;
using namespace gcontact;

/**
   DbPersistant
*/
DbPersistant::DbPersistant()
{
    m_flags.flags = 0;
    m_flags.register_mods = 1;
};

void DbPersistant::setDirty(bool val)
{
    if(m_flags.register_mods){
        m_flags.is_dirty = val ? 1 : 0;
    }
}

void DbPersistant::setRegisterModifications(bool val)
{
    m_flags.register_mods = val ? 1 : 0;
};

/**
   ContactXmlPersistant
*/
ContactXmlPersistant::ContactXmlPersistant() :m_status(localCopy)
{

};

void ContactXmlPersistant::markAsNormalCopy()
{
    if(registerMods()){
        m_status = localCopy;
        setDirty(true);
    }
};

void ContactXmlPersistant::markAsModified()
{
    if(registerMods()){
        m_status = localModified;
        setDirty(true);
    }
};

void ContactXmlPersistant::markAsDeleted()
{
    if(registerMods()){
        m_status = localRemoved;
        setDirty(true);
    }
};

void ContactXmlPersistant::markAsRetired()
{
    if(registerMods()){
        m_status = localRetired;
        setDirty(true);
    }
};

void ContactXmlPersistant::markAsIdLimbo()
{
    if (registerMods()) {
        m_status = localIdLimbo;
        setDirty(true);
    }
};

bool ContactXmlPersistant::isRepoSyncCompleted()const 
{ 
	return m_repo_sync_completed; 
}

void ContactXmlPersistant::setRepoSyncCompleted(bool val) 
{ 
	m_repo_sync_completed = val;
	if (m_repo_sync_completed) {
		markAsNormalCopy();
	}
}

void ContactXmlPersistant::printXmlParseError(QString contextMsg,
                                                       const QByteArray & data,
                                                       QString errorMsg,
                                                       int errorLine,
                                                       int errorColumn)
{
    qWarning() << contextMsg << errorMsg << "line=" << errorLine << "column=" << errorColumn;
    qWarning() << "-- begin data " << data.size() << "bytes";
    qWarning() << data;
    qWarning() << "-- end data";
    QString html_text = data;
    int line_idx = 1;
    QStringList lines = html_text.split("\n");
    for(auto & s : lines){
        QString sidx = QString("%1.").arg(line_idx);
        qWarning() << sidx.leftJustified(3, ' ') << s;
        line_idx++;
    }        
    qWarning() << "-----------";
    class MyXmlErrorHandler : public QXmlErrorHandler 
    {
    public:
        bool error(const QXmlParseException & ex) override
        {
            reportError("error", ex);            
            return false;
        };
        QString errorString() const override{ return ""; }
        bool fatalError(const QXmlParseException & ex) override 
        { 
            reportError("fatal-error", ex);
            return false; 
        }
        bool warning(const QXmlParseException & ex) override 
        { 
            reportError("warning", ex);
            return true; 
        }
        void reportError(QString context, const QXmlParseException & ex) 
        {
            qWarning() << context << ex.message() 
                << "line=" << ex.lineNumber() 
                << "column=" << ex.columnNumber() 
                << "publicId=" << ex.publicId()
                << "systemId=" << ex.systemId();
        }
    } err_handler;

    QXmlInputSource xml_source;
    xml_source.setData(data);
    QXmlSimpleReader r;
    r.setErrorHandler(&err_handler);
    r.parse(&xml_source);
    qWarning() << "-----------";
}

bool ContactXmlPersistant::parseXml(const QByteArray & data)
{
    m_is_null = true;

    QDomDocument doc;
    QString errorMsg = 0;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        printXmlParseError("ContactXmlPersistant - Failed to parse contacts XML document",
                           data,
                           errorMsg,
                           errorLine,
                           errorColumn);
        
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

bool ContactXmlPersistant::verifyXml(const QString & xml) 
{
    QByteArray data(xml.toStdString().c_str());

    QDomDocument doc;
    QString errorMsg = 0;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(data, &errorMsg, &errorLine, &errorColumn)) {
        printXmlParseError("ContactXmlPersistant - Failed to parse contacts XML document",
            data,
            errorMsg,
            errorLine,
            errorColumn);

        return false;
    }

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
        printXmlParseError("Failed to parse original contacts XML document during Xml-merge",
                           data,
                           errorMsg,
                           errorLine,
                           errorColumn);        
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

QString ContactXmlPersistant::status2string(EStatus status)
{
#define CASE_STATUS(S) case S: rv = #S;break;

    QString rv = "status-error";
    switch(status){
        CASE_STATUS(localCopy);
        CASE_STATUS(localModified);
        CASE_STATUS(localRemoved);
        CASE_STATUS(localRetired);
        CASE_STATUS(localIdLimbo);
    }
    return rv;
};

void ContactXmlPersistant::assignContent(const ContactXmlPersistant& src) 
{
    m_etag = src.m_etag;
    m_id = src.m_id;
    m_title = src.m_title;        
    m_content = src.m_content;
    m_updated = src.m_updated;
    m_parsed_xml = src.m_parsed_xml;
    m_status = src.m_status;
    markAsModified();
};

/**
    BatchRequest
*/
QString BatchRequest::batch2xml(googleQt::EBatchId bid)
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


QString BatchRequest::toBatchXmlEntryBegin()const 
{
    QString rv;
    switch (m_batch_id) {
    case googleQt::EBatchId::update:
    case googleQt::EBatchId::delete_operation: {
        rv = QString("<entry gd:etag=\'*\'>\n");
    }break;
    default: {
            rv = QString("<entry>\n");
    }
    }

    rv += batch2xml(m_batch_id);
    rv += "\n";
    return rv;
};

/**
BatchResult
*/
bool BatchResult::parseBatchResult(QDomNode n)
{
    m_is_null = true;
    m_id = EBatchId::none;
    m_operation_type = "";
    m_status_code = 0;
    m_status_reason = "";

    QDomElement elem_bid = n.firstChildElement("batch:id");
    if (!elem_bid.isNull()) {

        QString s_bid = elem_bid.text().trimmed();
        if (s_bid.compare("retrieve", Qt::CaseInsensitive) == 0) {
            m_id = EBatchId::retrieve;
        }
        else if (s_bid.compare("create", Qt::CaseInsensitive) == 0) {
            m_id = EBatchId::create;
        }
        else if (s_bid.compare("update", Qt::CaseInsensitive) == 0) {
            m_id = EBatchId::update;
        }
        else if (s_bid.compare("delete", Qt::CaseInsensitive) == 0) {
            m_id = EBatchId::delete_operation;
        }

        if (m_id != EBatchId::none) {
            m_is_null = false;

            QDomElement elem_op = n.firstChildElement("batch:operation");
            if (!elem_op.isNull()) {
                QDomNamedNodeMap attr_names = elem_op.attributes();
                if (attr_names.size() > 0) {
                    for (int j = 0; j < attr_names.size(); j++) {
                        QDomNode n2 = attr_names.item(j);
                        if (n2.nodeType() == QDomNode::AttributeNode) {
                            if (n2.nodeName().compare("type") == 0) {
                                m_operation_type = n2.nodeValue();
                            }
                        }
                    }
                }
            }//batch:operation

            QDomElement elem_status = n.firstChildElement("batch:status");
            if (!elem_status.isNull()) {
                QDomNamedNodeMap attr_names = elem_status.attributes();
                if (attr_names.size() > 0) {
                    for (int j = 0; j < attr_names.size(); j++) {
                        QDomNode n2 = attr_names.item(j);
                        if (n2.nodeType() == QDomNode::AttributeNode) {
                            if (n2.nodeName().compare("code") == 0) {
                                m_status_code = n2.nodeValue().toInt();
                            }
                            else if (n2.nodeName().compare("reason") == 0) {
                                m_status_reason = n2.nodeValue();
                            }
                        }
                    }
                }
            }//batch:status
        }
    }

    qDebug() << "parseBatchResult" << m_operation_type << m_status_code << m_status_reason << (m_is_null ? "ERR" : "OK");
    
    return !m_is_null;
};


bool BatchResult::succeded()const
{
    bool rv = false;
    if (!m_is_null) {
        rv = (m_status_code >= 200 && m_status_code < 400);
    }
    return rv;
};


#ifdef API_QT_AUTOTEST
QString BatchResult::EXAMPLE(googleQt::EBatchId bid) 
{
    QString rv = BatchRequest::batch2xml(bid) + "\n";
    switch (bid)
    {
    case googleQt::EBatchId::none:break;
    case googleQt::EBatchId::retrieve:
    case googleQt::EBatchId::update:
    case googleQt::EBatchId::delete_operation:
    {
        rv += "<batch:status code='200' reason='Success'/>";
    }break;
    case googleQt::EBatchId::create:
    {
        rv += "<batch:status code='201' reason='Created'/>";
    }break;    
    }

    return rv;
};
#endif
