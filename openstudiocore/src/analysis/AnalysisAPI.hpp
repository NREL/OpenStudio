#ifndef __ANALYSIS_API_HPP__
#define __ANALYSIS_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_analysis_EXPORTS
#define ANALYSIS_API __declspec(dllexport)
#else
#define ANALYSIS_API __declspec(dllimport)
#endif
#else
#define ANALYSIS_API
#endif

#endif
