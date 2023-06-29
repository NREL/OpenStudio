/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_RUBYEXCEPTION_HPP
#define UTILITIES_CORE_RUBYEXCEPTION_HPP

#include <stdexcept>

#include <boost/regex.hpp>

namespace openstudio {

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

  std::string shortLocation() const {
    std::string result;

    boost::regex regex("\\w*\\.rb:\\d*");
    boost::smatch m;
    if (boost::regex_search(m_location, m, regex)) {
      result = m[0];
    }

    return result;
  }

 private:
  std::string m_location;
};

}  // namespace openstudio

#endif  // UTILITIES_CORE_RUBYEXCEPTION_HPP
