/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_XML_XMLERRORS_HPP
#define UTILITIES_XML_XMLERRORS_HPP

#include "../core/LogMessage.hpp"
#include <vector>

using xmlError = struct _xmlError;

namespace openstudio {

namespace detail {

  struct ErrorCollector
  {
    std::vector<LogMessage> logMessages;
  };

  // Some callback function to collect messages into the ErrorCollector structure
  // Pass the pointer to the ErrorCollector instnace as libxml2's void* ctx argument (the first arg)
  // Could have passed XMLValidator directly, but I don't want to have to expose public functions nor friend these free functions
  void callback_messages_structured_error(void* errorCollectorVoidPtr, xmlError* error);

  void callback_messages_warning(void* errorCollectorVoidPtr, const char* fmt, ...);
  void callback_messages_error(void* errorCollectorVoidPtr, const char* fmt, ...);

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_XML_XMLERRORS_HPP
