#ifndef UTILITIES_UTILITIESTIME_I
#define UTILITIES_UTILITIESTIME_I

#ifdef SWIGPYTHON
%module openstudioutilitiestime
#endif


#define UTILITIES_API
#define UTILITIES_TEMPLATE_EXT

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/UtilitiesCore.i>

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

%}


// order that these are loaded matters, e.g TimeSeries in data needs to know about DateTime in time
%include <utilities/time/Time.i>

#endif // UTILITIES_UTILITIESTIME_I
