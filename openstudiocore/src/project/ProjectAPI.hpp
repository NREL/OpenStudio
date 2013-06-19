#ifndef __PROJECT_API_HPP__
#define __PROJECT_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_project_EXPORTS
#define PROJECT_API __declspec(dllexport)
#else
#define PROJECT_API __declspec(dllimport)
#endif
#else
#define PROJECT_API
#endif

#endif
