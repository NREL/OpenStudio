#include <cstdint>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

#ifdef _MSC_VER
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif

#include <memory>
#include <vector>

namespace embedded_files { 
  std::map<std::string, std::pair<size_t, const uint8_t *> > files();

  inline bool hasFile(const std::string &t_filename) {
    const auto fs = files();
    return fs.find(t_filename) != fs.end();
  }

  inline std::string getFileAsString(const std::string &t_filename) {
    const auto fs = files();
    const auto f = fs.at(t_filename);
    return std::string(f.second, f.second + f.first);
  }

  inline void extractFile(const std::string &t_filename, const std::string &t_location)
  {
    const auto fs = files();


    const auto create_dirs = [](const std::string &t_path) {
      const auto paths = [](const std::string &t_paths) {
        std::vector<std::string> subpaths;
        const auto handle_subpath = [&subpaths](std::string &t_subpath) {
          if (!t_subpath.empty()) {
            subpaths.push_back(t_subpath);
            t_subpath.clear();
          }
        };

        std::string curstring = "";
        for (const auto c : t_paths) {
          if (c != '/') {
            curstring.push_back(c);
          } else {
            handle_subpath(curstring);
          }
        }
        handle_subpath(curstring);
        return subpaths;
      }(t_path);


      std::string curpath = "";

      for (const auto &p : paths) {
        if (!curpath.empty()) curpath += '/';
        curpath += p;
#ifdef _MSC_VER
        _mkdir(curpath.c_str());
#else
        mkdir(curpath.c_str(), 0777);
#endif
      }
    };

    auto f = fs.at(t_filename);
    const auto fullpath = t_location + '/' + t_filename;
    create_dirs(std::string(std::begin(fullpath), std::begin(fullpath) + fullpath.rfind('/')));
    std::ofstream ofs(fullpath);
    ofs.write(reinterpret_cast<const char *>(f.second), f.first);
    std::cout << "***** Extracted " << t_filename << " to: " << fullpath << " *****\n";
  }

}
