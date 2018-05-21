#ifndef UTILITIES_FILETYPES_I
#define UTILITIES_FILETYPES_I

%{
  namespace openstudio{}
  using namespace openstudio;
  #include <utilities/filetypes/EpwFile.hpp>
  #include <utilities/filetypes/RunOptions.hpp>
  #include <utilities/filetypes/RunOptions_Impl.hpp>
  #include <utilities/filetypes/WorkflowStepResult.hpp>
  #include <utilities/filetypes/WorkflowStep.hpp>
  #include <utilities/filetypes/WorkflowStep_Impl.hpp>
  #include <utilities/filetypes/WorkflowJSON.hpp>

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

%template(EpwDataPointVector) std::vector<openstudio::EpwDataPoint>;
%template(EpwDesignConditionVector) std::vector<openstudio::EpwDesignCondition>;
%template(OptionalEpwDataPoint) boost::optional<openstudio::EpwDataPoint>;
%template(OptionalAirState) boost::optional<openstudio::AirState>;

%ignore std::vector<openstudio::EpwFile>::vector(size_type);
%ignore std::vector<openstudio::EpwFile>::resize(size_type);
%template(EpwFileVector) std::vector<openstudio::EpwFile>;
%template(OptionalEpwFile) boost::optional<openstudio::EpwFile>;

%template(OptionalCustomOutputAdapter) boost::optional<openstudio::CustomOutputAdapter>;

%template(OptionalRunOptions) boost::optional<openstudio::RunOptions>;

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

%include <utilities/filetypes/EpwFile.hpp>
%include <utilities/filetypes/RunOptions.hpp>
%include <utilities/filetypes/WorkflowStepResult.hpp>
%include <utilities/filetypes/WorkflowStep.hpp>
%include <utilities/filetypes/WorkflowJSON.hpp>

// extend class
%extend openstudio::RunOptions{
  std::string __str__() {
    std::ostringstream os;
    os << *self;
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
#endif //UTILITIES_FILETYPES_I
