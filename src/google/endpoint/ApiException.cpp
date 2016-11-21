#include "ApiException.h"

namespace googleQt{
void ReplyException::build(std::string err)
{
    m_what = m_msg;
    m_what += "\n";
    if(!m_error_summary.empty())m_what += "SUMMARY:" + m_error_summary;
    m_what += "\n";
    if(!err.empty())
        {
            m_what += "ERROR:\n";
            m_what += err;
        }
}

}//dropboxQt
