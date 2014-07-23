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

#include "StringStreamLogSink.hpp"
#include "StringStreamLogSink_Impl.hpp"

#include "Assert.hpp"

#include <boost/lexical_cast.hpp>

#include <sstream>

#include <QReadWriteLock>
#include <QWriteLocker>

namespace openstudio{

  namespace detail{

    StringStreamLogSink_Impl::StringStreamLogSink_Impl()
      : m_stringstream(new std::stringstream)
    {
      this->setStream(m_stringstream);
      this->enable();
    }

    StringStreamLogSink_Impl::~StringStreamLogSink_Impl()
    {
      this->disable();
    
      // already called
      //LogSink_Impl::~LogSink_Impl();
    }

    std::string StringStreamLogSink_Impl::string() const
    {
      QReadLocker l(m_mutex);

      return m_stringstream->str();
    }

    std::vector<LogMessage> StringStreamLogSink_Impl::logMessages() const
    {
      return LogMessage::parseLogText(this->string());
    }

    void StringStreamLogSink_Impl::resetStringStream()
    {
      QWriteLocker l(m_mutex);

      m_stringstream->str("");
    }

  }

  StringStreamLogSink::StringStreamLogSink()
    : LogSink(boost::shared_ptr<detail::StringStreamLogSink_Impl>(new detail::StringStreamLogSink_Impl()))
  {
    OS_ASSERT(getImpl<detail::StringStreamLogSink_Impl>());
  }

  std::string StringStreamLogSink::string() const
  {
    return this->getImpl<detail::StringStreamLogSink_Impl>()->string();
  }

  std::vector<LogMessage> StringStreamLogSink::logMessages() const
  {
    return this->getImpl<detail::StringStreamLogSink_Impl>()->logMessages();
  }

  void StringStreamLogSink::resetStringStream()
  {
    this->getImpl<detail::StringStreamLogSink_Impl>()->resetStringStream();
  }

} // openstudio
