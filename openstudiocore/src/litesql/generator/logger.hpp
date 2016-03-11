#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <string>
#include "litesql/except.hpp"

namespace litesql 
{
  class Logger {
  public:
    static void report(const std::string& msg,const std::string& msg2);
    static void report(const std::string& msg);
    static void error(const std::string& msg,const std::string& msg2);
    static void error(const std::string& msg2);
    static void error(const Except& ex);
    static void verbose(bool verbose);
  };
}

#endif // #ifndef LOGGER_HPP
