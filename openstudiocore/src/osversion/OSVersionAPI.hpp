#ifndef __OSVERSION_API_HPP__
#define __OSVERSION_API_HPP__

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
