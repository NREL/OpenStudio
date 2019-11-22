/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "Logger.hpp"

#include <boost/log/common.hpp>
#include <boost/log/attributes/function.hpp>

#include <boost/utility/empty_deleter.hpp>


#include <QReadWriteLock>
#include <QThread>

namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

namespace openstudio{

  // handle Qt messages
  void logQtMessage(QtMsgType type, const char *msg)
  {
    switch (type) {
      case QtDebugMsg:
        logFree(Debug, "Qt", msg);
        break;
#if QT_VERSION >= 0x050500
      case QtInfoMsg:
        logFree(Info, "Qt", msg);
        break;
#endif
      case QtWarningMsg:
        logFree(Warn, "Qt", msg);
        break;
      case QtCriticalMsg:
        logFree(Error, "Qt", msg);
        break;
      case QtFatalMsg:
        logFree(Fatal, "Qt", msg);
        break;
    }
  }

  // handle Qt messages after logger is destroyed
  void consoleLogQtMessage(QtMsgType type, const char *msg)
  {
    std::cout << "[Qt] <" << type << "> " << msg << std::endl;
  }

  /// convenience function for SWIG, prefer macros in C++
  void logFree(LogLevel level, const std::string& channel, const std::string& message)
  {
    BOOST_LOG_SEV(openstudio::Logger::instance().loggerFromChannel(channel), level) << message;
  }

  LoggerSingleton::LoggerSingleton()
    : m_mutex(new QReadWriteLock())
  {
    // Make QThread attribute available to logging
    boost::log::core::get()->add_global_attribute("QThread", boost::log::attributes::make_function(&QThread::currentThread));

    // We have to provide an empty deleter to avoid destroying the global stream
    boost::shared_ptr<std::ostream> stdOut(&std::cout, boost::empty_deleter());
    m_standardOutLogger.setStream(stdOut);
    m_standardOutLogger.setLogLevel(Warn);
    this->addSink(m_standardOutLogger.sink());

    // We have to provide an empty deleter to avoid destroying the global stream
    boost::shared_ptr<std::ostream> stdErr(&std::cerr, boost::empty_deleter());
    m_standardErrLogger.setStream(stdErr);
    m_standardErrLogger.setLogLevel(Warn);
    //this->addSink(m_standardErrLogger.sink());

    // register Qt message handler
    //qInstallMsgHandler(logQtMessage);
  }

  LoggerSingleton::~LoggerSingleton()
  {
    // unregister Qt message handler
    //qInstallMsgHandler(consoleLogQtMessage);

    delete m_mutex;
  }

  LogSink LoggerSingleton::standardOutLogger() const
  {
    QReadLocker l(m_mutex);

    return m_standardOutLogger;
  }

  LogSink LoggerSingleton::standardErrLogger() const
  {
    QReadLocker l(m_mutex);

    return m_standardErrLogger;
  }

  LoggerType& LoggerSingleton::loggerFromChannel(const LogChannel& logChannel)
  {
    QReadLocker l(m_mutex);

    auto it = m_loggerMap.find(logChannel);
    if (it == m_loggerMap.end()){
      //LoggerType newLogger(keywords::channel = logChannel, keywords::severity = Debug);
      LoggerType newLogger(keywords::channel = logChannel);

      std::pair<LogChannel, LoggerType> newPair(logChannel,newLogger);

      // Drop the read lock and grab a write lock - we need to add the new file to the map
      // this will reduce contention when multiple threads trying to log at once.
      l.unlock();
      QWriteLocker l2(m_mutex);

      std::pair<LoggerMapType::iterator, bool> inserted = m_loggerMap.insert(newPair);

      return inserted.first->second;

    }

    return it->second;
  }

  bool LoggerSingleton::findSink(boost::shared_ptr<LogSinkBackend> sink)
  {
    QWriteLocker l(m_mutex);

    auto it = m_sinks.find(sink);

    return (it != m_sinks.end());
  }

  void LoggerSingleton::addSink(boost::shared_ptr<LogSinkBackend> sink)
  {
    QWriteLocker l(m_mutex);

    auto it = m_sinks.find(sink);
    if (it == m_sinks.end()){

      // Drop the read lock and grab a write lock - we need to add the new file to the map
      // this will reduce contention when multiple threads trying to log at once.
      l.unlock();
      QWriteLocker l2(m_mutex);

      m_sinks.insert(sink);

      // Register the sink in the logging core
      boost::log::core::get()->add_sink(sink);
    }
  }

  void LoggerSingleton::removeSink(boost::shared_ptr<LogSinkBackend> sink)
  {
    QWriteLocker l(m_mutex);

    auto it = m_sinks.find(sink);
    if (it != m_sinks.end()){

      // Drop the read lock and grab a write lock - we need to add the new file to the map
      // this will reduce contention when multiple threads trying to log at once.
      l.unlock();
      QWriteLocker l2(m_mutex);

      m_sinks.erase(it);

      // Register the sink in the logging core
      boost::log::core::get()->remove_sink(sink);
    }
  }

} // openstudio
