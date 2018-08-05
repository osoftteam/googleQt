/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification 
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gdrive/GdriveRequestArg.h"
#include <QFile>
#include <QTextStream>
#include <QNetworkRequest>
#include "GoogleClient.h"
#include "gdrive/GdriveRoutes.h"

#ifdef API_QT_AUTOTEST
namespace googleQt{

    class GdriveAutotest {
    public:
        GdriveAutotest(GoogleClient&);

        void generateCalls();


    };//GdriveAutotest

}//googleQt
#endif //API_QT_AUTOTEST
