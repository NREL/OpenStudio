#ifndef MEASURE_MEASUREAPI_HPP
#define MEASURE_MEASUREAPI_HPP

  #if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
    #ifdef openstudio_measure_EXPORTS
      #define MEASURE_API __declspec(dllexport)
    #else
      #define MEASURE_API __declspec(dllimport)
    #endif
  #else
    #define MEASURE_API
  #endif
  
#endif
