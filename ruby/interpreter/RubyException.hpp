/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef CLI_RUBYEXCEPTION_HPP
#define CLI_RUBYEXCEPTION_HPP

#include <stdexcept>
#include <string>

class RubyException : public std::runtime_error
{
 public:
  RubyException(const std::string& msg, const std::string& location) : std::runtime_error(msg), m_location(location) {}
  RubyException(const RubyException&) = default;
  RubyException& operator=(const RubyException&) = default;
  RubyException(RubyException&&) = default;
  RubyException& operator=(RubyException&&) = default;
  virtual ~RubyException() noexcept override = default;

  std::string location() const {
    return m_location;
  }

 private:
  std::string m_location;
};

#endif  // CLI_RUBYEXCEPTION_HPP
