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
