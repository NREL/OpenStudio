/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_FILESYSTEM_HPP
#define UTILITIES_CORE_FILESYSTEM_HPP

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <vector>

#include "../UtilitiesAPI.hpp"

namespace openstudio {
using path = boost::filesystem::path;

namespace filesystem {

  // types
  using boost::filesystem::path;
  using boost::filesystem::wpath;

  using boost::filesystem::basic_ofstream;

  using boost::filesystem::ofstream;
  using boost::filesystem::ifstream;

  using boost::filesystem::recursive_directory_iterator;
  using boost::filesystem::filesystem_error;
  using boost::filesystem::copy_options;
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
