#ifndef UTILITIES_UTILITIESAPI_HPP
#define UTILITIES_UTILITIESAPI_HPP

#if (_WIN32 || _MSC_VER) && !OPENSTUDIO_DIRECT_INCLUDE

#ifdef openstudio_utilities_EXPORTS
#define UTILITIES_API __declspec(dllexport)
#define UTILITIES_TEMPLATE_EXT
#else
#define UTILITIES_API __declspec(dllimport)
#define UTILITIES_TEMPLATE_EXT extern
#endif

#else

#define UTILITIES_API
#define UTILITIES_TEMPLATE_EXT 

#endif

#endif
