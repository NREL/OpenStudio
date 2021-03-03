#ifndef EPJSON_I
#define EPJSON_I

#ifdef SWIGPYTHON
%module openstudioepjson
#endif


#define UTILITIES_API
#define EPJSON_API

// Ignore stuff that takes/returns Json::Value
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

#if defined SWIGRUBY
  // This isn't super clean and there might be a better way with typemaps in SWIG rather than using a String in between,
  // but still helpful I think: we redefine toJSON that will return a native ruby hash. 'json' is part of ruby stdlib since at least ruby 2.5.5
  %init %{
    rb_eval_string("OpenStudio::EPJSON.module_eval { define_method(:toJSON) { |arg| json_str = self.toJSONString(arg); require 'json'; JSON.load(json_str);  }; module_function(:toJSON) }");
  %}
#endif

#endif // EPJSON_I

