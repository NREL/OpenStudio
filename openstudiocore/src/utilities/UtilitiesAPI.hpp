#ifndef UTILITIES_UTILITIESAPI_HPP
  #define UTILITIES_UTILITIESAPI_HPP

  #if (_WIN32 || _MSC_VER) && !OPENSTUDIO_DIRECT_INCLUDE

    #define UTILITIES_API
    #ifdef openstudio_utilities_EXPORTS
      #define UTILITIES_TEMPLATE_EXT 
    #else
      #define UTILITIES_TEMPLATE_EXT extern
    #endif

  #else

    #define UTILITIES_API
    #define UTILITIES_TEMPLATE_EXT 

  #endif

#endif
