#ifndef UTILITIES_CORE_FILESYSTEMHELPERS_HPP
#define UTILITIES_CORE_FILESYSTEMHELPERS_HPP

#include "Filesystem.hpp"
#include <QByteArray>

namespace openstudio {
  namespace filesystem {
    UTILITIES_API QByteArray read_as_QByteArray(openstudio::filesystem::ifstream &t_file);
    UTILITIES_API QByteArray read_as_QByteArray(const openstudio::path &t_path);
    UTILITIES_API std::string read_as_string(openstudio::filesystem::ifstream &t_file);
    UTILITIES_API std::string read_as_string(const openstudio::path &t_path);
    UTILITIES_API std::vector<openstudio::path> directory_files(const openstudio::path &t_path);
    UTILITIES_API std::vector<openstudio::path> recursive_directory_files(const openstudio::path &t_path);
    UTILITIES_API bool copy_file_no_throw(const openstudio::path &t_src, const openstudio::path &t_dest);
    UTILITIES_API void write(openstudio::filesystem::ofstream &t_file, const QString &);
    UTILITIES_API time_t create_time_as_time_t(const openstudio::path &t_path);
    UTILITIES_API time_t last_write_time_as_time_t(const openstudio::path &t_path);
  }
}

#endif

