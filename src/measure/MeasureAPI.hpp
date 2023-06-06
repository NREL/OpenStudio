/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MEASURE_MEASUREAPI_HPP
#define MEASURE_MEASUREAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_measure_EXPORTS) || defined(openstudio_EXPORTS)
#    define MEASURE_API __declspec(dllexport)
#  else
#    define MEASURE_API __declspec(dllimport)
#  endif
#else
#  define MEASURE_API
#endif

#endif
