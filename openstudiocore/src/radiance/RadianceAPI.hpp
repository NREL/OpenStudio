#ifndef RADIANCE_RADIANCEAPI_HPP
#define RADIANCE_RADIANCEAPI_HPP

#if _WIN32 || _MSC_VER

#ifdef openstudio_radiance_EXPORTS
#define RADIANCE_API __declspec(dllexport)
#else
#define RADIANCE_API __declspec(dllimport)
#endif
#else
#define RADIANCE_API
#endif

#endif
