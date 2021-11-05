/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2021, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef UTILITIES_CORE_FILESYSTEM_HPP
#define UTILITIES_CORE_FILESYSTEM_HPP

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <vector>

#include "../UtilitiesAPI.hpp"

namespace openstudio {
typedef boost::filesystem::path path;

namespace filesystem {

  // types
  using boost::filesystem::path;
  using boost::filesystem::wpath;

  using boost::filesystem::basic_ofstream;

  using boost::filesystem::ofstream;
  using boost::filesystem::ifstream;

  using boost::filesystem::recursive_directory_iterator;
  using boost::filesystem::filesystem_error;
  using boost::filesystem::copy_option;
  using boost::filesystem::directory_entry;

  // functions
  using boost::filesystem::basename;
  using boost::filesystem::canonical;
  using boost::filesystem::complete;
  using boost::filesystem::copy;
  using boost::filesystem::copy_file;
  using boost::filesystem::create_directories;
  using boost::filesystem::create_directory;
  using boost::filesystem::directory_iterator;
  using boost::filesystem::equivalent;
  using boost::filesystem::exists;
  using boost::filesystem::extension;
  using boost::filesystem::file_size;
  using boost::filesystem::initial_path;
  using boost::filesystem::is_directory;
  using boost::filesystem::is_empty;
  using boost::filesystem::is_regular_file;
  using boost::filesystem::is_symlink;
  using boost::filesystem::last_write_time;
  using boost::filesystem::relative;
  using boost::filesystem::remove;
  using boost::filesystem::remove_all;
  using boost::filesystem::file_size;
  using boost::filesystem::system_complete;
  using boost::filesystem::temp_directory_path;
  using boost::filesystem::read_symlink;
  using boost::filesystem::weakly_canonical;

}  // namespace filesystem
}  // namespace openstudio

#endif
