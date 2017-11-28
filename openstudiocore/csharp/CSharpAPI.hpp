#ifndef CSHARPAPI_HPP
#define CSHARPAPI_HPP

  #if (_WIN32 || _MSC_VER)
    #ifdef openstudio_csharp_EXPORTS
      #define CSHARP_API __declspec(dllexport)
    #else
      #define CSHARP_API __declspec(dllimport)
    #endif
  #else
    #define CSHARP_API
  #endif

#endif
