/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification 
 Part of "Ardi - the organizer" project.
 osoft4ardi@gmail.com
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gcontact/GcontactRequestArg.h"
#include <QFile>
#include <QTextStream>
#include <QNetworkRequest>
#include "GoogleClient.h"
#include "gcontact/GcontactRoutes.h"

#ifdef API_QT_AUTOTEST
namespace googleQt{

    class GcontactAutotest {
    public:
        GcontactAutotest(GoogleClient&);

        void generateCalls();


    };//GcontactAutotest

}//googleQt
#endif //API_QT_AUTOTEST
