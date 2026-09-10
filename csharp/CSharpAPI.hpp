/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef CSHARPAPI_HPP
#define CSHARPAPI_HPP

#if (_WIN32 || _MSC_VER)
#  ifdef openstudio_csharp_EXPORTS
#    define CSHARP_API __declspec(dllexport)
#  else
#    define CSHARP_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) || defined(__clang__)
#  ifdef openstudio_csharp_EXPORTS
#    define CSHARP_API __attribute__((visibility("default")))
#  else
#    define CSHARP_API
#  endif
#else
#  define CSHARP_API
#endif

#endif
