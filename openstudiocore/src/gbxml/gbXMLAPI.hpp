#ifndef __GBXML_API_HPP__
#define __GBXML_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_gbxml_EXPORTS
#define GBXML_API __declspec(dllexport)
#else
#define GBXML_API __declspec(dllimport)
#endif
#else
#define GBXML_API
#endif

#endif
