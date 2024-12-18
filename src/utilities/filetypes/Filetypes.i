#ifndef UTILITIES_FILETYPES_I
#define UTILITIES_FILETYPES_I


#if defined(SWIGCSHARP)

  // Avoid triggering a SWIG warning: 'string' is a C# keyword
  %rename(toString) openstudio::RunOptions::string;
  %rename(toString) openstudio::ForwardTranslatorOptions::string;
  %rename(toString) openstudio::WorkflowStepValue::string;
  %rename(toString) openstudio::WorkflowStepResult::string;
  %rename(toString) openstudio::WorkflowStep::string;
  %rename(toString) openstudio::WorkflowJSON::string;
  %rename(toString) openstudio::CSVFile::string;
  %rename(toString) openstudio::StandardsJSON::string;

  // Overload resolution
  %ignore openstudio::WorkflowStepValue::WorkflowStepValue(std::string const&, char const*);
  %ignore openstudio::MeasureStep::setArgument(std::string const&, char const*);

  // Ignore Json::Value return type (toJSON / fromJSON are globally ignored already)
  %ignore openstudio::StandardsJSON::getPrimaryKey;

  %ignore openstudio::CustomOutputAdapter::CustomOutputAdapter;
  %ignore openstudio::CustomOutputAdapter::optionsJSON;

#endif


%{
  namespace openstudio{}
  using namespace openstudio;
  #include <utilities/filetypes/CSVFile.hpp>
  #include <utilities/filetypes/EpwFile.hpp>
  #include <utilities/filetypes/RunOptions.hpp>
  #include <utilities/filetypes/RunOptions_Impl.hpp>
  #include <utilities/filetypes/ForwardTranslatorOptions.hpp>
  #include <utilities/filetypes/WorkflowStepResult.hpp>
  #include <utilities/filetypes/WorkflowStep.hpp>
  #include <utilities/filetypes/WorkflowStep_Impl.hpp>
  #include <utilities/filetypes/WorkflowJSON.hpp>
  #include <utilities/filetypes/StandardsJSON.hpp>

  // this is all garbage, I don't know why it is needed, probably comes from quantity
  #include <utilities/units/Quantity.hpp>
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

//%import <utilities/units/Quantity.i>
%import <utilities/data/Variant.i>

%ignore std::vector<openstudio::CSVFile>::vector(size_type);
%ignore std::vector<openstudio::CSVFile>::resize(size_type);
%template(CSVFileVector) std::vector<openstudio::CSVFile>;
%template(OptionalCSVFile) boost::optional<openstudio::CSVFile>;

%ignore std::vector<openstudio::EpwDataPoint>::vector(size_type);
%ignore std::vector<openstudio::EpwDataPoint>::resize(size_type);
%template(EpwDataPointVector) std::vector<openstudio::EpwDataPoint>;

%ignore std::vector<openstudio::EpwDesignCondition>::vector(size_type);
%ignore std::vector<openstudio::EpwDesignCondition>::resize(size_type);
%template(EpwDesignConditionVector) std::vector<openstudio::EpwDesignCondition>;
%template(OptionalEpwDesignCondition) boost::optional<openstudio::EpwDesignCondition>;

%ignore std::vector<openstudio::EpwGroundTemperatureDepth>::vector(size_type);
%ignore std::vector<openstudio::EpwGroundTemperatureDepth>::resize(size_type);
%template(EpwGroundTemperatureDepthVector) std::vector<openstudio::EpwGroundTemperatureDepth>;

%ignore std::vector<openstudio::EpwHoliday>::vector(size_type);
%ignore std::vector<openstudio::EpwHoliday>::resize(size_type);
%template(EpwHolidayVector) std::vector< openstudio::EpwHoliday >;

%template(OptionalEpwDataPoint) boost::optional<openstudio::EpwDataPoint>;
%template(OptionalAirState) boost::optional<openstudio::AirState>;

%ignore std::vector<openstudio::EpwFile>::vector(size_type);
%ignore std::vector<openstudio::EpwFile>::resize(size_type);
%template(EpwFileVector) std::vector<openstudio::EpwFile>;
%template(OptionalEpwFile) boost::optional<openstudio::EpwFile>;

%template(OptionalCustomOutputAdapter) boost::optional<openstudio::CustomOutputAdapter>;

%template(OptionalRunOptions) boost::optional<openstudio::RunOptions>;
%template(OptionalForwardTranslatorOptions) boost::optional<openstudio::ForwardTranslatorOptions>;
%template(ForwardTranslatorOptionKeyMethodVector) std::vector<openstudio::ForwardTranslatorOptionKeyMethod>;

%template(OptionalStepResult) boost::optional<openstudio::StepResult>;

%ignore std::vector<openstudio::WorkflowStepValue>::vector(size_type);
%ignore std::vector<openstudio::WorkflowStepValue>::resize(size_type);
%template(WorkflowStepValueVector) std::vector<openstudio::WorkflowStepValue>;
%template(OptionalWorkflowStepValue) boost::optional<openstudio::WorkflowStepValue>;

%template(WorkflowStepResultVector) std::vector<openstudio::WorkflowStepResult>;
%template(OptionalWorkflowStepResult) boost::optional<openstudio::WorkflowStepResult>;

%ignore std::vector<openstudio::WorkflowStep>::vector(size_type);
%ignore std::vector<openstudio::WorkflowStep>::resize(size_type);
%template(WorkflowStepVector) std::vector<openstudio::WorkflowStep>;
%template(OptionalWorkflowStep) boost::optional<openstudio::WorkflowStep>;

%ignore std::vector<openstudio::MeasureStep>::vector(size_type);
%ignore std::vector<openstudio::MeasureStep>::resize(size_type);
%template(MeasureStepVector) std::vector<openstudio::MeasureStep>;
%template(OptionalMeasureStep) boost::optional<openstudio::MeasureStep>;

%template(WorkflowJSONVector) std::vector<openstudio::WorkflowJSON>;
%template(OptionalWorkflowJSON) boost::optional<openstudio::WorkflowJSON>;

%template(StandardsJSONVector) std::vector<openstudio::StandardsJSON>;
%template(OptionalStandardsJSON) boost::optional<openstudio::StandardsJSON>;

%include <utilities/filetypes/CSVFile.hpp>
%include <utilities/filetypes/EpwFile.hpp>
%include <utilities/filetypes/RunOptions.hpp>
%include <utilities/filetypes/ForwardTranslatorOptions.hpp>
%include <utilities/filetypes/WorkflowStepResult.hpp>
%include <utilities/filetypes/WorkflowStep.hpp>
%include <utilities/filetypes/WorkflowJSON.hpp>
%include <utilities/filetypes/StandardsJSON.hpp>

// extend class
%extend openstudio::RunOptions{
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

%extend openstudio::ForwardTranslatorOptions{
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

// extend classes
%extend openstudio::ForwardTranslatorOptionKeyMethod {
  std::string __str__() {
    std::ostringstream os;
    os << *$self;
    return os.str();
  }
};

%extend openstudio::WorkflowStepResult{
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

%extend openstudio::WorkflowStep{
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }

  boost::optional<openstudio::MeasureStep> to_MeasureStep() {
    return self->optionalCast<openstudio::MeasureStep>();
  }

};

%extend openstudio::WorkflowJSON{
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

%extend openstudio::StandardsJSON{
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};
#endif //UTILITIES_FILETYPES_I
