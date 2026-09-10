/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef SDD_SDDAPI_HPP
#define SDD_SDDAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_sdd_EXPORTS) || defined(openstudio_EXPORTS)
#    define SDD_API __declspec(dllexport)
#  else
#    define SDD_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) || defined(__clang__)
#  if defined(openstudio_sdd_EXPORTS) || defined(openstudio_EXPORTS)
#    define SDD_API __attribute__((visibility("default")))
#  else
#    define SDD_API
#  endif
#else
#  define SDD_API
#endif

#endif
