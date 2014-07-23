#ifndef PATAPP_PATAPI_HPP
#define PATAPP_PATAPI_HPP

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
