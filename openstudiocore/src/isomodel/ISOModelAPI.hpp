#ifndef ISOMODEL_ISOMODELAPI_HPP
#define ISOMODEL_ISOMODELAPI_HPP

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
