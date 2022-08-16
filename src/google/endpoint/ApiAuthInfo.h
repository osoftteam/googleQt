#pragma once
#include <QString>

namespace googleQt{
    class ApiAuthInfo
    {
        friend class GoogleWebAuth;
    public:
        ApiAuthInfo();
        ApiAuthInfo(QString token_file, int scope = 0);
        virtual ~ApiAuthInfo(){};
        
        void purge();
        virtual bool reload();
        virtual bool updateToken(const QJsonObject& js_in);
        void setEmail(QString email){ m_email = email; }        
        
        QString getAccessToken()const {return m_accessToken;}
        QString getRefreshToken()const{return m_refreshToken;}
        QString getEmail()const{return m_email; }
        int     getExpirationInSeconds()const{return m_expires_in;}
        int     getScope()const { return m_token_scope; }

    protected:
        virtual bool readFromFile(QString path);
        virtual bool storeToFile(QString path)const;

    protected:
        QString m_token_file;
        QString m_accessToken;
        QString m_refreshToken;
        QString m_type;
        int     m_expires_in;
        QString m_expire_time;
        QString m_email;
        int     m_token_scope;
    };
}//dropboxQt
