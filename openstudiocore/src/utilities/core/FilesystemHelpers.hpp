#ifndef UTILITIES_CORE_FILESYSTEMHELPERS_HPP
#define UTILITIES_CORE_FILESYSTEMHELPERS_HPP

#include "Filesystem.hpp"
#include <QByteArray>

namespace openstudio {
  namespace filesystem {
    /// reads entire file from the current read position until the end of file
    /// and returns as std::vector<char>
    UTILITIES_API std::vector<char> read(openstudio::filesystem::ifstream &t_path);

    /// reads entire file from the current read position until the end of file
    /// and returns as QByteArray
    UTILITIES_API QByteArray read_as_QByteArray(openstudio::filesystem::ifstream &t_file);

    /// reads entire file from the current read position until the end of file
    /// and returns as QByteArray
    UTILITIES_API QByteArray read_as_QByteArray(const openstudio::path &t_path);

    /// reads entire file from the current read position until the end of file
    /// and returns as std::string
    UTILITIES_API std::string read_as_string(openstudio::filesystem::ifstream &t_file);

    /// reads entire file from the current read position until the end of file
    /// and returns as std::string
    UTILITIES_API std::string read_as_string(const openstudio::path &t_path);

    /// Returns all file paths (non recursively) in the directory t_path. 
    /// Result values are relative to t_path
    UTILITIES_API std::vector<openstudio::path> directory_files(const openstudio::path &t_path);

    /// Returns all file paths (recursively) in the directory t_path. 
    /// Result values are relative to t_path
    UTILITIES_API std::vector<openstudio::path> recursive_directory_files(const openstudio::path &t_path);

    /// Copies file from src to dest. If there is an error, false is returned
    UTILITIES_API bool copy_file_no_throw(const openstudio::path &t_src, const openstudio::path &t_dest);

    // consider making this a operator<< overload. Did it this way for now to keep the changes straight
    UTILITIES_API void write(openstudio::filesystem::ofstream &t_file, const QString &);

    UTILITIES_API time_t last_write_time_as_time_t(const openstudio::path &t_path);
  }
}

#endif

