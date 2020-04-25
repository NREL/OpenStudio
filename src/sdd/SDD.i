#ifndef SDD_I
#define SDD_I

#ifdef SWIGPYTHON
%module openstudiosdd
#endif


#define UTILITIES_API
#define SDD_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  #include <sdd/ReverseTranslator.hpp>
  #include <sdd/ForwardTranslator.hpp>
  using namespace openstudio::sdd;
  using namespace openstudio;

  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
%}

// #ifdef SWIGCSHARP
%rename(SddReverseTranslator) openstudio::sdd::ReverseTranslator;
%rename(SddForwardTranslator) openstudio::sdd::ForwardTranslator;
// #endif

%include <sdd/ReverseTranslator.hpp>
%include <sdd/ForwardTranslator.hpp>

#endif //SDD_I
