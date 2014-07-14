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

#ifndef UTILITIES_CORE_STRINGSTREAMLOGSINK_IMPL_HPP
#define UTILITIES_CORE_STRINGSTREAMLOGSINK_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "LogSink_Impl.hpp"

namespace openstudio{

  namespace detail {

    class UTILITIES_API StringStreamLogSink_Impl : public LogSink_Impl
    {
      public:

      /// constructor makes a new string stream to write to and registers in the global logger
      StringStreamLogSink_Impl();

      /// destructor, disables log sink
      virtual ~StringStreamLogSink_Impl();

      /// get the string stream's content
      std::string string() const;

      /// get messages out of the string stream's content
      std::vector<LogMessage> logMessages() const;

      /// reset the string stream's content
      void resetStringStream();

      private:

      boost::shared_ptr<std::stringstream> m_stringstream;

    };

  } // detail

} // openstudio

#endif // UTILITIES_CORE_STRINGSTREAMLOGSINK_IMPL_HPP
