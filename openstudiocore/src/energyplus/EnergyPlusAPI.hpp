#ifndef ENERGYPLUS_ENERGYPLUSAPI_HPP
#define ENERGYPLUS_ENERGYPLUSAPI_HPP

#if _WIN32 || _MSC_VER

#ifdef openstudio_energyplus_EXPORTS
#define ENERGYPLUS_API __declspec(dllexport)
#else
#define ENERGYPLUS_API __declspec(dllimport)
#endif
#else
#define ENERGYPLUS_API
#endif

#endif
