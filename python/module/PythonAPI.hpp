#ifndef PYTHONAPI_HPP
#define PYTHONAPI_HPP

#if (_WIN32 || _MSC_VER)
#  ifdef openstudio_py_EXPORTS
#    define PYTHON_API __declspec(dllexport)
#  endif
#else
#  define PYTHON_API
#endif

#endif
