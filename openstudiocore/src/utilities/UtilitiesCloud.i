#ifndef UTILITIES_UTILITIESCLOUD_I
#define UTILITIES_UTILITIESCLOUD_I


#ifdef SWIGPYTHON
%module openstudioutilitiescloud
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

  #include <utilities/core/Application.hpp>
  #include <QAction> 
%}

%include <utilities/cloud/Cloud.i>

#endif // UTILITIES_UTILITIESCLOUD_I
