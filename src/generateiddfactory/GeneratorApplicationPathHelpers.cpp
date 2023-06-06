/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <sstream>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <iostream>

#ifdef _WIN32
#  include <Windows.h>
#else
#  include <dlfcn.h>
#endif

namespace openstudio {

using path = boost::filesystem::path;

openstudio::path getApplicationOutputDirectory(const std::string& outdir) {
  openstudio::path result(outdir);
  std::stringstream ss;
  std::stringstream tss;

  // turn relative path into absolute path
  result = boost::filesystem::system_complete(result);
  std::string dirStr = result.string();
  // create string used in error messages
  if (dirStr != outdir) {
    tss << " (" << dirStr << ")";
  }

  // result cannot be a file
  if (boost::filesystem::is_regular_file(result)) {
    ss << "Invalid outdir, " << outdir << tss.str() << ", is a file instead of a directory with a valid parent_path.";
    throw std::runtime_error(ss.str().c_str());
  }

  // if result is not yet a directory
  if (!boost::filesystem::is_directory(result)) {
    // parent path must be an existing directory
    if (!boost::filesystem::is_directory(result.parent_path())) {
      ss << "Invalid outdir, " << outdir << tss.str() << ", has a parent_path, " << result.parent_path().string()
         << ", that is either invalid, or corresponds to a non-existent directory.";
      throw std::runtime_error(ss.str().c_str());
    }
    // make the output directory, if possible
    std::cout << "Creating output directory " << dirStr << "." << '\n';
    bool ok = boost::filesystem::create_directory(result);
    if (!ok) {
      ss << "Unable to create outdir " << outdir << tss.str() << ".";
      throw std::runtime_error(ss.str().c_str());
    }
  }

  std::cout << '\n' << "All output will be directed to " << dirStr << "." << '\n' << '\n';
  return result;
}

}  // namespace openstudio
