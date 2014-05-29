/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include "LogSink.hpp"
#include "LogSink_Impl.hpp"

#include "Logger.hpp"

#include <boost/log/common.hpp>
#include <boost/log/support/regex.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/expressions/attr_fwd.hpp>
#include <boost/log/expressions/attr.hpp>
#include <boost/log/attributes/value_extraction.hpp>

#include <QReadWriteLock>
#include <QWriteLocker>
#include <QThread>

namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;
namespace expr = boost::log::expressions;

namespace openstudio{

  namespace detail{

    LogSink_Impl::LogSink_Impl()
      : m_mutex(new QReadWriteLock()), m_threadId(nullptr)
    {
      m_sink = boost::shared_ptr<LogSinkBackend>(new LogSinkBackend());
    }

    LogSink_Impl::~LogSink_Impl()
    {
      delete m_mutex;
    }

    bool LogSink_Impl::isEnabled() const
    {
      return Logger::instance().findSink(m_sink);
    }

    void LogSink_Impl::enable()
    {
      Logger::instance().addSink(m_sink);
    }

    void LogSink_Impl::disable()
    {
      Logger::instance().removeSink(m_sink);
    }

    boost::optional<LogLevel> LogSink_Impl::logLevel() const
    {
      QReadLocker l(m_mutex);

      return m_logLevel;
    }
      
    void LogSink_Impl::setLogLevel(LogLevel logLevel)
    {
      QWriteLocker l(m_mutex);

      m_logLevel = logLevel;

      this->updateFilter(l);
    }

    void LogSink_Impl::resetLogLevel()
    {
      QWriteLocker l(m_mutex);

      m_logLevel.reset();

      this->updateFilter(l);
    }

    boost::optional<boost::regex> LogSink_Impl::channelRegex() const
    {
      QReadLocker l(m_mutex);

      return m_channelRegex;
    }
   
    void LogSink_Impl::setChannelRegex(const boost::regex& channelRegex)
    {
      QWriteLocker l(m_mutex);

      m_channelRegex = channelRegex;

      this->updateFilter(l);
    }

    void LogSink_Impl::resetChannelRegex()
    {
      QWriteLocker l(m_mutex);

      m_channelRegex.reset();

      this->updateFilter(l);
    }

    bool LogSink_Impl::autoFlush() const
    {
      QReadLocker l(m_mutex);

      return m_autoFlush;
    }
    
    void LogSink_Impl::setAutoFlush(bool autoFlush)
    {
      QWriteLocker l(m_mutex);

      m_autoFlush = autoFlush;

      m_sink->locked_backend()->auto_flush(autoFlush);
    }
  
    QThread* LogSink_Impl::threadId() const
    {
      QWriteLocker l(m_mutex);

      return m_threadId;
    }

    void LogSink_Impl::setThreadId(QThread* threadId)
    {
      QWriteLocker l(m_mutex);

      m_threadId = threadId;

      this->updateFilter(l);
    }

    void LogSink_Impl::resetThreadId()
    {
      QWriteLocker l(m_mutex);

      m_threadId = nullptr;

      this->updateFilter(l);
    }

    void LogSink_Impl::setStream(boost::shared_ptr<std::ostream> os)
    {
      QWriteLocker l(m_mutex);

      m_sink->locked_backend()->add_stream(os);

      // set formatting, seems like you have to call this after the stream is added
      // DLM@20110701: would like to format Severity as string but can't figure out how to do it
      // because you can't overload operator<< for an enum type
      // this seems to suggest this should work: http://www.edm2.com/0405/enumeration.html
      m_sink->set_formatter(expr::stream
        << "[" << expr::attr< LogChannel >("Channel")
        << "] <" << expr::attr< LogLevel >("Severity")
        << "> " << expr::smessage);

      //m_sink->locked_backend()->set_formatter(fmt::stream
      //  << "[" << fmt::attr< LogChannel >("Channel")
      //  << "] <" << fmt::attr< LogLevel >("Severity")
      //  << "> @" << fmt::attr< QThread* >("QThread") << ", " << fmt::message());

      this->updateFilter(l);

      // avoid deadlock
      l.unlock();

      this->setAutoFlush(true);  
    }
      
    boost::shared_ptr<LogSinkBackend> LogSink_Impl::sink() const
    {
      QReadLocker l(m_mutex);

      return m_sink;
    }

    void LogSink_Impl::updateFilter(const QWriteLocker& l)
    {
      m_sink->reset_filter();

      LogLevel filterLogLevel = Trace;
      if (m_logLevel){
        filterLogLevel = *m_logLevel;
      }

      boost::regex filterChannelRegex(".*");
      if (m_channelRegex){
        filterChannelRegex = *m_channelRegex;
      }

      if (m_threadId){
        m_sink->set_filter(expr::attr< LogLevel >("Severity") >= filterLogLevel &&
                           expr::attr< QThread* >("QThread") == m_threadId &&
                           expr::matches(expr::attr< LogChannel >("Channel"), filterChannelRegex));
      }else{
        m_sink->set_filter(expr::attr< LogLevel >("Severity") >= filterLogLevel &&
                           expr::matches(expr::attr< LogChannel >("Channel"), filterChannelRegex));
      }
     
    }

  } // detail

  LogSink::LogSink()
    : m_impl(new detail::LogSink_Impl())
  {}

  LogSink::LogSink(const boost::shared_ptr<detail::LogSink_Impl>& impl)
    : m_impl(impl)
  {}

  bool LogSink::isEnabled() const
  {
    return m_impl->isEnabled();
  }

  void LogSink::enable()
  {
    m_impl->enable();
  }

  void LogSink::disable()
  {
    m_impl->disable();
  }

  boost::optional<LogLevel> LogSink::logLevel() const
  {
    return m_impl->logLevel();
  }
    
  void LogSink::setLogLevel(LogLevel logLevel)
  {
    m_impl->setLogLevel(logLevel);
  }

  void LogSink::resetLogLevel()
  {
    m_impl->resetLogLevel();
  }

  boost::optional<boost::regex> LogSink::channelRegex() const
  {
    return m_impl->channelRegex();
  }
 
  void LogSink::setChannelRegex(const boost::regex& channelRegex)
  {
    m_impl->setChannelRegex(channelRegex);
  }

  void LogSink::resetChannelRegex()
  {
    m_impl->resetChannelRegex();
  }

  bool LogSink::autoFlush() const
  {
    return m_impl->autoFlush();
  }
  
  void LogSink::setAutoFlush(bool autoFlush)
  {
    m_impl->setAutoFlush(autoFlush);
  }

  QThread* LogSink::threadId() const
  {
    return m_impl->threadId();
  }

  void LogSink::setThreadId(QThread* threadId)
  {
    m_impl->setThreadId(threadId);
  }

  void LogSink::resetThreadId()
  {
    m_impl->resetThreadId();
  }

  void LogSink::setStream(boost::shared_ptr<std::ostream> os)
  {
    m_impl->setStream(os);
  }
    
  boost::shared_ptr<LogSinkBackend> LogSink::sink() const
  {
    return m_impl->sink();
  }


} // openstudio
