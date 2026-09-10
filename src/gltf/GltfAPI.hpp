/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef GLTF_GLTFAPI_HPP
#define GLTF_GLTFAPI_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_gltf_EXPORTS) || defined(openstudio_EXPORTS)
#    define GLTF_API __declspec(dllexport)
#  else
#    define GLTF_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) || defined(__clang__)
#  if defined(openstudio_gltf_EXPORTS) || defined(openstudio_EXPORTS)
#    define GLTF_API __attribute__((visibility("default")))
#  else
#    define GLTF_API
#  endif
#else
#  define GLTF_API
#endif

#endif
