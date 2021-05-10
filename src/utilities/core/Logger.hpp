/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_LOGGER_HPP
#define UTILITIES_CORE_LOGGER_HPP

#include "../UtilitiesAPI.hpp"

#include "Singleton.hpp"
#include "Exception.hpp"
#include "Compare.hpp"
#include "LogSink.hpp"

#include <boost/shared_ptr.hpp>

#include <sstream>
#include <set>
#include <map>
#include <shared_mutex>

/// defines method logChannel() to get a logger for a class
#define REGISTER_LOGGER(__logChannel__)        \
  static openstudio::LogChannel logChannel() { \
    return __logChannel__;                     \
  }

/// log a message from within a registered class
#define LOG(__level__, __message__) LOG_FREE(__level__, logChannel(), __message__);

/// log a message from within a registered class and throw an exception
#define LOG_AND_THROW(__message__) LOG_FREE_AND_THROW(logChannel(), __message__);

/// log a message from outside a registered class
#define LOG_FREE(__level__, __channel__, __message__)        \
  {                                                          \
    std::stringstream _ss1;                                  \
    _ss1 << __message__;                                     \
    openstudio::logFree(__level__, __channel__, _ss1.str()); \
  }

/// log a message from outside a registered class and throw an exception
#define LOG_FREE_AND_THROW(__channel__, __message__)             \
  {                                                              \
    LOG_FREE(Fatal, __channel__, __message__);                   \
    std::stringstream _ss2;                                      \
    _ss2 << __FILE__ << "@" << __LINE__ << " : " << __message__; \
    throw openstudio::Exception(_ss2.str());                     \
  }

namespace openstudio {

/// convenience function for SWIG, prefer macros in C++
UTILITIES_API void logFree(LogLevel level, const std::string& channel, const std::string& message);

/** Singleton logger class.  Singleton Logger object maintains logging state throughout
   *   program execution.
   */
class UTILITIES_API LoggerSingleton
{

  friend class Singleton<LoggerSingleton>;

 public:
  /// destructor, cleans up, writes xml file footers, etc
  ~LoggerSingleton();

  /// get logger for standard out
  LogSink standardOutLogger() const;

  /// get logger for standard error
  LogSink standardErrLogger() const;

  /// get a logger from a LogChannel enumeration, if logChannel does not
  /// exist a new logger will be set up at the default level
  LoggerType& loggerFromChannel(const LogChannel& logChannel);

 protected:
  friend class detail::LogSink_Impl;

  /// is the sink found in the logging core
  bool findSink(boost::shared_ptr<LogSinkBackend> sink);

  /// adds a sink to the logging core, equivalent to logSink.enable()
  void addSink(boost::shared_ptr<LogSinkBackend> sink);

  /// removes a sink to the logging core, equivalent to logSink.disable()
  void removeSink(boost::shared_ptr<LogSinkBackend> sink);

 private:
  /// private constructor
  LoggerSingleton();

  mutable std::shared_mutex m_mutex;

  /// standard out logger
  LogSink m_standardOutLogger;

  /// standard err logger
  LogSink m_standardErrLogger;

  /// map of std::string to logger
  typedef std::map<std::string, LoggerType, openstudio::IstringCompare> LoggerMapType;
  LoggerMapType m_loggerMap;

  /// current sinks, kept here so don't destruct when LogSink wrapper goes out of scope
  typedef std::set<boost::shared_ptr<LogSinkBackend>> SinkSetType;
  SinkSetType m_sinks;
};

#if _WIN32 || _MSC_VER

/// Explicitly instantiate and export LoggerSingleton Singleton template instance
/// so that the same instance is shared between the DLL's that link to Utilities.dll
UTILITIES_TEMPLATE_EXT template class UTILITIES_API openstudio::Singleton<LoggerSingleton>;

#endif

typedef openstudio::Singleton<LoggerSingleton> Logger;
}  // namespace openstudio

#endif  // UTILITIES_CORE_LOGGER_HPP
