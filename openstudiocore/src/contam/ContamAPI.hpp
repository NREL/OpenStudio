#ifndef CONTAM_CONTAMAPI_HPP
#define CONTAM_CONTAMAPI_HPP

#if _WIN32 || _MSC_VER

#ifdef openstudio_contam_EXPORTS
#define CONTAM_API __declspec(dllexport)
#else
#define CONTAM_API __declspec(dllimport)
#endif
#else
#define CONTAM_API
#endif

#endif
