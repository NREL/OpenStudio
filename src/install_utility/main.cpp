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

#include <iostream>
#include <string>
#include <cstdlib>
#include <regex>
#include <sstream>
#if defined __APPLE__
#  include <mach-o/dyld.h> /* _NSGetExecutablePath */
#  include <limits.h>      /* PATH_MAX */
#elif defined _WIN32
#  include <windows.h>
#endif
#include "../utilities/core/Filesystem.hpp"

#ifdef __APPLE__
#  include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

inline std::string applicationFilePath() {
#ifdef __APPLE__
  // This is for create_symlink which is not included in utilities core filesystem
  char path[PATH_MAX + 1];
  uint32_t size = sizeof(path);
  if (_NSGetExecutablePath(path, &size) == 0) {
    return {path};
  }
#elif defined _WIN32
  TCHAR szPath[MAX_PATH];
  if (GetModuleFileName(nullptr, szPath, MAX_PATH)) {
    return std::string(szPath);
  }
#endif
  return {};
}

int main(int argc, char* argv[]) {

#ifdef __APPLE__
  if (argc > 1u) {
    openstudio::filesystem::path appDir = openstudio::filesystem::path(applicationFilePath()).parent_path();
    openstudio::filesystem::path cliPath = appDir / "openstudio";
    openstudio::filesystem::path appPath = appDir.parent_path() / "OpenStudioApp.app";

    if (std::string(argv[1]) == "Install") {
      try {
        if (openstudio::filesystem::exists(cliPath)) {
          openstudio::filesystem::path dir("/usr/local/bin");
          if (!openstudio::filesystem::exists(dir)) {
            openstudio::filesystem::create_directories(dir);
          }
          try {
            openstudio::filesystem::remove("/usr/local/bin/openstudio");
          } catch (...) {
          }
          fs::create_symlink(cliPath.string(), "/usr/local/bin/openstudio");
        }
      } catch (...) {
      }

    } else if (std::string(argv[1]) == "Remove") {
      try {
        openstudio::filesystem::remove("/usr/local/bin/openstudio");
      } catch (...) {
      }
    }
  }
#endif

#if defined _WIN32
  if (argc > 1u) {
    if (std::string(argv[1]) == "Install") {
      openstudio::filesystem::path appDir = openstudio::filesystem::path(applicationFilePath()).parent_path();
      openstudio::filesystem::path cliPath = appDir / "openstudio.exe";
      openstudio::filesystem::path mongoPath = appDir.parent_path() / "pat\\mongo\\bin\\mongod.exe";

      std::ostringstream oss;
      oss << "netsh advfirewall firewall add rule name=\"Allow OpenStudio CLI\" dir=in program="
          << "\"" << cliPath.string() << "\" "
          << "action=allow";
      std::system(oss.str().c_str());

      std::ostringstream oss2;
      oss2 << "netsh advfirewall firewall add rule name=\"Allow OpenStudio Mongo\" dir=in program="
           << "\"" << mongoPath.string() << "\" "
           << "action=allow";
      std::system(oss2.str().c_str());

    } else if (std::string(argv[1]) == "Remove") {
      std::ostringstream oss;
      oss << "netsh advfirewall firewall delete rule name=\"Allow OpenStudio CLI\"";
      std::system(oss.str().c_str());

      std::ostringstream oss2;
      oss2 << "netsh advfirewall firewall delete rule name=\"Allow OpenStudio Mongo\"";
      std::system(oss2.str().c_str());
    }
  }
#endif

  return 0;
}
