#include "ApiAutotest.h"

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
