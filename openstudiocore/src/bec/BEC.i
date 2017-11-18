#ifndef BEC_I
#define BEC_I

#ifdef SWIGPYTHON
%module openstudiobec
#endif


#define UTILITIES_API
#define BEC_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  #include <bec/ReverseTranslator.hpp>
  #include <bec/ForwardTranslator.hpp>
  using namespace openstudio::bec;
  using namespace openstudio;
  
  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
  
  #include <QDomDocument>
  #include <QDomElement>
%}

// #ifdef SWIGCSHARP
%rename(becReverseTranslator) openstudio::bec::ReverseTranslator;
%rename(becForwardTranslator) openstudio::bec::ForwardTranslator;
// #endif

%include <bec/ReverseTranslator.hpp>
%include <bec/ForwardTranslator.hpp>

#endif //BEC_I 
