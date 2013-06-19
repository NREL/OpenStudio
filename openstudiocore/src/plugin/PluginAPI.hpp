#ifndef __OPENSTUDIO_PLUGIN_API_HPP__
#define __OPENSTUDIO_PLUGIN_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_plugin_EXPORTS
#define PLUGIN_API __declspec(dllexport)
#else
#define PLUGIN_API __declspec(dllimport)
#endif
#else
#define PLUGIN_API
#endif

#endif
