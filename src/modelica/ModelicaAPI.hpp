#ifndef MODELICA_API_HPP
#define MODELICA_API_HPP

#if (_WIN32 || _MSC_VER) && SHARED_OS_LIBS
#  if defined(openstudio_modelica_EXPORTS) || defined(openstudio_EXPORTS)
#    define MODELICA_API __declspec(dllexport)
#  else
#    define MODELICA_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) || defined(__clang__)
#  if defined(openstudio_modelica_EXPORTS) || defined(openstudio_EXPORTS)
#    define MODELICA_API __attribute__((visibility("default")))
#  else
#    define MODELICA_API
#  endif
#else
#  define MODELICA_API
#endif

#endif  // MODELICA_API_HPP
