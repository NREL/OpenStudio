#ifndef SIMXML_I
#define SIMXML_I

#define UTILITIES_API
#define SIMXML_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  #include <ReverseTranslator.hpp>
  using namespace openstudio::simxml;
  using namespace openstudio;
  
  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
  
  #include <QDomDocument>
  #include <QDomElement>
%}

// #ifdef SWIGCSHARP
%rename(SimXMLReverseTranslator) openstudio::simxml::ReverseTranslator;
// #endif

%include <ReverseTranslator.hpp>

#endif //SIMXML_I 
