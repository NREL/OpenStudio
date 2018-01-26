#ifndef UTILITIES_UTILITIESPLOT_I
#define UTILITIES_UTILITIESPLOT_I

#ifdef SWIGPYTHON
%module openstudioutilitiesplot
#endif


#define UTILITIES_API
#define UTILITIES_TEMPLATE_EXT

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/UtilitiesData.i>

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
  #include <QComboBox>
  #include <QMainWindow>
  #include <QNetworkAccessManager>
  #include <utilities/core/PathWatcher.hpp>
  #include <utilities/core/UpdateManager.hpp>
%}


// order that these are loaded matters, e.g TimeSeries in data needs to know about DateTime in time
%include <utilities/plot/Plot.i>

#endif // UTILITIES_UTILITIESPLOT_I
