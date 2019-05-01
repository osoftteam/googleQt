#include <QCoreApplication>
#include "ApiAutotest.h"
#include "gmail/messages/MessagesMessagePayload.h"
#include "ApiClient.h"

using namespace googleQt;

bool ApiAutotest::init(QString filePathName){
    close();
    m_out = fopen(filePathName.toStdString().c_str(), "wb");
    if (m_out) {
        setvbuf(m_out, NULL, _IONBF, 0);
    }
    return (m_out != nullptr);
}

void ApiAutotest::close()
{
    if (m_out) {
        fclose(m_out);
        m_out = nullptr;
    }
};

#ifdef API_QT_AUTOTEST

static ApiAutotest* theInstance;

void ApiAutotest::log_string(const char* s)
{
    if (m_out) {
        fwrite(s, sizeof(char), strlen(s), m_out);
        static const char* end_of_line = "\n";
        fwrite(end_of_line, sizeof(char), strlen(end_of_line), m_out);
    }
};

ApiAutotest& ApiAutotest::INSTANCE(){return *theInstance;};
ApiAutotest& ApiAutotest::operator << (const char * s){
    log_string(s);
    return *this;
}

ApiAutotest& ApiAutotest::operator << (const QString & s){
    std::string s2 = s.toStdString();
    log_string(s2.c_str());
    return *this;
}
ApiAutotest& ApiAutotest::operator << (const QByteArray & arr){
    log_string(arr.constData());
    return *this;
}

ApiAutotest& ApiAutotest::operator << (const int & val) 
{
    QString s = QString("%1").arg(val);
    std::string s2 = s.toStdString();
    log_string(s2.c_str());
    return *this;
};

