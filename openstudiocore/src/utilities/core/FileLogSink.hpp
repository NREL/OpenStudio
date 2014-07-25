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

#ifndef UTILITIES_CORE_FILELOGSINK_HPP
#define UTILITIES_CORE_FILELOGSINK_HPP

#include "../UtilitiesAPI.hpp"

#include "LogSink.hpp"
#include "Path.hpp"

namespace openstudio{

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

} // openstudio

#endif // UTILITIES_CORE_FILELOGSINK_HPP
