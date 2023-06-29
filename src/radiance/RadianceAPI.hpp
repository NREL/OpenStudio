/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RADIANCE_RADIANCEAPI_HPP
#define RADIANCE_RADIANCEAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_radiance_EXPORTS) || defined(openstudio_EXPORTS)
#    define RADIANCE_API __declspec(dllexport)
#  else
#    define RADIANCE_API __declspec(dllimport)
#  endif
#else
#  define RADIANCE_API
#endif

#endif