ApiAutotest& ApiAutotest::operator << (const QNetworkRequest & r){
    QString s = QString("POST %1").arg(r.url().toString());
    log_string(s.toStdString().c_str());

    QList<QByteArray> lst = r.rawHeaderList();
    for (QList<QByteArray>::iterator i = lst.begin(); i != lst.end(); i++) {
        s = QString("--header %1: %2").arg(i->constData()).arg(r.rawHeader(*i).constData());
        log_string(s.toStdString().c_str());
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
                        //case 0: h->setName("From"); h->setValue("From_" + h->value() + "@gmail.com"); break;
                        case 0: h->setName("From"); h->setValue(ApiAutotest::INSTANCE().userId()); break;
                        case 1: h->setName("To"); h->setValue("To_" + h->value() + "@gmail.com"); break;
                        case 2: h->setName("Subject"); h->setValue("Subject_" + h->value() + "@gmail.com"); break;
                        case 3: h->setName("CC"); h->setValue("CC_" + h->value() + "@gmail.com"); break;
                        case 4: h->setName("BCC"); h->setValue("BCC_" + h->value() + "@gmail.com"); break;
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

QByteArray ApiAutotest::generateData(const char* context_class_name, int context_index, int parent_context_index)
{
    Q_UNUSED(context_index);

    QByteArray rv = QByteArray("AUTOTEST-DATA").toBase64(QByteArray::Base64UrlEncoding);
    if (strcmp(context_class_name, "messages::MessagePartBody") == 0)
        {
            static int ref_num = 0;
            ref_num++;

            QString sample_header = QString("<p><strong>ref# %1</strong></p>\
            <p>Mr.M. %2 <br / >Chief of Syrup Production<br / >Old Sticky Pancake Company<br / >\
            456 Maple Lane<br / >Forest, ON 7W8 9Y0<br / ><br / >Dear Mr. %2 :<br / ><br / >")
                .arg(ref_num)
                .arg(userId());

            QString sample_body = "Let me begin by thanking you for your past contributions to our Little League baseball team.\
            Your sponsorship aided in the purchase of ten full uniforms and several pieces of baseball equipment \
            for last year's season.<br /><br />Next month, our company is planning an employee appreciation pancake \
            breakfast honoring retired employees for their past years of service and present employees for their \
            loyalty and dedication in spite of the current difficult economic conditions.<br /><br />\
            We would like to place an order with your company for 25 pounds of pancake mix and five gallons \
            of maple syrup. We hope you will be able to provide these products in the bulk quantities \
            we require.<br /><br />As you are a committed corporate sponsor and long-time associate, \
            we hope that you will be able to join us for breakfast on December 12, 2016.<br /><br />\
            Respectfully yours,<br /><br />&nbsp;<br /><br />Derek Jeter<br />\
            https://www.scribendi.com/advice/formal_letter_example.en.html</p>";

            QString sample_html = sample_header + sample_body;

            QString s;
            if (parent_context_index == 0)
                {
                    QString tmp = sample_html;
                    tmp.remove(QRegExp("<[^>]*>"));
                    s = tmp;
                }
            else if (parent_context_index == 1)
                {
                    s = sample_html;
                }

            rv = QByteArray(s.toStdString().c_str()).toBase64(QByteArray::Base64UrlEncoding);
        }else if (strcmp(context_class_name, "attachments::MessageAttachment") == 0){
            if(m_attachmentDataGenerationOn){
                rv = QByteArray("Attachment from AUTOTEST-DATA").toBase64(QByteArray::Base64UrlEncoding);                
            }
            else{
                rv = QByteArray();
            }
        }

    return rv;
};

void ApiAutotest::logRequest(QString req) 
{
    if (m_request_log_enabled)
        {
            *this << req;
        }
};

void ApiAutotest::addId(const char* class_name, QString id)
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

void ApiAutotest::addMemberValue(const char* class_name, const char* field_name, QString val)
{
    auto i = m_availMemberValue.find(class_name);
    if (i != m_availMemberValue.end())
    {
        //i->second[field_name].push_back(std::pair<QString, int>(val, 0));
        i->second[field_name].insert(std::make_pair(val, 0));
    }
    else 
    {
        MEMBER_VAL m;
        auto& j = m[field_name].find(val);
        if (j != m[field_name].end()) {
            j->second = j->second - 1;
        }
        else {
            m[field_name].insert(std::make_pair(val, 0));
        }
        m_availMemberValue[class_name] = m;
    }
};

void ApiAutotest::addBatchId(const char* class_name, std::pair<QString, googleQt::EBatchId> bid) 
{
    auto i = m_availBatchID.find(class_name);
    if (i != m_availBatchID.end())
    {
        i->second.push_back(bid);
    }
    else
    {
        BATCH_LIST lst;
        lst.push_back(bid);
        m_availBatchID[class_name] = lst;
    }
};

void ApiAutotest::addIdSet(const char* class_name, const IDSET& id_set)
{
    for (auto c : id_set) {       
        addId(class_name, c);
    }
};

void ApiAutotest::addBatchIdList(const char* class_name, const BATCH_LIST& bid_list)
{
    for (auto c : bid_list) {
        addBatchId(class_name, c);
    }
};

IDSET ApiAutotest::getReservedIdSet(const char* class_name)
{
    IDSET rv;
    CLASS_ID_MAP::iterator i = m_availID.find(class_name);
    if (i != m_availID.end() && !i->second.empty())
    {
        rv = i->second;
        i->second.clear();
    }
    return rv;
};

BATCH_LIST ApiAutotest::getReservedBatchList(const char* class_name) 
{
    BATCH_LIST rv;
    auto i = m_availBatchID.find(class_name);
    if (i != m_availBatchID.end() && !i->second.empty())
    {
        rv = i->second;
        i->second.clear();
    }
    return rv;

};

QString ApiAutotest::getId(const char* class_name, int default_id_num)
{
    QString rv;

    CLASS_ID_MAP::iterator i = m_availID.find(class_name);
    if (i != m_availID.end() && !i->second.empty())
        {
            IDSET& ids = i->second;
            IDSET::iterator j = ids.begin();
            rv = *j;
            ids.erase(j);
        }
    else 
        {
            if (strcmp(class_name, "messages::MessageResource") == 0){
                    rv = QString("mid_%1_%2_%3")
                        .arg(default_id_num)
                        .arg(QDateTime::currentMSecsSinceEpoch())
                        .arg(qrand());
                }
            else if(strcmp(class_name, "threads::ThreadResource") == 0){
                rv = QString("thread_id_%1_%2_%3")
                    .arg(default_id_num)
                    .arg(QDateTime::currentMSecsSinceEpoch())
                    .arg(qrand());
            }
            else if (strcmp(class_name, "labels::LabelResource") == 0) {
                rv = QString("label_id_%1_%2_%3")
                    .arg(default_id_num)
                    .arg(QDateTime::currentMSecsSinceEpoch())
                    .arg(qrand());
            }
            else
                {
                    rv = QString("id_%1").arg(default_id_num);
                }
        }
    return rv;
};

quint64 ApiAutotest::getInt(const char* class_name, const char* field_name, int default_id_num)
{
    quint64 rv = default_id_num;
    if (strcmp(class_name, "messages::MessageResource") == 0)
        {
            if (strcmp(field_name, "m_internalDate") == 0) 
                {
                    rv = QDateTime::currentMSecsSinceEpoch() + default_id_num * 1000;
                }

        }
    return rv;
};

QString ApiAutotest::getString(const char* class_name, const char* field_name, QString default_value)
{
    QString rv = default_value;
    if (strcmp(class_name, "messages::MessagePayload") == 0)
        {
            if (strcmp(field_name, "m_filename") == 0) 
                {
                    rv += ".txt";
                }
        }
    else if (strcmp(class_name, "messages::MessagePart") == 0)
    {
        if (strcmp(field_name, "m_filename") == 0)
        {
            rv += ".txt";
        }
    }
    else if (strcmp(class_name, "attachments::MessageAttachment") == 0) 
    {
        if (strcmp(field_name, "m_attachmentId") == 0)
        {
            rv = QString("att_%1_%2_%3")
                .arg(default_value)
                .arg(QDateTime::currentMSecsSinceEpoch())
                .arg(qrand());
        }
    }//ykh - todo could be generic
    else if (strcmp(class_name, "messages::MessageResource") == 0 ||
            strcmp(class_name, "labels::LabelResource") == 0)
    {
        if (strcmp(field_name, "m_threadId") == 0 ||
            strcmp(field_name, "m_name") == 0)
        {
            auto i = m_availMemberValue.find(class_name);
            if (i != m_availMemberValue.end()) {
                auto j = i->second.find(field_name);
                if (j != i->second.end()) {
                    if (!j->second.empty()) {
                        IDVALMAP& v2usage = j->second;
                        if (!v2usage.empty()) {
                            IDVALMAP::iterator min_it = v2usage.begin();
                            for (IDVALMAP::iterator k = v2usage.begin(); k != v2usage.end(); k++) {
                                if (k->second < min_it->second) {
                                    min_it = k;
                                }
                            }

                            if (min_it != v2usage.end()) {
                                min_it->second += 1;
                                rv = min_it->first;
                            }
                        }
                    }
                }
            }
        }
    }

    return rv;
};


void ApiAutotest::setString4List(const char* class_name, const char* field_name, QString value) 
{
    QString key = class_name;
    key += field_name;
    m_str4list[key] = value;
};

void ApiAutotest::clearString4List(const char* class_name, const char* field_name) 
{
    QString key = class_name;
    key += field_name;
    m_str4list.erase(key);
};


QString ApiAutotest::getString4List(const char* class_name, const char* field_name)
{
    QString rv = "";
    if (strcmp(class_name, "messages::MessageResource") == 0){
        if (strcmp(field_name, "m_labelIds") == 0)
        {
            QString key = class_name;
            key += field_name;

            auto it = m_str4list.find(key);
            if (it != m_str4list.end()) {
                rv = it->second;
            }
    }      
    }
    return rv;
}

void ApiAutotest::cancellAll()const
{
    m_cancelRequest = true;
};

void ApiAutotest::emulateAutotestDownloadProgress(googleQt::ApiClient* cl)
{
    if(!m_cancelRequest){
        qApp->processEvents();
        if (isProgressEmulationEnabled()) {
            static int max_progress     = 100;
            static int step_progress    = 20;
            for (int progress = 0; progress < max_progress; progress += step_progress) {
                cl->downloadProgress(progress, max_progress);
                sleep(10);
                if(m_cancelRequest)
                    break;
            }
        }
    }
};

void ApiAutotest::sleep(int millisecondsToWait)
{
    if(!m_cancelRequest){
        QTime endTime = QTime::currentTime().addMSecs(millisecondsToWait);
        while (QTime::currentTime() < endTime)
            {
                QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
                if(m_cancelRequest)
                    break;
            }
    }
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
