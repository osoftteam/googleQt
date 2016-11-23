#include "ApiTerminal.h"

using namespace googleQt;
using namespace demo;

std::ostream& operator << (std::ostream& os, const QString& s)
{
    os << s.toStdString();
    return os;
}


