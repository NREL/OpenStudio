#ifndef ANALYSISDRIVER_ANALYSISDRIVERAPI_HPP
#define ANALYSISDRIVER_ANALYSISDRIVERAPI_HPP

#if _WIN32 || _MSC_VER

#ifdef openstudio_analysisdriver_EXPORTS
#define ANALYSISDRIVER_API __declspec(dllexport)
#else
#define ANALYSISDRIVER_API __declspec(dllimport)
#endif
#else
#define ANALYSISDRIVER_API
#endif

#endif
