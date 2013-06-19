#ifndef __RULESENGINE_API_HPP__
#define __RULESENGINE_API_HPP__

#if _WIN32 || _MSC_VER

  #ifdef openstudio_rulesengine_EXPORTS
    #define RULESENGINE_API __declspec(dllexport)
    #define RULESENGINE_TEMPLATE_EXT
  #else
    #define RULESENGINE_API __declspec(dllimport)
    #define RULESENGINE_TEMPLATE_EXT extern
  #endif

#else

  #define RULESENGINE_API
  #define RULESENGINE_TEMPLATE_EXT

#endif

#endif // __RULESENGINE_API_HPP__
