#ifndef OPENSTUDIO_RUNMANAGER_API_HPP__
#define OPENSTUDIO_RUNMANAGER_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_runmanager_EXPORTS
#define RUNMANAGER_API __declspec(dllexport)
#else
#define RUNMANAGER_API __declspec(dllimport)
#endif

#else
#define RUNMANAGER_API

#endif

#endif

