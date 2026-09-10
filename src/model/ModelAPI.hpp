/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MODELAPI_HPP
#define MODEL_MODELAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_model_EXPORTS) || defined(openstudio_EXPORTS)
#    define MODEL_API __declspec(dllexport)
#  else
#    define MODEL_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) || defined(__clang__)
#  if defined(openstudio_model_EXPORTS) || defined(openstudio_EXPORTS)
#    define MODEL_API __attribute__((visibility("default")))
#  else
#    define MODEL_API
#  endif
#else
#  define MODEL_API
#endif

#endif
