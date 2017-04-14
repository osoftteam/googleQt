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

	QByteArray rv = QByteArray("AUTOTEST-DATA").toBase64();
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
