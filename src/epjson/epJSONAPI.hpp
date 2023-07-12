/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef EPJSON_EPJSONAPI_HPP
#define EPJSON_EPJSONAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_epjson_EXPORTS) || defined(openstudio_EXPORTS)
#    define EPJSON_API __declspec(dllexport)
#  else
#    define EPJSON_API __declspec(dllimport)
#  endif
#else
#  define EPJSON_API
#endif

#endif
