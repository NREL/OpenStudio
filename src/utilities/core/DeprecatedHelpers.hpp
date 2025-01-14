/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_DEPRECATEDHELPERS_HPP
#define UTILITIES_CORE_DEPRECATEDHELPERS_HPP

#include "Compare.hpp"
#include "Exception.hpp"
#include "Logger.hpp"

#include <OpenStudio.hxx>

#include <boost/current_function.hpp>
#include <fmt/core.h>
#include <fmt/compile.h>

#include <string>

namespace openstudio {

struct cx_VersionString
{
  constexpr cx_VersionString(int t_major, int t_minor, int t_patch) : major(t_major), minor(t_minor), patch(t_patch) {}

  constexpr cx_VersionString() = default;

  constexpr bool isGreaterThan(const cx_VersionString& other, int numMinors = 3) const {
    return (major > other.major) || (minor >= other.minor + numMinors);
  }

  std::string str() const {
    return fmt::format("{}.{}.{}", major, minor, patch);
  }

  int major = openstudio::detail::cx_openStudioVersionMajor();
  int minor = openstudio::detail::cx_openStudioVersionMinor();
  int patch = openstudio::detail::cx_openStudioVersionPatch();
};

// static inline constexpr cx_VersionString cx_currentVersion{};

namespace detail {
  inline void log_deprecation_and_throw_if_time_to_remove(const std::string& deprecatedAt, const std::string& logChannel,
                                                          const std::string& methodName, const std::string& extraMessage = "") {
    if (extraMessage.empty()) {
      LOG_FREE(Warn, logChannel, "As of " << deprecatedAt << ", " << methodName << " is deprecated.");
    } else {
      LOG_FREE(Warn, logChannel, "As of " << deprecatedAt << ", " << methodName << " is deprecated. " << extraMessage);
    }
  }
}  // namespace detail
}  // namespace openstudio

#define DEPRECATED_AT(__deprecatedAtVersionMajor__, __deprecatedAtVersionMinor__, __deprecatedAtVersionPatch__) \
  DEPRECATED_AT_MSG(__deprecatedAtVersionMajor__, __deprecatedAtVersionMinor__, __deprecatedAtVersionPatch__, "")

#define DEPRECATED_AT_MSG(__deprecatedAtVersionMajor__, __deprecatedAtVersionMinor__, __deprecatedAtVersionPatch__, __message__)                     \
  constexpr openstudio::cx_VersionString deprecatedAt(__deprecatedAtVersionMajor__, __deprecatedAtVersionMinor__, __deprecatedAtVersionPatch__);     \
  static_assert(true || !openstudio::cx_VersionString{}.isGreaterThan(deprecatedAt),                                                                 \
                "Time to remove, deprecated at " #__deprecatedAtVersionMajor__ "." #__deprecatedAtVersionMinor__ "." #__deprecatedAtVersionPatch__); \
  openstudio::detail::log_deprecation_and_throw_if_time_to_remove(deprecatedAt.str(), logChannel(), BOOST_CURRENT_FUNCTION, __message__)

#endif  // UTILITIES_CORE_DEPRECATEDHELPERS_HPP
