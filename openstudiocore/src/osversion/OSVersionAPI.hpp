#ifndef OSVERSION_OSVERSIONAPI_HPP
#define OSVERSION_OSVERSIONAPI_HPP

#if _WIN32 || _MSC_VER

#ifdef openstudio_osversion_EXPORTS
#define OSVERSION_API __declspec(dllexport)
#else
#define OSVERSION_API __declspec(dllimport)
#endif
#else
#define OSVERSION_API
#endif

#endif
