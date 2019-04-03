#ifndef GBXML_I
#define GBXML_I

#ifdef SWIGPYTHON
%module openstudiogbxml
#endif


#define UTILITIES_API
#define GBXML_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  #include <gbxml/ReverseTranslator.hpp>
  #include <gbxml/ForwardTranslator.hpp>
  using namespace openstudio::gbxml;
  using namespace openstudio;

  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
%}

// #ifdef SWIGCSHARP
%rename(GbXMLReverseTranslator) openstudio::gbxml::ReverseTranslator;
%rename(GbXMLForwardTranslator) openstudio::gbxml::ForwardTranslator;
// #endif

%include <gbxml/ReverseTranslator.hpp>
%include <gbxml/ForwardTranslator.hpp>

#endif //GBXML_I
