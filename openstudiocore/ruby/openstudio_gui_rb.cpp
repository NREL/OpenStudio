#include "RubyGuiAPI.hpp"
#include "../src/utilities/core/Path.hpp"
#include <ruby.h>
#include <string>
#include <functional>
#include <iostream>

#if defined _WIN32 
#include <windows.h>
#else
#include <stdio.h>
#include <dlfcn.h>
#endif

inline std::string libraryPath() {
#if defined _WIN32 
  TCHAR szPath[MAX_PATH];
  if( !GetModuleFileName( nullptr, szPath, MAX_PATH ) ) {
    return std::string(szPath);
  } else {
      return std::string();
  }
#else
  Dl_info info;
  if (dladdr(reinterpret_cast<const void *>(&libraryPath), &info)) {
     return std::string(info.dli_fname);
  } else {
    return std::string();
  }
#endif
}

extern "C" {

void Init_openstudiomodeleditor();

RUBY_GUI_API void Init_openstudio_gui(void) {
  // expect openstudio_gui binding library to be next to openstudio library
  openstudio::path t_path(libraryPath());
  rb_require((t_path.parent_path() / "openstudio").string().c_str()); 

  Init_openstudiomodeleditor();
}

}

