/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ALFALFA_ALFALFAAPI_HPP
#define ALFALFA_ALFALFAAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_alfalfa_EXPORTS) || defined(openstudio_EXPORTS)
#    define ALFALFA_API __declspec(dllexport)
#  else
#    define ALFALFA_API __declspec(dllimport)
#  endif
#else
#  define ALFALFA_API
#endif

#endif
