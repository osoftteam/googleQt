#include <iostream>
#include "ApiListener.h"

using namespace googleQt;
using namespace demo;

void ApiListener::transferProgress(qint64 bytesProcessed, qint64 total)
{
    if(bytesProcessed != total && total != -1)
        std::cout << "progress " << bytesProcessed << " " << total << std::endl;
};
