/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_EXCEPTION_HPP
#define UTILITIES_CORE_EXCEPTION_HPP

#include <string>
#include <exception>
#include "../UtilitiesAPI.hpp"

namespace openstudio {

/** Base class for exceptions. */
class UTILITIES_API Exception : public std::exception
{
 public:
  /// Constructor takes a message
  Exception(const std::string& msg) : m_msg(msg) {}

  /// Needed for rb_raise support in swig
  Exception(const char* msg) : m_msg(msg) {}

  Exception(const Exception&) = default;
  Exception& operator=(const Exception&) = default;
  Exception(Exception&&) = default;
  Exception& operator=(Exception&&) = default;
  virtual ~Exception() noexcept override = default;

  /// return the message
  virtual const std::string& message() const {
    return m_msg;
  }

  /// return the message
  virtual const char* what() const noexcept override {
    return m_msg.c_str();
  }

 private:
  std::string m_msg;
};

}  // namespace openstudio

#endif  // UTILITIES_CORE_EXCEPTION_HPP
