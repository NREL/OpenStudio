#ifndef EPJSON_I
#define EPJSON_I

#ifdef SWIGPYTHON
%module openstudioepjson
#endif


#define UTILITIES_API
#define EPJSON_API


%ignore openstudio::epJSON::toJSON;
%ignore openstudio::epJSON::loadJSON;

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>


%{
  #include <utilities/core/Path.hpp>
  #include <epjson/epJSONTranslator.hpp>
%}

%include <epjson/epJSONTranslator.hpp>

#endif // EPJSON_I

