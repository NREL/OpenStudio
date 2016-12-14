#ifndef BIMSERVER_BIMSERVERAPI_HPP
#define BIMSERVER_BIMSERVERAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS

#ifdef openstudio_bimserver_EXPORTS
#define BIMSERVER_API __declspec(dllexport)
#else
#define BIMSERVER_API __declspec(dllimport)
#endif
#else
#define BIMSERVER_API
#endif

#endif
