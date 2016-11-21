/**********************************************************
 DO NOT EDIT
 This file was generated from stone specification 
 www.prokarpaty.net
***********************************************************/

#pragma once
#include "google/endpoint/ApiUtil.h"
#include "gtask/GtaskRequestArg.h"
#include <QFile>
#include <QTextStream>
#include <QNetworkRequest>
#include "GoogleClient.h"
#include "gtask/GtaskRoutes.h"

#ifdef API_QT_AUTOTEST
namespace googleQt{

    class GtaskAutotest {
    public:
        GtaskAutotest(GoogleClient&);

        void generateCalls();


    };//GtaskAutotest

}//googleQt
#endif //API_QT_AUTOTEST
