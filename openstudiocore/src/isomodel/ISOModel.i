#ifndef ISOMODEL_I
#define ISOMODEL_I

#ifdef SWIGPYTHON
%module openstudioisomodel
#endif

#define UTILITIES_API
#define ISOMODEL_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  #include <isomodel/ForwardTranslator.hpp>
  #include <isomodel/UserModel.hpp>
  #include <isomodel/SimModel.hpp>
  
  using namespace openstudio::isomodel;
  using namespace openstudio;
  
  #include <model/Model.hpp>
  #include <utilities/data/EndUses.hpp>
%}

// #ifdef SWIGCSHARP
%rename(ISOModelForwardTranslator) openstudio::isomodel::ForwardTranslator;
// #endif

%include <isomodel/SimModel.hpp>
%include <isomodel/UserModel.hpp>
%include <isomodel/ForwardTranslator.hpp>

#endif //ISOMODEL_I 
