/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "Filesystem.hpp"
#include "Path.hpp"
#include "Assert.hpp"

namespace openstudio {
namespace filesystem {
  std::vector<char> read(openstudio::filesystem::ifstream& t_file) {
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

  std::vector<char> read(const openstudio::path& t_path) {
    openstudio::filesystem::ifstream f(t_path, std::ios_base::binary);
    return read(f);
  }

  std::string read_as_string(openstudio::filesystem::ifstream& t_file) {
    const auto vec = read(t_file);
    return std::string(vec.begin(), vec.end());
  }

  std::string read_as_string(const openstudio::path& t_path) {
    openstudio::filesystem::ifstream f(t_path);
    return read_as_string(f);
  }

  template <typename Itr, typename D>
  Itr advance_itr_copy(Itr i, D distance) {
    std::advance(i, distance);
    return i;
  }

  template <typename Itr>
  openstudio::path rebuild_path(Itr begin, const Itr& end) {
    openstudio::path result;
    while (begin != end) {
      result /= *begin;
      ++begin;
    }
    return result;
  }

  template <typename Predicate>
  std::vector<openstudio::path> directory_elements(const openstudio::path& t_path, Predicate predicate) {
    std::vector<openstudio::path> files;

    const auto num_path_elems = [&]() {
      const auto distance = std::distance(t_path.begin(), t_path.end());
      if (openstudio::toString(t_path.filename()) == ".") {
        return distance - 1;
      } else {
        return distance;
      }
    }();

    for (auto dir_itr = openstudio::filesystem::directory_iterator(t_path); dir_itr != openstudio::filesystem::directory_iterator(); ++dir_itr) {
      // skip the path so these names are all relative
      const auto p = dir_itr->path();
      if (predicate(p)) {
        files.push_back(rebuild_path(advance_itr_copy(p.begin(), num_path_elems), p.end()));
        LOG_FREE(Debug, "FilesystemHelpers",
                 "directory_elements '" << openstudio::toString(p) << "' -> '" << openstudio::toString(files.back()) << "'");
      }
    }

    return files;
  }

  std::vector<openstudio::path> directory_files(const openstudio::path& t_path) {
    return directory_elements(t_path, [](const auto& f) { return openstudio::filesystem::is_regular_file(f); });
  }

  std::vector<openstudio::path> directory_directories(const openstudio::path& t_path) {
    return directory_elements(t_path, [](const auto& f) { return openstudio::filesystem::is_directory(f); });
  }

  std::vector<openstudio::path> recursive_directory_files(const openstudio::path& t_path) {
    std::vector<openstudio::path> files;

    const auto num_path_elems = [&]() {
      const auto distance = std::distance(t_path.begin(), t_path.end());
      if (openstudio::toString(t_path.filename()) == ".") {
        return distance - 1;
      } else {
        return distance;
      }
    }();

    for (auto dir_itr = openstudio::filesystem::recursive_directory_iterator(t_path);
         dir_itr != openstudio::filesystem::recursive_directory_iterator(); ++dir_itr) {
      // skip the path so these names are all relative
      const auto p = dir_itr->path();
      if (openstudio::filesystem::is_regular_file(p)) {
        files.push_back(rebuild_path(advance_itr_copy(p.begin(), num_path_elems), p.end()));
      }
    }

    return files;
  }

  bool copy_file_no_throw(const openstudio::path& t_src, const openstudio::path& t_dest) {
    try {
      openstudio::filesystem::copy_file(t_src, t_dest);
      return true;
    } catch (const std::exception&) {
      return false;
    }
  }

  time_t to_time_t(time_t t) {
    return t;
  }

  template <typename T>
  time_t to_time_t(const T& t) {
    // for future move from boost to std filesystem, which uses std::chrono
    return T::clock::to_time_t(t);
  }

  time_t last_write_time_as_time_t(const openstudio::path& t_path) {
    return to_time_t(openstudio::filesystem::last_write_time(t_path));
  }

  openstudio::path create_temporary_directory(const openstudio::path& basename) {
    // making this count static/atomic so that we reduce the chance of collisions
    // on each run of the binary. This is threadsafe, with the atomic
    static std::atomic<unsigned int> count = 0;
    constexpr auto allowed_attempts = 1000;

    const auto temp_dir = openstudio::filesystem::temp_directory_path();

    int attempts{0};

    while (attempts < allowed_attempts) {
      // concat number to path basename, without adding a new path element
      auto filename = basename;
      filename += openstudio::toPath("-" + std::to_string(std::time(nullptr)) + "-" + std::to_string(count++));
      const auto full_pathname = temp_dir / filename;
      // full_path_name = {temp_path}/{base_name}-{count++}

      try {
        if (openstudio::filesystem::create_directories(full_pathname)) {
          // if the path was created, then we know it was created for us
          return full_pathname;
        }
      } catch (...) {
        // swallow exceptions, we don't care, we only care when it succeeds
      }
    }

    // after too many attempts we never made a path
    return {};
  }

  openstudio::path home_path() {
    const auto build_path = [](const auto&... elem) {
      auto path_elem = [](auto& missing, const auto& env_var) {
        if (const auto var = std::getenv(env_var); var != nullptr) {
          const auto path = toPath(var);
          if (path.empty()) {
            missing = true;
          }
          return path;
        } else {
          missing = true;
          return openstudio::path{};
        }
      };

      bool missing_element = false;

      // build up the path from the environment variable names passed in.
      const auto path = (path_elem(missing_element, elem) / ...).lexically_normal();

      if (!missing_element && openstudio::filesystem::is_directory(path)) {
        return path;
      } else {
        return openstudio::path{};
      }
    };

    // TODO: instead of trace, just comment out?
    if (const auto home = build_path("USERPROFILE"); !home.empty()) {
      LOG_FREE(Trace, "FilesystemHelpers", "home_path USERPROFILE: " << toString(home));
      return home;
    }

    if (const auto home = build_path("HOMEDRIVE", "HOMEPATH"); !home.empty()) {
      LOG_FREE(Trace, "FilesystemHelpers", "home_path HOMEDRIVE/HOMEPATH: " << toString(home));
      return home;
    }

    if (const auto home = build_path("HOME"); !home.empty()) {
      LOG_FREE(Trace, "FilesystemHelpers", "home_path HOME: " << toString(home));
      return home;
    }

    LOG_FREE(Warn, "FilesystemHelpers", "home_path No Home Found");
    return toPath("/");
  }
}  // namespace filesystem
}  // namespace openstudio
