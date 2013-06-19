#ifndef __EEFG_API_HPP__
#define __EEFG_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_eefg_EXPORTS
#define EEFG_API __declspec(dllexport)
#else
#define EEFG_API __declspec(dllimport)
#endif

#else
#define EEFG_API
#endif

#endif
