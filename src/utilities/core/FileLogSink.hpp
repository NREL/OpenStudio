/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_FILELOGSINK_HPP
#define UTILITIES_CORE_FILELOGSINK_HPP

#include "../UtilitiesAPI.hpp"

#include "LogSink.hpp"
#include "Path.hpp"

namespace openstudio {

class UTILITIES_API FileLogSink : public LogSink
{
 public:
  /// constructor takes path of file, opens in write mode positioned at file beginning
  /// and registers in the global logger
  FileLogSink(const openstudio::path& path);

  /// returns the path that log messages are written to
  openstudio::path path() const;

  /// get messages out of the file content
  std::vector<LogMessage> logMessages() const;
};

}  // namespace openstudio

#endif  // UTILITIES_CORE_FILELOGSINK_HPP
