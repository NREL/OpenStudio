/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_FILESYSTEMHELPERS_HPP
#define UTILITIES_CORE_FILESYSTEMHELPERS_HPP

#include "Filesystem.hpp"

namespace openstudio {
namespace filesystem {
  /// reads entire file from the current read position until the end of file
  /// and returns as std::vector<char>
  UTILITIES_API std::vector<char> read(openstudio::filesystem::ifstream& t_file);

  /// reads entire file from the current read position until the end of file
  /// and returns as std::vector<char>
  UTILITIES_API std::vector<char> read(const openstudio::path& t_path);

  /// reads entire file from the current read position until the end of file
  /// and returns as std::string
  UTILITIES_API std::string read_as_string(openstudio::filesystem::ifstream& t_file);

  /// reads entire file from the current read position until the end of file
  /// and returns as std::string
  UTILITIES_API std::string read_as_string(const openstudio::path& t_path);

  /// Returns all file paths (non recursively) in the directory t_path.
  /// Result values are relative to t_path
  UTILITIES_API std::vector<openstudio::path> directory_files(const openstudio::path& t_path);

  /// Returns all dir paths (non recursively) in the directory t_path.
  /// Result values are relative to t_path
  UTILITIES_API std::vector<openstudio::path> directory_directories(const openstudio::path& t_path);

  /// Returns all file paths (recursively) in the directory t_path.
  /// Result values are relative to t_path
  UTILITIES_API std::vector<openstudio::path> recursive_directory_files(const openstudio::path& t_path);

  /// Copies file from src to dest. If there is an error, false is returned
  UTILITIES_API bool copy_file_no_throw(const openstudio::path& t_src, const openstudio::path& t_dest);

  UTILITIES_API time_t last_write_time_as_time_t(const openstudio::path& t_path);

  /// Creates a new temporary directory in the form of
  /// "/systemtemppath/{base_name}-id"
  /// where `id` is unique identifier determined by the runtime.
  /// \returns path created or empty path if creation failed
  UTILITIES_API openstudio::path create_temporary_directory(const openstudio::path& basename);

  /// Returns the current user's home path
  UTILITIES_API openstudio::path home_path();

}  // namespace filesystem
}  // namespace openstudio

#endif
