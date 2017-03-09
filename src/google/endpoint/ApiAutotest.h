#pragma once
#include "google/endpoint/ApiUtil.h"
#include <QFile>
#include <QTextStream>
#include <QNetworkRequest>

namespace googleQt{

    using IDSET = std::set<QString>;
    using CLASS_ID_MAP = std::map<QString, IDSET>;

    class ApiAutotest{
    public:
        ApiAutotest();
        virtual ~ApiAutotest();

        bool init(QString filePathName);
        void close();
        virtual void generateCalls() = 0;

        #ifdef API_QT_AUTOTEST
        static ApiAutotest& INSTANCE();
        ApiAutotest& operator << (const char * string);
        ApiAutotest& operator << (const QString & string);
        ApiAutotest& operator << (const QByteArray & array);
        ApiAutotest& operator << (const QNetworkRequest & req);

        void prepareAutoTestObj(const char* context_class_name, const char* class_name, void* p, int idx, int context_index);
		QByteArray generateData(const char* context_class_name, int context_index, int parent_context_index);
        void logRequest(QString req);
        void enableRequestLog(bool val) { m_request_log_enabled = val; };
        void addId(QString class_name, QString id);
        QString getId(QString class_name, int default_id_num);

    protected:
        CLASS_ID_MAP m_availID;
        #endif //API_QT_AUTOTEST        

    protected:
        QFile m_f;
        QTextStream m_out;
        bool  m_request_log_enabled{true};
    };//ApiAutotest

}//googleQt
