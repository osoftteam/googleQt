#pragma once

#include <QObject>

namespace googleQt{
    namespace demo{
        class ApiListener: public QObject
        {
            Q_OBJECT

        public slots:
            void transferProgress(qint64 bytesProcessed, qint64 total);
        };
    }//demo
};//googleQt
