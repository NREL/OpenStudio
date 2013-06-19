#include "logger.hpp"
#
#include <iostream>

using namespace std; 
using namespace litesql; 

static bool logger_verbose = false;

void Logger::verbose(bool verbose)
{
  logger_verbose = verbose;
}

void Logger::report(const string& msg,const string& msg2)
{
  if (logger_verbose)
    cout  << msg  << msg2 << endl;
}

void Logger::report(const string& msg)
{
  if (logger_verbose)
    cout  << msg << endl;
}

void Logger::error(const std::string& msg)
{
    cerr  << msg << endl;
}

void Logger::error(const std::string& msg,const std::string& msg2)
{
    cerr  << msg << msg2 << endl;
}

void Logger::error(const Except& ex)
{
    cerr  << ex << endl;
}


