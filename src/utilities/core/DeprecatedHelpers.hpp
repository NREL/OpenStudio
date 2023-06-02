/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_CORE_DEPRECATEDHELPERS_HPP
#define UTILITIES_CORE_DEPRECATEDHELPERS_HPP

#include "Compare.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

#include <OpenStudio.hxx>

#include <boost/current_function.hpp>
#include <fmt/core.h>

#include <string>

#define DEPRECATED_AT(__deprecatedAt__) \
  openstudio::detail::log_deprecation_and_throw_if_time_to_remove(__deprecatedAt__, logChannel(), BOOST_CURRENT_FUNCTION)

#define DEPRECATED_AT_MSG(__deprecatedAt__, __message__) \
  openstudio::detail::log_deprecation_and_throw_if_time_to_remove(__deprecatedAt__, logChannel(), BOOST_CURRENT_FUNCTION, __message__)

namespace openstudio {

namespace detail {
  inline void log_deprecation_and_throw_if_time_to_remove(const std::string& deprecatedAt, const std::string& logChannel,
                                                          const std::string& methodName, const std::string& extraMessage = "") {
    if (extraMessage.empty()) {
      LOG_FREE(Warn, logChannel, "As of " << deprecatedAt << ", " << methodName << " is deprecated.");
    } else {
      LOG_FREE(Warn, logChannel, "As of " << deprecatedAt << ", " << methodName << " is deprecated. " << extraMessage);
    }

    const VersionString currentVersion(openStudioVersion());
    const VersionString deprecatedAtVersion(deprecatedAt);
    const bool isTimeToRemoveDeprecated =
      ((currentVersion.major() == deprecatedAtVersion.major()) || (currentVersion.minor() >= deprecatedAtVersion.minor() + 3));
    if (isTimeToRemoveDeprecated) {
      throw openstudio::Exception(fmt::format("Time to remove {} from the API", methodName));
    }
  }
}  // namespace detail
}  // namespace openstudio

#endif  // UTILITIES_CORE_DEPRECATEDHELPERS_HPP
