#pragma once

#include "ApiUtil.h"

namespace googleQt{
    
    class GoogleHost
    {
    public:
        
        static GoogleHost& DEFAULT();

        GoogleHost();

        const QString& getApi()const{return m_api;}
        const QString& getAuth()const{return m_auth;}

    protected:
        QString m_api;
        QString m_auth;
    };
}
