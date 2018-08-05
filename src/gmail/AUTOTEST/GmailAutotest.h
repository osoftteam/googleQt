/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification 
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gmail/GmailRequestArg.h"
#include <QFile>
#include <QTextStream>
#include <QNetworkRequest>
#include "GoogleClient.h"
#include "gmail/GmailRoutes.h"

#ifdef API_QT_AUTOTEST
namespace googleQt{

    class GmailAutotest {
    public:
        GmailAutotest(GoogleClient&);

        void generateCalls();


    };//GmailAutotest

}//googleQt
#endif //API_QT_AUTOTEST
