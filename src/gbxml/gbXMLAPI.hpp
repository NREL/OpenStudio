/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GBXML_GBXMLAPI_HPP
#define GBXML_GBXMLAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_gbxml_EXPORTS) || defined(openstudio_EXPORTS)
#    define GBXML_API __declspec(dllexport)
#  else
#    define GBXML_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) || defined(__clang__)
#  if defined(openstudio_gbxml_EXPORTS) || defined(openstudio_EXPORTS)
#    define GBXML_API __attribute__((visibility("default")))
#  else
#    define GBXML_API
#  endif
#else
#  define GBXML_API
#endif

#endif
