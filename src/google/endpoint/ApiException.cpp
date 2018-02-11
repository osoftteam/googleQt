#include "ApiException.h"

using namespace googleQt;

std::unique_ptr<GoogleException> GoogleException::create(const QByteArray& data, int status_code, const std::string& message)
{
    std::string summary;
    if (!data.isEmpty())
    {
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject js_in = doc.object();
        summary = js_in["error_summary"].toString().toStdString();
    }
    return std::unique_ptr<GoogleException>(new GoogleException(summary, status_code, message));
};

void GoogleException::build(std::string err)
{
    m_what = m_msg;
    m_what += "\n";
    if (!m_error_summary.empty())m_what += "SUMMARY:" + m_error_summary;
    m_what += "\n";
    if (!err.empty())
    {
        m_what += "ERROR:\n";
        m_what += err;
    }
}

GoogleException* GoogleException::clone()const 
{
    GoogleException* rv = new GoogleException(m_msg, m_status_code, m_error_summary);
    return rv;
};
