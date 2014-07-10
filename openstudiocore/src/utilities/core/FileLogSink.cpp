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

#include "FileLogSink.hpp"
#include "FileLogSink_Impl.hpp"

#include "Assert.hpp"

#include <QReadWriteLock>
#include <QWriteLocker>

namespace openstudio{

  namespace detail{

    FileLogSink_Impl::FileLogSink_Impl(const openstudio::path& path)
      : m_path(path)
    {
      m_ofs = boost::shared_ptr<boost::filesystem::ofstream>(new boost::filesystem::ofstream(path));
      this->setStream(m_ofs);
      this->enable();
    }

    FileLogSink_Impl::~FileLogSink_Impl()
    {
      // already called
      //LogSink_Impl::~LogSink_Impl();
    }

    openstudio::path FileLogSink_Impl::path() const
    {
      QReadLocker l(m_mutex);

      return m_path;
    }

    std::vector<LogMessage> FileLogSink_Impl::logMessages() const
    {
      boost::filesystem::ifstream ifs(m_path);
      std::string line;
      std::string text;
      while(std::getline(ifs, line)){
        text += line + "\n";
      }
      return LogMessage::parseLogText(text);
    }
  } // detail

  FileLogSink::FileLogSink(const openstudio::path& path)
    : LogSink(boost::shared_ptr<detail::FileLogSink_Impl>(new detail::FileLogSink_Impl(path)))
  {
    OS_ASSERT(getImpl<detail::FileLogSink_Impl>());
  }

  openstudio::path FileLogSink::path() const
  {
    return this->getImpl<detail::FileLogSink_Impl>()->path();
  }

  std::vector<LogMessage> FileLogSink::logMessages() const
  {
    return this->getImpl<detail::FileLogSink_Impl>()->logMessages();
  }

} // openstudio
