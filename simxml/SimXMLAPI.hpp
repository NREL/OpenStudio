#ifndef __SIMXML_API_HPP__
#define __SIMXML_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_simxml_EXPORTS
#define SIMXML_API __declspec(dllexport)
#else
#define SIMXML_API __declspec(dllimport)
#endif
#else
#define SIMXML_API
#endif

#endif
