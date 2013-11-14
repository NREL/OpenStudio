#ifndef __ISOMODEL_API_HPP__
#define __ISOMODEL_API_HPP__

#if _WIN32 || _MSC_VER

#ifdef openstudio_isomodel_EXPORTS
#define ISOMODEL_API __declspec(dllexport)
#else
#define ISOMODEL_API __declspec(dllimport)
#endif
#else
#define ISOMODEL_API
#endif

#endif
