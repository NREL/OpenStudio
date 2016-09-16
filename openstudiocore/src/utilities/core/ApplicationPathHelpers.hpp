/**********************************************************************
*  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

  /// \returns The packaged EnergyPlus installation.
  UTILITIES_API openstudio::path getEnergyPlusDirectory();

  /// \returns The path to the packaged EnergyPlus executable.
  UTILITIES_API openstudio::path getEnergyPlusExecutable();

  /// \returns The packaged Radiance installation if it exists.
  UTILITIES_API boost::optional<openstudio::path> getRadianceDirectory();

} // openstudio

#endif //UTILITIES_CORE_PATHHELPERS_HPP
