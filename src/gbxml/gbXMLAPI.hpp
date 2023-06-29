/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
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
#else
#  define GBXML_API
#endif

#endif
