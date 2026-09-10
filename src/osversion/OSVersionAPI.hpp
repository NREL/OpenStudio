/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef OSVERSION_OSVERSIONAPI_HPP
#define OSVERSION_OSVERSIONAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_osversion_EXPORTS) || defined(openstudio_EXPORTS)
#    define OSVERSION_API __declspec(dllexport)
#  else
#    define OSVERSION_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) || defined(__clang__)
#  if defined(openstudio_osversion_EXPORTS) || defined(openstudio_EXPORTS)
#    define OSVERSION_API __attribute__((visibility("default")))
#  else
#    define OSVERSION_API
#  endif
#else
#  define OSVERSION_API
#endif

#endif
