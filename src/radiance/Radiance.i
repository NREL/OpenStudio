#ifndef RADIANCE_RADIANCE_I
#define RADIANCE_RADIANCE_I

#ifdef SWIGPYTHON
%module openstudioradiance
#endif


#define UTILITIES_API
#define RADIANCE_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  #include <radiance/ForwardTranslator.hpp>
  #include <radiance/Utils.hpp>
  using namespace openstudio::radiance;
  using namespace openstudio;
%}

%ignore formatString;
%ignore cleanName;

// #ifdef SWIGCSHARP
%rename(RadianceForwardTranslator) openstudio::radiance::ForwardTranslator;
// #endif

%include <radiance/ForwardTranslator.hpp>
%include <radiance/Utils.hpp>

#endif //RADIANCE_RADIANCE_I
