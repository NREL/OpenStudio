#ifndef EPJSON_I
#define EPJSON_I

#ifdef SWIGPYTHON
%module openstudioepjson
#endif


%include <utilities/UtilitiesAPI.hpp>
#define EPJSON_API

// You're better off just loading the json directly in the target language, so ignore
%ignore openstudio::epJSON::loadJSON;
#ifdef SWIGCSHARP
%ignore openstudio::epJSON::toJSON;
#endif

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%include <model/Model_Common_Include.i>
%import <model/ModelCore.i>

%{
  #include <utilities/core/Path.hpp>
  #include <epjson/epJSONTranslator.hpp>
%}

%include <epjson/epJSONTranslator.hpp>

#endif // EPJSON_I
