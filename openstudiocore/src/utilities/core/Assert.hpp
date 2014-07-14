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

#ifndef UTILITIES_CORE_ASSERT_HPP
#define UTILITIES_CORE_ASSERT_HPP

/****************************************************************************
!!! THIS FILE MUST BE INCLUDED BY ANY SOURCE FILE THAT USES OPENSTUDIO_ASSERT!!!
!!! THIS FILE MUST BE INCLUDED BY ANY SOURCE FILE THAT USES BOOST_ASSERT!!!
*****************************************************************************/

#include "../UtilitiesAPI.hpp"
#include "Logger.hpp"

#include <sstream>

#define OS_ASSERT(expr) BOOST_ASSERT(expr)

#ifdef NDEBUG
  //#define BOOST_DISABLE_ASSERTS
  #define BOOST_ENABLE_ASSERT_HANDLER
#else
  #define BOOST_ENABLE_ASSERT_HANDLER
#endif

// include after definitions
#include <boost/assert.hpp>

namespace boost {
  inline void assertion_failed(char const * expr, char const * function, char const * file, long line) {
    std::stringstream ss;
    ss << "Assertion " << expr << " failed on line " << line << " of " << function << " in file " << file << ".";
    openstudio::Logger::instance().standardErrLogger().enable();
    LOG_FREE(Fatal, "BOOST_ASSERT", ss.str());
    assert(false);
  }

  inline void assertion_failed_msg(char const * expr, char const * msg, char const * function, char const * file, long line)
  {
    std::stringstream ss;
    ss << "Assertion " << expr << " failed on line " << line << " of " << function << " in file " << file << "." << std::endl << msg;
    openstudio::Logger::instance().standardErrLogger().enable();
    LOG_FREE(Fatal, "BOOST_ASSERT", ss.str());
    assert(false);
  }
}

#endif // UTILITIES_CORE_ASSERT_HPP
