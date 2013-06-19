#ifndef __PAT_API_HPP__
#define __PAT_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef pat_lib_EXPORTS
#define PAT_API __declspec(dllexport)
#else
#define PAT_API __declspec(dllimport)
#endif
#else
#define PAT_API
#endif

#endif
