#include "ApiAutotest.h"
#include "gmail/messages/MessagesMessagePayload.h"

using namespace googleQt;

bool ApiAutotest::init(QString filePathName){
    if(m_f.isOpen())m_f.close();
    m_f.setFileName(filePathName);
    if(!m_f.open(QIODevice::WriteOnly|QIODevice::Text|QIODevice::Truncate)){qWarning() << "failed to open" << filePathName; return false;}
    m_out.setDevice(&m_f);
    return true;
}

void ApiAutotest::close()
{
    if(m_f.isOpen())
        m_f.close();
};

#ifdef API_QT_AUTOTEST

static ApiAutotest* theInstance;

ApiAutotest& ApiAutotest::INSTANCE(){return *theInstance;};
ApiAutotest& ApiAutotest::operator << (const char * s){
    m_out << s << endl;
    return *this;
}

ApiAutotest& ApiAutotest::operator << (const QString & s){
    m_out << s << endl;
    return *this;
}
ApiAutotest& ApiAutotest::operator << (const QByteArray & arr){
    m_out << arr << endl;
    return *this;
}
ApiAutotest& ApiAutotest::operator << (const QNetworkRequest & r){
    m_out << "POST " << r.url().toString() << endl;
    QList<QByteArray> lst = r.rawHeaderList();
    for(QList<QByteArray>::iterator i = lst.begin(); i != lst.end(); i++){
        m_out << "--header " << *i << ": " << r.rawHeader(*i) << endl;
    }
    return *this;
}

void ApiAutotest::prepareAutoTestObj(const char* context_class_name, 
    const char* class_name, 
    void* p, 
    int idx,
    int context_index)
{
    if (strcmp(class_name, "messages::MessagePayloadHeader") == 0)
    {
        if (strcmp(context_class_name, "messages::MessagePayload") == 0)
        {
            messages::MessagePayloadHeader* h = (messages::MessagePayloadHeader*)p;
            switch (idx)
            {
            case 0: h->setName("From"); h->setValue("From_" + h->value()); break;
            case 1: h->setName("To"); h->setValue("To_" + h->value()); break;
            case 2: h->setName("Subject"); h->setValue("Subject_" + h->value()); break;
            }
        }//messages::MessagePayload
        if (strcmp(context_class_name, "messages::MessagePart") == 0) 
        {
            messages::MessagePayloadHeader* h = (messages::MessagePayloadHeader*)p;
            switch (context_index)
            {
            case 0:
                if (idx == 0)
                {
                    h->setName("Content-Type");
                    h->setValue("text/plain");
                }
                break;
            case 1: 
                if (idx == 0)
                {
                    h->setName("Content-Type");
                    h->setValue("text/html");
                }
                break;            
            }
        }
    }
};

void ApiAutotest::logRequest(QString req) 
{
    if (m_request_log_enabled)
    {
        *this << req;
    }
};

void ApiAutotest::addId(QString class_name, QString id) 
{
    CLASS_ID_MAP::iterator i = m_availID.find(class_name);
    if (i != m_availID.end())
    {
        i->second.insert(id);
    }
    else 
    {
        IDSET ids;
        ids.insert(id);
        m_availID[class_name] = ids;
    }
};

QString ApiAutotest::getId(QString class_name, int default_id_num) 
{
    QString rv;

    CLASS_ID_MAP::iterator i = m_availID.find(class_name);
    if (i != m_availID.end() && !i->second.empty())
    {
        IDSET& ids = i->second;
        IDSET::iterator j = ids.begin();
        rv = *j;
        ids.erase(j);
        //rv = ids
    }
    else 
    {
        rv = QString("id_%1").arg(default_id_num);
    }
    return rv;
};


#endif //API_QT_AUTOTEST

ApiAutotest::ApiAutotest()
{
    #ifdef API_QT_AUTOTEST
    theInstance = this;
    #endif //API_QT_AUTOTEST
}

ApiAutotest::~ApiAutotest()
{
#ifdef API_QT_AUTOTEST
    theInstance = NULL;
#endif //API_QT_AUTOTEST
};
