/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_LOGGER_IMPL_HPP
#define UTILITIES_CORE_LOGGER_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "Compare.hpp"
#include "LogSink.hpp"

#include <boost/shared_ptr.hpp>

#include <map>
#include <set>
#include <shared_mutex>

namespace openstudio {

class Logger;

namespace detail {

  class UTILITIES_API Logger_Impl
  {
   public:
    /// get logger for standard out
    LogSink standardOutLogger() const;

    /// get logger for standard error
    LogSink standardErrLogger() const;

    /// get a logger from a LogChannel enumeration, if logChannel does not
    /// exist a new logger will be set up at the default level
    LoggerType& loggerFromChannel(const LogChannel& logChannel);

    /// is the sink found in the logging core
    bool findSink(boost::shared_ptr<LogSinkBackend> sink);

    /// adds a sink to the logging core, equivalent to logSink.enable()
    void addSink(boost::shared_ptr<LogSinkBackend> sink);

    /// removes a sink to the logging core, equivalent to logSink.disable()
    void removeSink(boost::shared_ptr<LogSinkBackend> sink);

    // cppcheck-suppress functionConst
    void addTimeStampToLogger();

   protected:
    friend class openstudio::Logger;
    Logger_Impl();

   private:
    mutable std::shared_mutex m_mutex;

    /// standard out logger
    LogSink m_standardOutLogger;

    /// standard err logger
    LogSink m_standardErrLogger;

    /// map of std::string to logger
    using LoggerMapType = std::map<std::string, LoggerType, openstudio::IstringCompare>;
    LoggerMapType m_loggerMap;

    /// current sinks, kept here so don't destruct when LogSink wrapper goes out of scope
    using SinkSetType = std::set<boost::shared_ptr<LogSinkBackend>>;
    SinkSetType m_sinks;
  };

}  // namespace detail

}  // namespace openstudio

#endif  // UTILITIES_CORE_LOGGER_IMPL_HPP
