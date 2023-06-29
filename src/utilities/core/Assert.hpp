/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
#  define BOOST_ENABLE_ASSERT_HANDLER
#else
#  define BOOST_ENABLE_ASSERT_HANDLER
#endif

// include after definitions
#include <boost/assert.hpp>

namespace boost {
inline void assertion_failed(char const* expr, char const* function, char const* file, long line) {
  std::stringstream ss;
  ss << "Assertion " << expr << " failed on line " << line << " of " << function << " in file " << file << ".";
  openstudio::Logger::instance().standardErrLogger().enable();
  LOG_FREE(Fatal, "BOOST_ASSERT", ss.str());
  assert(false);
}

inline void assertion_failed_msg(char const* expr, char const* msg, char const* function, char const* file, long line) {
  std::stringstream ss;
  ss << "Assertion " << expr << " failed on line " << line << " of " << function << " in file " << file << "." << '\n' << msg;
  openstudio::Logger::instance().standardErrLogger().enable();
  LOG_FREE(Fatal, "BOOST_ASSERT", ss.str());
  assert(false);
}
}  // namespace boost

#endif  // UTILITIES_CORE_ASSERT_HPP
