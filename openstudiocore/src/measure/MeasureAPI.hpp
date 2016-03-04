#ifndef MEASURE_MEASUREAPI_HPP
#define MEASURE_MEASUREAPI_HPP

#if _WIN32 || _MSC_VER

#ifdef openstudio_measure_EXPORTS
#define MEASURE_API __declspec(dllexport)
#define MEASURE_TEMPLATE_EXT 
#else
#define MEASURE_API __declspec(dllimport)
#define MEASURE_TEMPLATE_EXT extern
#endif
#else
#define MEASURE_API
#endif

#endif
