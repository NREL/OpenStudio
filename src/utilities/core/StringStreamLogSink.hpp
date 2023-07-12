/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_STRINGSTREAMLOGSINK_HPP
#define UTILITIES_CORE_STRINGSTREAMLOGSINK_HPP

#include "../UtilitiesAPI.hpp"

#include "LogSink.hpp"

namespace openstudio {

class UTILITIES_API StringStreamLogSink : public LogSink
{
 public:
  /// constructor makes a new string stream to write to and registers in the global logger
  StringStreamLogSink();

  /// get the string stream's content
  std::string string() const;

  /// get messages out of the string stream's content
  std::vector<LogMessage> logMessages() const;

  /// reset the string stream's content
  void resetStringStream();
};

}  // namespace openstudio

#endif  // UTILITIES_CORE_STRINGSTREAMLOGSINK_HPP
