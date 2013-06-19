#ifndef __OPENSTUDIO_API_HPP__
#define __OPENSTUDIO_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_lib_EXPORTS
#define OPENSTUDIO_API __declspec(dllexport)
#else
#define OPENSTUDIO_API __declspec(dllimport)
#endif
#else
#define OPENSTUDIO_API
#endif

#endif
