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
