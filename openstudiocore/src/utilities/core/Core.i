#ifndef UTILITIES_CORE_CORE_I
#define UTILITIES_CORE_CORE_I

// does not turn on directors for all classes, just enables them
%module(directors="1") OpenStudio


%ignore openstudio::detail;

%{
  #include <utilities/core/StringHelpers.hpp>
  #include <utilities/core/FileReference.hpp>
  #include <utilities/core/Compare.hpp>
  #include <utilities/core/Optional.hpp>
  #include <utilities/core/ApplicationPathHelpers.hpp>  
  
  #include <utilities/math/FloatCompare.hpp>
  
  #include <OpenStudio.hxx>
  #include <QAction>
%}

#if defined(SWIGJAVA)

  %ignore openstudio::toString(const std::string&);
  %ignore openstudio::toString(const std::wstring&);
  %ignore openstudio::toString(const char *);
  %ignore openstudio::toQString(const std::wstring& w);
#endif


%include <utilities/core/Qt.i>
%include <utilities/core/Enum.i>
//%include <utilities/core/Exception.i> // moved ahead in include order in CommonInclude
%include <utilities/core/Path.i>
%include <OpenStudio.hxx>
%include <utilities/core/ApplicationPathHelpers.hpp>
%include <utilities/core/PathWatcher.i>
%include <utilities/core/String.hpp>
%include <utilities/core/StringHelpers.hpp>
%include <utilities/core/Singleton.i>
%include <utilities/core/Application.i>
%include <utilities/core/Logger.i>
%include <utilities/core/UpdateManager.i>
%include <utilities/core/Url.i>
%include <utilities/core/UUID.i>
%include <utilities/core/Checksum.i>
%include <utilities/core/Optional.hpp>
%include <utilities/core/UnzipFile.i>
%include <utilities/core/ZipFile.i>

// DLM@20110107: this is causing issues for C#
#if defined(SWIGRUBY) || defined(SWIGJAVASCRIPT)
%include <utilities/core/System.i>
#endif

%template(QSqlDatabasePtr) std::shared_ptr<QSqlDatabase>;
%template(FileReferenceTypeVector) std::vector<openstudio::FileReferenceType>;
%template(OptionalFileReferenceType) boost::optional<openstudio::FileReferenceType>;
%ignore std::vector<openstudio::FileReference>::vector(size_type);
%ignore std::vector<openstudio::FileReference>::resize(size_type);
%template(FileReferenceVector) std::vector<openstudio::FileReference>;
%template(OptionalFileReference) boost::optional<openstudio::FileReference>;
%template(OptionalQUrl) boost::optional<QUrl>;
%template(OptionalQVariant) boost::optional<QVariant>;
%template(OptionalVersionString) boost::optional<openstudio::VersionString>;

%include <utilities/core/FileReference.hpp>

%ignore openstudio::checkPtrVecEqual;
%ignore openstudio::firstOfPairEqual;
%ignore openstudio::secondOfPairEqual;
%ignore openstudio::firstOfPairIStringEqual;
%ignore openstudio::secondOfPairIStringEqual;
%ignore openstudio::FirstOfPairLess;
%ignore openstudio::SecondOfPairLess;
%ignore openstudio::IndexLess;
%ignore openstudio::uuidsEqual;
%ignore openstudio::uuidsAndVersionsEqual;
%ignore openstudio::uuidEquals;
%ignore openstudio::handleEquals;

%include <utilities/core/Compare.hpp>

// not the best place for this, but I don't want to make a new target just for these
%include <utilities/math/FloatCompare.hpp>
%template(DoublesEqual) openstudio::equal<double>;
%template(DoublesLessThanOrEqual) openstudio::lessThanOrEqual<double>;
%template(DoublesGreaterThanOrEqual) openstudio::greaterThanOrEqual<double>;
%template(DoublesRelativeError) openstudio::relativeError<double>;

#endif // UTILITIES_CORE_CORE_I
