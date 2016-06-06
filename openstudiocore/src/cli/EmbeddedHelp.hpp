
#include <iostream>
#if defined __APPLE__
#include <mach-o/dyld.h> /* _NSGetExecutablePath */
#include <limits.h> /* PATH_MAX */
#elif defined _WIN32 
#include <windows.h>
#endif

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
#ifdef __APPLE__
    char path[PATH_MAX + 1];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        return std::string(path);
    } else {
        return std::string();
    }
#elif defined _WIN32 
    TCHAR szPath[MAX_PATH];
    if( !GetModuleFileName( nullptr, szPath, MAX_PATH ) ) {
      return std::string(szPath);
    } else {
        return std::string();
    }
#endif
  }
}
