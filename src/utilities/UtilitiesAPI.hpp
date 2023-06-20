/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_UTILITIESAPI_HPP
#define UTILITIES_UTILITIESAPI_HPP

#if (_WIN32 || _MSC_VER) && !OPENSTUDIO_DIRECT_INCLUDE

#  ifdef SHARED_OS_LIBS

#    if defined(openstudio_utilities_EXPORTS) || defined(openstudio_EXPORTS)
#      define UTILITIES_API __declspec(dllexport)
#      define UTILITIES_TEMPLATE_EXT
#    else
#      define UTILITIES_API __declspec(dllimport)
#      define UTILITIES_TEMPLATE_EXT extern
#    endif

#  else

#    define UTILITIES_API
#    if defined(openstudio_utilities_EXPORTS) || defined(openstudio_EXPORTS)
#      define UTILITIES_TEMPLATE_EXT
#    else
#      define UTILITIES_TEMPLATE_EXT extern
#    endif

#  endif

#else

#  define UTILITIES_API
#  define UTILITIES_TEMPLATE_EXT

#endif

#endif
