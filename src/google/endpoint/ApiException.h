#pragma once

#include <exception>
#include <string>

namespace googleQt{
  class GoogleException: public std::exception
  {
  public:
    explicit GoogleException(const std::string& message) :m_msg(message), m_status_code(0){}

    explicit GoogleException(const std::string& message, int code):
      m_msg(message),
      m_status_code(code)
    {};        
        
    virtual const char* what() const throw (){
      return m_msg.c_str();
    }

    virtual int statusCode()const throw(){
      return m_status_code;
    }        
        
  protected:
    std::string m_msg;
    int m_status_code;
  };

  class ReplyException: public GoogleException
  {
  public:
    explicit ReplyException(const std::string& message, int code, const std::string summary)
      :GoogleException(message, code), m_error_summary(summary){build("");};

    virtual const char* what() const throw (){
      return m_what.c_str();
    }

    const std::string& errSummary()const{return m_error_summary;}
        
        
  protected:
    void build(std::string err);
        
  protected:
    std::string m_error_summary;
    std::string m_what;
  };    

};
