#pragma once

#include <exception>
#include <string>
#include <memory>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonObject>

namespace googleQt{
  class GoogleException: public std::exception
  {
  public:
    explicit GoogleException(const std::string& message) :m_msg(message), m_status_code(0){}

    explicit GoogleException(const std::string& message, int code):
      m_msg(message),
      m_status_code(code)
    {};        
        
    explicit GoogleException(const std::string& message, int code, const std::string summary) :
        m_msg(message), m_status_code(code), m_error_summary(summary) {
        build("");
    };

    GoogleException* clone()const;

    virtual const char* what() const throw (){
      return m_msg.c_str();
    }

    virtual int statusCode()const throw(){
      return m_status_code;
    }        

    const std::string& errSummary()const { return m_error_summary; }

    static  std::unique_ptr<GoogleException> create(const QByteArray& data, int status_code, const std::string& message);
    virtual void raise() { throw *this; }


  protected:
      void build(std::string err);

  protected:
    std::string m_msg;
    int m_status_code;
    std::string m_error_summary;
    std::string m_what;
  };
};
