#ifndef RUBYGUIAPI_HPP
#define RUBYGUIAPI_HPP

  #if (_WIN32 || _MSC_VER)
    #ifdef openstudio_gui_rb_EXPORTS
      #define RUBY_GUI_API __declspec(dllexport)
    #else
      #define RUBY_GUI_API __declspec(dllimport)
    #endif
  #else
    #define RUBY_GUI_API
  #endif

#endif
