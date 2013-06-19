#ifndef STANDARDS_STANDARDSINTERFACE_I
#define STANDARDS_STANDARDSINTERFACE_I

#ifdef SWIGPYTHON
%module openstudiostandardsinterface
#endif


#define STANDARDSINTERFACE_API
#define UTILITIES_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  #include <standardsinterface/AgnosticInterface.hpp>
  #include <standardsinterface/Result.hpp>
  #include <standardsinterface/OpenStudioStandardsInterface.hpp>
  #include <standardsinterface/Title24Interface.hpp>

  #include <standardsinterface/ObjectType.hxx>
  #include <standardsinterface/BooleanAttribute.hxx>
  #include <standardsinterface/IntegerAttribute.hxx>
  #include <standardsinterface/DoubleAttribute.hxx>
  #include <standardsinterface/EnumAttribute.hxx>
  // ETH@20110223 Do not SWIG EnumAttributeValues. Potential to cause name conflicts.
  // #include <standardsinterface/EnumAttributeValues.hxx>
  #include <standardsinterface/DataDictionary.hpp>
  
  // central list of all concrete ModelObject header files (_Impl and non-_Impl)
  #include <model/ConcreteModelObjects.hpp>
  
  using namespace openstudio;
  using namespace openstudio::standardsinterface;
%}

#ifdef SWIGRUBY
  %predicate *::good;
#endif

// ignore abstract interface class
//%ignore openstudio::standardsinterface::AgnosticInterface; 

%rename("OSStandardsInterface") openstudio::standardsinterface::OpenStudioStandardsInterface;

%include "standardsinterface/Result.hpp"

%include <standardsinterface/AgnosticInterface.hpp>
%template(AgnosticInterfaceModelObject) openstudio::standardsinterface::AgnosticInterface<openstudio::model::ModelObject>;

%template(VoidResult) openstudio::standardsinterface::Result<openstudio::standardsinterface::result::none>;
%template(BoolResult) openstudio::standardsinterface::Result<bool>;
%template(IntegerResult) openstudio::standardsinterface::Result<int>;
%template(DoubleResult) openstudio::standardsinterface::Result<double>;
%template(StringResult) openstudio::standardsinterface::Result<std::string>;
%template(ModelObjectVectorResult) openstudio::standardsinterface::Result<std::vector<openstudio::model::ModelObject> >;
%template(ModelObjectResult) openstudio::standardsinterface::Result<openstudio::model::ModelObject>;

%template(OptionalPartsOfSpeech) boost::optional<openstudio::standardsinterface::PartsOfSpeech>;

%template(ObjectTypeVector) std::vector<openstudio::standardsinterface::ObjectType>;
%template(BooleanAttributeVector) std::vector<openstudio::standardsinterface::BooleanAttribute>;
%template(IntegerAttributeVector) std::vector<openstudio::standardsinterface::IntegerAttribute>;
%template(DoubleAttributeVector) std::vector<openstudio::standardsinterface::DoubleAttribute>;
%template(EnumAttributeVector) std::vector<openstudio::standardsinterface::EnumAttribute>;
%template(OpenStudioInterfaceVector) std::vector<openstudio::standardsinterface::OpenStudioStandardsInterface>;

%include <standardsinterface/ObjectType.hxx>
%include <standardsinterface/BooleanAttribute.hxx>
%include <standardsinterface/IntegerAttribute.hxx>
%include <standardsinterface/DoubleAttribute.hxx>
%include <standardsinterface/EnumAttribute.hxx>
// ETH@20110223 Do not SWIG EnumAttributeValues. Potential to cause name conflicts.
// %include <standardsinterface/EnumAttributeValues.hxx>
%include <standardsinterface/DataDictionary.hpp>
%include <standardsinterface/OpenStudioStandardsInterface.hpp>
%include <standardsinterface/Title24Interface.hpp>

#endif // STANDARDS_STANDARDSINTERFACE_I
