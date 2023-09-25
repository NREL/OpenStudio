/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef SCRIPTENGINE_SCRIPTENGINEAPI_HPP
#define SCRIPTENGINE_SCRIPTENGINEAPI_HPP

#if SHARED_OS_LIBS
#  if (_WIN32 || _MSC_VER)
#    if defined(openstudio_scriptengine_EXPORTS) || defined(openstudio_EXPORTS)
#      define SCRIPTENGINE_API __declspec(dllexport)
#    else
#      define SCRIPTENGINE_API __declspec(dllimport)
#    endif
#  else
#    if defined(openstudio_scriptengine_EXPORTS) || defined(openstudio_EXPORTS)
#      define SCRIPTENGINE_API __attribute__((visibility("default")))
#    else
#      define SCRIPTENGINE_API
#    endif
#  endif
#else
#  define SCRIPTENGINE_API
#  error "This shouldn't happen. ScriptEngine is always supposed to be used in as a Shared Library"
#endif

#endif  // SCRIPTENGINE_SCRIPTENGINEAPI_HPP
