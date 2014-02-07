#ifndef CONTAM_I
#define CONTAM_I

#ifdef SWIGPYTHON
%module openstudiocontam
#endif


#define UTILITIES_API
#define CONTAM_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  //#include <contam/ReverseTranslator.hpp>
  #include <contam/ForwardTranslator.hpp>
  using namespace openstudio::contam;
  using namespace openstudio;
  
  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
%}

%ignore QSharedData;
%ignore PrjModelImpl;
%ignore PrjModel(Reader &input);

// #ifdef SWIGCSHARP
//%rename(ContamReverseTranslator) openstudio::contam::ReverseTranslator;
%rename(ContamForwardTranslator) openstudio::contam::ForwardTranslator;
// #endif

// DLM: this is neccesary for now due to https://github.com/NREL/OpenStudio/issues/679
// eventually we should compile C# code into separate namespaces
#ifdef SWIGCSHARP
%rename(PrjDataPoint) openstudio::contam::DataPoint;
%rename(PrjPath) openstudio::contam::Path;
#endif

%include <contam/PrjSubobjects.hpp>
%include <contam/PrjObjects.hpp>
%include <contam/PrjAirflowElements.hpp>
%include <contam/PrjModel.hpp>
//%include <contam/ReverseTranslator.hpp>
%include <contam/ForwardTranslator.hpp>

#endif //CONTAM_I 
