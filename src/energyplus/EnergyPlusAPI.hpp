/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef ENERGYPLUS_ENERGYPLUSAPI_HPP
#define ENERGYPLUS_ENERGYPLUSAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_energyplus_EXPORTS) || defined(openstudio_EXPORTS)
#    define ENERGYPLUS_API __declspec(dllexport)
#  else
#    define ENERGYPLUS_API __declspec(dllimport)
#  endif
#else
#  define ENERGYPLUS_API
#endif

#endif
