#ifndef __SDD_API_HPP__
#define __SDD_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_sdd_EXPORTS
#define SDD_API __declspec(dllexport)
#else
#define SDD_API __declspec(dllimport)
#endif
#else
#define SDD_API
#endif

#endif
