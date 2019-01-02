/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "FilesystemHelpers.hpp"
#include "Path.hpp"
#include "Assert.hpp"

namespace openstudio {
  namespace filesystem {
    std::vector<char> read(openstudio::filesystem::ifstream &t_file)
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

    QByteArray read_as_QByteArray(openstudio::filesystem::ifstream &t_file)
    {
      const auto buf = read(t_file);
      return QByteArray(&buf.front(), buf.size());
    }

    QByteArray read_as_QByteArray(const openstudio::path &t_path)
    {
      openstudio::filesystem::ifstream f(t_path, std::ios_base::binary);
      return read_as_QByteArray(f);
    }

    std::string read_as_string(openstudio::filesystem::ifstream &t_file)
    {
      const auto vec = read(t_file);
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
