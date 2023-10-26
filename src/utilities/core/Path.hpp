/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_PATH_HPP
#define UTILITIES_CORE_PATH_HPP

#include "../UtilitiesAPI.hpp"

#include <ostream>
#include <boost/optional.hpp>
#include <string>
#include <string_view>

#include "Filesystem.hpp"

namespace openstudio {

// allow wpath to be written to cout on Windows
UTILITIES_API ::std::ostream& operator<<(::std::ostream& os, const path& p);

/** path to a temporary directory. */
UTILITIES_API path tempDir();

/** path to std::string. */
UTILITIES_API std::string toString(const path& p);

/** UTF-8 encoded char* to path*/
UTILITIES_API path toPath(const char* s);

/** UTF-8 encoded std::string to path*/
UTILITIES_API path toPath(const std::string& s);

UTILITIES_API path toPath(std::string_view s);

#ifdef _WIN32
/** UTF-16 encoded std::wstring for opening fstreams*/
UTILITIES_API std::wstring toSystemFilename(const path& p);
#else
/** UTF-8 encoded std::string for opening fstreams*/
UTILITIES_API std::string toSystemFilename(const path& p);
#endif

/** Optional path*/
using OptionalPath = boost::optional<path>;

using PathPair = std::pair<path, path>;

/// Return the fully expanded Windows path name ie "c:\Progra~1" into "C:\Program Files"
//UTILITIES_API QString longPathName(const QString& path);

}  // namespace openstudio

#endif  // UTILITIES_CORE_PATH_HPP
