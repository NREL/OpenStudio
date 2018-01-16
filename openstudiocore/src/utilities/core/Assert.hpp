/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
