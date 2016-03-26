#include "FilesystemHelpers.hpp"
#include "Path.hpp"
#include "String.hpp"
#include "Assert.hpp"

namespace openstudio {
  namespace filesystem {
    QByteArray read_as_QByteArray(openstudio::filesystem::ifstream &t_file)
    {
      const auto buf = read_as_vector(t_file);
      return QByteArray(&buf.front(), buf.size());
    }

    QByteArray read_as_QByteArray(const openstudio::path &t_path)
    {
      openstudio::filesystem::ifstream f(t_path, std::ios_base::binary);
      return read_as_QByteArray(f);
    }

    std::vector<char> read_as_vector(openstudio::filesystem::ifstream &t_file)
    {
      OS_ASSERT(t_file.good());
      const auto cur_pos = t_file.tellg();
      t_file.seekg(0, std::ios_base::end);
      const auto end_pos = t_file.tellg();
      const auto file_len = end_pos - cur_pos;
      t_file.seekg(cur_pos);
      std::vector<char> buf(file_len);
      t_file.read(&buf.front(), file_len);
      return buf;
    }

    std::string read_as_string(openstudio::filesystem::ifstream &t_file)
    {
      const auto vec = read_as_vector(t_file);
      return std::string(vec.begin(), vec.end());
    }

    std::string read_as_string(const openstudio::path &t_path)
    {
      openstudio::filesystem::ifstream f(t_path);
      return read_as_string(f);
    }

    template<typename Itr, typename D>
      Itr advance_itr_copy(Itr i, D distance)
      {
        std::advance(i, distance);
        return i;
      }

    template<typename Itr>
      openstudio::path rebuild_path(Itr begin, const Itr &end)
      {
        openstudio::path result;
        while (begin != end) {
          result /= *begin;
          ++begin;
        }
        return result;
      }

    std::vector<openstudio::path> directory_files(const openstudio::path &t_path)
    {
      std::vector<openstudio::path> files;

      const auto num_path_elems = [&](){
        const auto distance = std::distance(t_path.begin(), t_path.end());
        if (openstudio::toString(t_path.filename()) == ".") {
          return distance - 1;
        } else {
          return distance;
        }
      }();

      for (auto dir_itr = openstudio::filesystem::directory_iterator(t_path);
           dir_itr != openstudio::filesystem::directory_iterator();
           ++dir_itr)
      {
        // skip the path so these names are all relative
        const auto p = dir_itr->path();
        if (openstudio::filesystem::is_regular_file(p)) {
          files.push_back(rebuild_path(advance_itr_copy(p.begin(), num_path_elems), p.end()));
          LOG_FREE(Debug, "FilesystemHelpers", "directory_file '" << openstudio::toString(p) << "' -> '" << openstudio::toString(files.back()) << "'");
        }
      }

      return files;
    }

    std::vector<openstudio::path> recursive_directory_files(const openstudio::path &t_path)
    {
      std::vector<openstudio::path> files;

      const auto num_path_elems = [&](){
        const auto distance = std::distance(t_path.begin(), t_path.end());
        if (openstudio::toString(t_path.filename()) == ".") {
          return distance - 1;
        } else {
          return distance;
        }
      }();

      for (auto dir_itr = openstudio::filesystem::recursive_directory_iterator(t_path);
          dir_itr != openstudio::filesystem::recursive_directory_iterator();
          ++dir_itr)
      {
        // skip the path so these names are all relative
        const auto p = dir_itr->path();
        if (openstudio::filesystem::is_regular_file(p)) {
          files.push_back(rebuild_path(advance_itr_copy(p.begin(), num_path_elems), p.end()));
        }
      }

      return files;
    }

    bool copy_file_no_throw(const openstudio::path &t_src, const openstudio::path &t_dest)
    {
      try {
        openstudio::filesystem::copy_file(t_src, t_dest);
        return true;
      } catch (const std::exception &) {
        return false;
      }
    }

    void write(openstudio::filesystem::ofstream &t_file, const QString &s)
    {
      t_file << openstudio::toString(s);
    }

    time_t to_time_t(time_t t) {
      return t;
    }

    template<typename T>
      time_t to_time_t(const T &t) {
        // for future move from boost to std filesystem, which uses std::chrono
        return T::clock::to_time_t(t);
      }

    time_t last_write_time_as_time_t(const openstudio::path &t_path)
    {
      return to_time_t(openstudio::filesystem::last_write_time(t_path));
    }
  }
}
