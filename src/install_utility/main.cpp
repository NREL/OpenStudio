/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

int main(int argc, const char* argv[]) {

#ifdef __APPLE__
  if (argc > 1u) {
    openstudio::filesystem::path appDir = openstudio::filesystem::path(applicationFilePath()).parent_path();
    openstudio::filesystem::path cliPath = appDir / "openstudio";

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
      oss << "netsh advfirewall firewall add rule name=\"Allow OpenStudio CLI\" dir=in program=" << "\"" << cliPath.string() << "\" "
          << "action=allow";
      std::system(oss.str().c_str());

      std::ostringstream oss2;
      oss2 << "netsh advfirewall firewall add rule name=\"Allow OpenStudio Mongo\" dir=in program=" << "\"" << mongoPath.string() << "\" "
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
