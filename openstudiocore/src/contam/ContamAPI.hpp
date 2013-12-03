#ifndef __CONTAM_API_HPP__
#define __CONTAM_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_contam_EXPORTS
#define CONTAM_API __declspec(dllexport)
#else
#define CONTAM_API __declspec(dllimport)
#endif
#else
#define CONTAM_API
#endif

#endif
