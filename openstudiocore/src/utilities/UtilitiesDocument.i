#ifndef UTILITIES_UTILITIESDOCUMENT_I
#define UTILITIES_UTILITIESDOCUMENT_I

#ifdef SWIGPYTHON
%module openstudioutilitiesdocument
#endif


#define UTILITIES_API
#define UTILITIES_TEMPLATE_EXT

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i> 
%import <utilities/UtilitiesUnits.i>

//Ignore common stream related functions
//that will not be supported by SWIG


%ignore *::toText(std::ostream&) const;
%ignore *::fromText(std::istream&);
%ignore *::toXml(std::ostream&) const;
%ignore *::fromXml(std::istream&);
%ignore *::print(std::ostream&) const;
%ignore *::fromXmlStream(std::istream&);
%ignore *::toXmlStream(std::ostream&) const;

// Unit conflicts with test/unit in Ruby, rename "globally"
%rename(ZUnit) openstudio::Unit;


%{
  namespace openstudio{}
  using namespace openstudio;

  #include <utilities/core/Application.hpp>
  #include <utilities/idf/IdfObject.hpp>
  #include <utilities/idf/WorkspaceObject.hpp>
  
  #include <utilities/units/Quantity.hpp>
  #include <utilities/units/Unit.hpp>
  #include <utilities/units/BTUUnit.hpp>
  #include <utilities/units/CelsiusUnit.hpp>
  #include <utilities/units/CFMUnit.hpp>
  #include <utilities/units/FahrenheitUnit.hpp>
  #include <utilities/units/GPDUnit.hpp>
  #include <utilities/units/IPUnit.hpp>
  #include <utilities/units/Misc1Unit.hpp>
  #include <utilities/units/MPHUnit.hpp>
  #include <utilities/units/SIUnit.hpp>
  #include <utilities/units/ThermUnit.hpp>
  #include <utilities/units/WhUnit.hpp>  
 
%}


// order that these are loaded matters, e.g TimeSeries in data needs to know about DateTime in time
%include <utilities/document/Document.i>

#endif // UTILITIES_UTILITIESDOCUMENT_I
