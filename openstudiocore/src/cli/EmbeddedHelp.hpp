
#include <iostream>
#include <QCoreApplication>
#include <mach-o/dyld.h> /* _NSGetExecutablePath */
#include <limits.h> /* PATH_MAX */

extern "C" {
  void Init_generator(void);
  void Init_parser(void);
}

namespace embedded_help { 
  inline void initJSONParser() {
    Init_parser();
  }

  inline void initJSONGenerator() {
    Init_generator();
  }

  inline std::string applicationFilePath() {
    char path[PATH_MAX + 1];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0)
        return std::string(path);
    else
        return std::string();
  }
}
