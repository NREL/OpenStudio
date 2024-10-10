/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ALFALFA_ALFALFAAPI_HPP
#define ALFALFA_ALFALFAAPI_HPP

#if (_WIN32 || _MSC_VER) && !OPENSTUDIO_DIRECT_INCLUDE

#  ifdef SHARED_OS_LIBS

#    if defined(openstudio_utilities_EXPORTS) || defined(openstudio_EXPORTS)
#      define ALFALFA_API __declspec(dllexport)
#      define ALFALFA_TEMPLATE_EXT
#    else
#      define ALFALFA_API __declspec(dllimport)
#      define ALFALFA_TEMPLATE_EXT extern
#    endif

#  else

#    define ALFALFA_API
#    if defined(openstudio_alfalfa_EXPORTS) || defined(openstudio_EXPORTS)
#      define ALFALFA_TEMPLATE_EXT
#    else
#      define ALFALFA_TEMPLATE_EXT extern
#    endif

#  endif

#else

#  define ALFALFA_API
#  define ALFALFA_TEMPLATE_EXT

#endif

#endif
