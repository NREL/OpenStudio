#ifndef RUNMANAGER_LIB_RUNMANAGERAPI_HPP
#define RUNMANAGER_LIB_RUNMANAGERAPI_HPP

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

