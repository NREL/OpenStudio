#ifndef UTILITIES_CORE_EXCEPTION_I
#define UTILITIES_CORE_EXCEPTION_I


%include <std_except.i>

%{
  #include <utilities/core/Exception.hpp>

  /// test method
  void throwException(){throw std::runtime_error(std::string("test exception"));}

%}

#if defined(SWIGRUBY)
  %exceptionclass openstudio::Exception;

  %exception {
    try {
      $action
    } catch (const std::exception& e) {
      rb_raise(rb_eRuntimeError, "%s", e.what());
    }
  }

#elif defined(SWIGPYTHON)

  %exception {
    try {
      $action
    } catch (const std::exception &e) {
      PyErr_SetString(PyExc_RuntimeError, e.what());
    }
  }

#elif defined(SWIGCSHARP)

  // Help in overload selection to avoid warning
  %ignore openstudio::Exception::Exception(char const*);

  %exception {
    try {
      $action
    } catch (const std::exception &e) {
      SWIG_CSharpSetPendingException(SWIG_CSharpApplicationException, e.what());
    }
  }
#elif defined(SWIGJAVA)
  %exception {
    try {
      $action
    } catch (const std::exception &e) {
      SWIG_exception(SWIG_RuntimeError, const_cast<char*>(e.what()));
    }
  }

#elif defined(SWIGJAVASCRIPT)

  %exception {
    try {
      $action
    } catch (const std::exception &e) {
      return v8::ThrowException(v8::String::New(e.what()));
    }
  }

#else
  #warning Exception handler not available for current target language
#endif

/// test method
void throwException();

%include <utilities/core/Exception.hpp>

#endif //UTILITIES_CORE_EXCEPTION_I
