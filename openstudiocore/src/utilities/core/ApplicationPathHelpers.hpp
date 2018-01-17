/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef UTILITIES_CORE_APPLICATIONPATHHELPERS_HPP
#define UTILITIES_CORE_APPLICATIONPATHHELPERS_HPP

#include "../UtilitiesAPI.hpp"
#include "Path.hpp"
#include <vector>

namespace openstudio {

  /** If possible, creates an openstudio::path to a system directory from outdir, otherwise throws 
   *  a std::runtime_error. For use by auto-generation applications that are writing files for 
   *  OpenStudio. Requires that the path represented by outdir exist up to the parent level of the
   *  final directory. Outputs informative messages to std::cout. */
  UTILITIES_API openstudio::path getApplicationOutputDirectory(const std::string& outdir);

  /// \returns The source directory the application was built from
  UTILITIES_API openstudio::path getApplicationSourceDirectory();

  /// \returns The directory the application was built in
  UTILITIES_API openstudio::path getApplicationBuildDirectory();

  /// \returns The directory the application was configured to be installed in (install prefix)
  UTILITIES_API openstudio::path getApplicationInstallDirectory();

  /// \returns The directory the application binary lives in
  UTILITIES_API openstudio::path getApplicationRunDirectory();

  /// \returns True if the application is running from the build directory
  UTILITIES_API bool applicationIsRunningFromBuildDirectory();

  /// \returns The bare paths of the openstudio ruby libraries, with no root specified
  UTILITIES_API std::vector<openstudio::path> getOpenStudioBareRubyPaths();
  
  /// \returns The locations of the openstudio ruby libraries appropriate for this application instance can be found
  UTILITIES_API std::vector<openstudio::path> getOpenStudioRubyPaths();

  /// \returns The directory the openstudio ruby library appropriate for this application instance can be found
  UTILITIES_API openstudio::path getOpenStudioRubyPath();

  /** \returns The directory that should be included when making calls to ruby scripts from a ruby
   *  job (to make require 'openstudio' work). */
  UTILITIES_API openstudio::path getOpenStudioRubyIncludePath();

  /// \returns The openstudio ruby scripts library appropriate for this application instance.
  UTILITIES_API openstudio::path getOpenStudioRubyScriptsPath();

  /// \returns The location to load resources from
  UTILITIES_API openstudio::path getSharedResourcesPath();

  /// \returns The location of the ruby installation intended for aws-sdk use with aws-sdk
  ///          gem already installed
  UTILITIES_API openstudio::path getOpenStudioAWSRubyPath();

  /// \returns The packaged ruby installation from the embedded ruby version.
  ///          Returns nothing on non-windows platforms
  UTILITIES_API openstudio::path getOpenStudioEmbeddedRubyPath();

  /// \returns The packaged EnergyPlus installation.
  UTILITIES_API openstudio::path getEnergyPlusDirectory();

  /// \returns The path to the packaged EnergyPlus executable.
  UTILITIES_API openstudio::path getEnergyPlusExecutable();

  /// \returns The packaged Radiance installation if it exists.
  UTILITIES_API boost::optional<openstudio::path> getRadianceDirectory();

} // openstudio

#endif //UTILITIES_CORE_PATHHELPERS_HPP
