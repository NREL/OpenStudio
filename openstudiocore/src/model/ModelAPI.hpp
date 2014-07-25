#ifndef MODEL_MODELAPI_HPP
#define MODEL_MODELAPI_HPP

#if _WIN32 || _MSC_VER

#ifdef openstudio_model_EXPORTS
#define MODEL_API __declspec(dllexport)
#else
#define MODEL_API __declspec(dllimport)
#endif
#else
#define MODEL_API
#endif

#endif
