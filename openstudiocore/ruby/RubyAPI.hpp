#ifndef RUBYAPI_HPP
#define RUBYAPI_HPP

  #if (_WIN32 || _MSC_VER)
    #ifdef openstudiorb_EXPORTS
      #define RUBY_API __declspec(dllexport)
    #else
      #define RUBY_API __declspec(dllimport)
    #endif
  #else
    #define RUBY_API
  #endif

#endif
