#ifndef UTILITIES_UTILITIESSQL_I
#define UTILITIES_UTILITIESSQL_I

#ifdef SWIGPYTHON
%module openstudioutilitiessql
#endif


#define UTILITIES_API
#define UTILITIES_TEMPLATE_EXT

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/UtilitiesGeometry.i>

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


// order that these are loaded matters, e.g TimeSeries in data needs to know about DateTime in time
%include <utilities/sql/SqlFile.i>

%{
  struct ManageSQLite3Initialization {
    ManageSQLite3Initialization() {
      // Notes:
      // * this is a no-op if sqlite3 is already initialized.
      // * generally we don't need to call sqlite3_shutdown.
      // * sqlite3 should be self initializing, if *not* compiled with
      //   SQLITE_OMIT_AUTOINIT.
      //
      // However, it is possible when sqlite3 is called from different DLL's on Windows
      // (which have different views of globals per DLL's) that we will need to manually 
      // call this.
      //
      // This situation occurs when we use system ruby on load OpenStudioRuby DLL's on
      // Windows.
      //
      // related SWIG Doc for why this is a static object http://www.swig.org/Doc4.0/SWIG.html#SWIG_nn44
      // why we are not worried about sqlite3_shutdown http://sqlite.1065341.n5.nabble.com/Q-When-to-use-sqlite3-shutdown-td67987.html
      // notes on initializing sqlite3 http://sqlite.org/c3ref/initialize.html
      // similar issue found with Python on Windows https://stackoverflow.com/questions/24253406/access-violation-on-sqlite3-mutex-enter-why
      sqlite3_initialize();
    }
  };
  
  static ManageSQLite3Initialization sqlite3initializerobjectforswigbindings;
%}

#endif // UTILITIES_UTILITIESSQL_I
