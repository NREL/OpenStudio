#ifndef __MODEL_API_HPP__
#define __MODEL_API_HPP__

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
