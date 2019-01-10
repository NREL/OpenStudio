#ifndef UTILITIES_UTILITIESBCL_I
#define UTILITIES_UTILITIESBCL_I


#ifdef SWIGPYTHON
%module openstudioutilitiesbcl
#endif


#define UTILITIES_API
#define UTILITIES_TEMPLATE_EXT

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/UtilitiesCore.i>
%import <utilities/UtilitiesSql.i>

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

  #include <utilities/idf/IdfObject.hpp>
  #include <utilities/idf/WorkspaceObject.hpp>

%}


%include <utilities/bcl/LocalBCL.i>

#endif // UTILITIES_UTILITIESBCL_I
