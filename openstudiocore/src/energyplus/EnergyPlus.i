#ifndef ENERGYPLUS_I
#define ENERGYPLUS_I

#ifdef SWIGPYTHON
%module openstudioenergyplus
#endif


#define UTILITIES_API
#define MODEL_API
#define ENERGYPLUS_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>

%ignore openstudio::detail;
%ignore openstudio::model::detail;
%ignore openstudio::energyplus::detail;

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
%import <model/Model.i>
#else
%import(module="openstudiomodel") <model/Model.hpp>
%import(module="openstudiomodel") <model/ModelObject.hpp>
#endif

%{
  #include <energyplus/ForwardTranslator.hpp>
  #include <energyplus/ReverseTranslator.hpp>
  #include <energyplus/ErrorFile.hpp>
  
  using namespace openstudio;
  using namespace openstudio::model;
  using namespace openstudio::energyplus;
%}

#ifdef SWIGCSHARP
%rename(EnergyPlusForwardTranslator) openstudio::energyplus::ForwardTranslator;
%rename(EnergyPlusReverseTranslator) openstudio::energyplus::ReverseTranslator;
#endif

// Ignore Static Initializers
%ignore openstudio::energyplus::detail;
%ignore StaticInitializer;
%ignore ForwardTranslatorInitializer;
%ignore openstudio::energyplus::detail::ForwardTranslatorInitializer;

%include <energyplus/ErrorFile.hpp>
%include <energyplus/ForwardTranslator.hpp>
%include <energyplus/ReverseTranslator.hpp>

#endif //ENERGYPLUS_I 

