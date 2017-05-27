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
                        case 0: h->setName("From"); h->setValue("From_" + h->value() + "@gmail.com"); break;
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

            static const char* sample_html = "<p><strong>ref# %1</strong></p>\
            <p>Mr.M.Leaf<br / >Chief of Syrup Production<br / >Old Sticky Pancake Company<br / >\
            456 Maple Lane<br / >Forest, ON 7W8 9Y0<br / ><br / >Dear Mr.Leaf:<br / ><br / >\
            Let me begin by thanking you for your past contributions to our Little League baseball team.\
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

            QString s;
            if (parent_context_index == 0)
                {
                    QString tmp = sample_html;
                    tmp.remove(QRegExp("<[^>]*>"));
                    s = QString(tmp).arg(ref_num);
                }
            else if (parent_context_index == 1)
                {
                    s = QString(sample_html).arg(ref_num);
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
            if (strcmp(class_name, "messages::MessageResource") == 0)
                {
                    rv = QString("mid_%1_%2_%3")
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

    return rv;
};

void ApiAutotest::emulateAutotestDownloadProgress(googleQt::ApiClient* cl)
{
    qApp->processEvents();
    if (isProgressEmulationEnabled()) {
        static int max_progress     = 100;
        static int step_progress    = 20;
        for (int progress = 0; progress < max_progress; progress += step_progress) {
            cl->downloadProgress(progress, max_progress);
            sleep(100);
        }
    }
};

void ApiAutotest::sleep(int millisecondsToWait)
{
    QTime endTime = QTime::currentTime().addMSecs(millisecondsToWait);
    while (QTime::currentTime() < endTime)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
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
