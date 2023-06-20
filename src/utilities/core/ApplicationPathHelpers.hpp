/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_APPLICATIONPATHHELPERS_HPP
#define UTILITIES_CORE_APPLICATIONPATHHELPERS_HPP

#include "../UtilitiesAPI.hpp"
#include "Path.hpp"
#include <vector>

namespace openstudio {

/// \returns The source directory the application was built from
UTILITIES_API openstudio::path getApplicationSourceDirectory();

/// \returns The directory the application was built in
UTILITIES_API openstudio::path getApplicationBuildDirectory();

/// \returns The path to the current executable application
UTILITIES_API openstudio::path getApplicationPath();

/// \returns The directory of the current executable application
UTILITIES_API openstudio::path getApplicationDirectory();

/// \returns True if the application is running from the build directory
UTILITIES_API bool applicationIsRunningFromBuildDirectory();

/// \returns Will return path to the binary containing OpenStudio Utilities, could be openstudio.exe, openstudio.so, etc.
UTILITIES_API openstudio::path getOpenStudioModule();

/// \returns Will return dir containing the binary containing OpenStudio Utilities, could be openstudio.exe, openstudio.so, etc.
UTILITIES_API openstudio::path getOpenStudioModuleDirectory();

/// \returns True if the OpenStudio Module is running from the build directory
UTILITIES_API bool moduleIsRunningFromBuildDirectory();

/// \returns The path to the OpenStudio Command Line Interface  if it exists.
UTILITIES_API openstudio::path getOpenStudioCLI();

/// \returns The packaged EnergyPlus installation if it exists.
UTILITIES_API openstudio::path getEnergyPlusDirectory();

/// \returns The path to the EnergyPlus executable if it exists.
UTILITIES_API openstudio::path getEnergyPlusExecutable();

/// \returns The path to the Radiance installation if it exists.
UTILITIES_API openstudio::path getRadianceDirectory();

/// \returns The path to the Perl executable if it exists.
UTILITIES_API openstudio::path getPerlExecutable();

/// \returns The platform specific suffix for shared modules.
/// "dll" on Windows, ".so" on Linux, etc.
UTILITIES_API std::string_view getSharedModuleSuffix();

/// \returns The platform specific file prefix for shared modules.
/// "lib" on Linux.
UTILITIES_API std::string_view getSharedModulePrefix();

/// \returns The platform specific shared module name,
/// concatenates the prefix and suffix to the given basename
UTILITIES_API std::string getSharedModuleName(std::string_view basename);

}  // namespace openstudio

#endif  //UTILITIES_CORE_PATHHELPERS_HPP
