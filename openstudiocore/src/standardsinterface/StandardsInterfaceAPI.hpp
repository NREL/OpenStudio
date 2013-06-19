#ifndef __STANDARDSINTERFACE_API_HPP__
#define __STANDARDSINTERFACE_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_standardsinterface_EXPORTS
#define STANDARDSINTERFACE_API __declspec(dllexport)
#else
#define STANDARDSINTERFACE_API __declspec(dllimport)
#endif
#else
#define STANDARDSINTERFACE_API
#endif

#endif
