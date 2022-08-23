#ifndef UTILITIES_OUTPUT_SQLFILE_I
#define UTILITIES_OUTPUT_SQLFILE_I

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>

%{
  #include <utilities/sql/SqlFile.hpp>
  #include <utilities/sql/SqlFileEnums.hpp>
  #include <utilities/sql/SqlFileTimeSeriesQuery.hpp>
  #include <utilities/sql/SummaryData.hpp>

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

// ignore detail namespace
%ignore openstudio::detail;

// These functions return via reference parameters - something we cannot support with SWIG out of the box
%ignore openstudio::SqlFile::illuminanceMapMaxValue(const std::string&, double&, double&) const;
%ignore openstudio::SqlFile::illuminanceMapMaxValue(int, double&, double&) const;

// create an instantiation of the optional classes
%template(OptionalSqlFile) boost::optional<openstudio::SqlFile>;
%template(OptionalEnvironmentType) boost::optional<openstudio::EnvironmentType>;
%template(OptionalReportingFrequency) boost::optional<openstudio::ReportingFrequency>;
%template(OptionalKeyValueIdentifier) boost::optional<openstudio::KeyValueIdentifier>;
%template(OptionalEnvironmentIdentifier) boost::optional<openstudio::EnvironmentIdentifier>;
%template(OptionalTimeSeriesIdentifier) boost::optional<openstudio::TimeSeriesIdentifier>;
%template(ReportingFrequencySet) std::set<openstudio::ReportingFrequency>;
%template(ReportingFrequencyVector) std::vector<openstudio::ReportingFrequency>;
%template(IntDateTimePair) std::pair<int, openstudio::DateTime>;
%template(IntDateTimePairVector) std::vector<std::pair<int, openstudio::DateTime> >;

// Not default-constructible, so ignore vector and resize
%ignore std::vector<openstudio::SummaryData>::vector(size_type);
%ignore std::vector<openstudio::SummaryData>::resize(size_type);
%template(SummaryDataVector) std::vector<openstudio::SummaryData>;

%template(SqlTimeSeriesQueryVector) std::vector<openstudio::SqlFileTimeSeriesQuery>;

%include <utilities/sql/SummaryData.hpp>
%include <utilities/sql/SqlFile.hpp>
%include <utilities/sql/SqlFileTimeSeriesQuery.hpp>
%include <utilities/sql/SqlFileEnums.hpp>

#endif //UTILITIES_OUTPUT_SQLFILE_I
