/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RUBYAPI_HPP
#define RUBYAPI_HPP

#if (_WIN32 || _MSC_VER)
#  ifdef openstudio_rb_EXPORTS
#    define RUBY_API __declspec(dllexport)
#  elif defined openstudio_modeleditor_rb_EXPORTS
#    define RUBY_API __declspec(dllexport)
#  else
#    define RUBY_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) || defined(__clang__)
#  if defined(openstudio_rb_EXPORTS) || defined(openstudio_modeleditor_rb_EXPORTS)
#    define RUBY_API __attribute__((visibility("default")))
#  else
#    define RUBY_API
#  endif
#else
#  define RUBY_API
#endif

#endif
