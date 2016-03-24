#include "FilesystemHelpers.hpp"

namespace openstudio {
  namespace filesystem {
    QByteArray read_as_QByteArray(openstudio::filesystem::ifstream &t_file)
    {
      throw std::string("Not Implemented");
    }

    QByteArray read_as_QByteArray(const openstudio::path &t_path)
    {
      throw std::string("Not Implemented");
    }

    std::string read_as_string(openstudio::filesystem::ifstream &t_file)
    {
      throw std::string("Not Implemented");
    }

    std::string read_as_string(const openstudio::path &t_path)
    {
      throw std::string("Not Implemented");
    }

    std::vector<openstudio::path> directory_files(const openstudio::path &t_path)
    {
      throw std::string("Not Implemented");
    }

    std::vector<openstudio::path> recursive_directory_files(const openstudio::path &t_path)
    {
      throw std::string("Not Implemented");
    }

    bool copy_file_no_throw(const openstudio::path &t_src, const openstudio::path &t_dest)
    {
      throw std::string("Not Implemented");
    }

    void write(openstudio::filesystem::ofstream &t_file, const QString &)
    {
      throw std::string("Not Implemented");
    }

    time_t create_time_as_time_t(const openstudio::path &t_path)
    {
      throw std::string("Not Implemented");
    }

    time_t last_write_time_as_time_t(const openstudio::path &t_path)
    {
      throw std::string("Not Implemented");
    }
  }
}
