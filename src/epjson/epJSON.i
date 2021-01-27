#ifndef EPJSON_I
#define EPJSON_I

#ifdef SWIGPYTHON
%module openstudioepjson
#endif


#define UTILITIES_API
#define EPJSON_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>


%{
  #include <epjson/epJSONTranslator.hpp>
%}

// %include <epjson/EPJSONTranslator.hpp>

#endif // EPJSON_I

