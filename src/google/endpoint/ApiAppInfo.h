#pragma once

#include <QString>

namespace googleQt{
    class ApiAppInfo
    {
    public:
        ApiAppInfo();

        const QString& getKey()const{return m_key;}
        const QString& getSecret()const{return m_secret;}

        void setKeySecret(QString key_val, QString secret_val);
        bool readFromFile(QString path);
        bool storeToFile(QString path);

    protected:
        QString m_key;
        QString m_secret;
    };

}
