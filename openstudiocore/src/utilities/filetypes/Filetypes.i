#ifndef UTILITIES_FILETYPES_I
#define UTILITIES_FILETYPES_I

%{
  namespace openstudio{}
  using namespace openstudio;
  #include <utilities/filetypes/EpwFile.hpp>
  #include <utilities/filetypes/WorkflowStepResult.hpp>
  #include <utilities/filetypes/WorkflowStep.hpp>
  #include <utilities/filetypes/WorkflowJSON.hpp>
  #include <utilities/units/Quantity.hpp>
  #include <utilities/idf/IdfObject.hpp>
  #include <utilities/idf/WorkspaceObject.hpp>
%}

%import <utilities/units/Quantity.i>

%template(EpwDataPointVector) std::vector<openstudio::EpwDataPoint>;
%template(OptionalEpwDataPoint) boost::optional<openstudio::EpwDataPoint>;
%template(OptionalAirState) boost::optional<openstudio::AirState>;

%ignore std::vector<openstudio::EpwFile>::vector(size_type);
%ignore std::vector<openstudio::EpwFile>::resize(size_type);
%template(EpwFileVector) std::vector<openstudio::EpwFile>;
%template(OptionalEpwFile) boost::optional<openstudio::EpwFile>;

%template(WorkflowStepResultVector) std::vector<openstudio::WorkflowStepResult>;
%template(OptionalWorkflowStepResult) boost::optional<openstudio::WorkflowStepResult>;
%template(WorkflowStepResultValue) std::pair<std::string, Variant>;
%template(WorkflowStepResultValueVector) std::vector<std::pair<std::string, Variant> >;

%ignore std::vector<openstudio::WorkflowStep>::vector(size_type);
%ignore std::vector<openstudio::WorkflowStep>::resize(size_type);
%template(WorkflowStepVector) std::vector<openstudio::WorkflowStep>;
%template(OptionalWorkflowStep) boost::optional<openstudio::WorkflowStep>;

%template(WorkflowJSONVector) std::vector<openstudio::WorkflowJSON>;
%template(OptionalWorkflowJSON) boost::optional<openstudio::WorkflowJSON>;

%include <utilities/filetypes/EpwFile.hpp>
%include <utilities/filetypes/WorkflowStep.hpp>
%include <utilities/filetypes/WorkflowJSON.hpp>

#endif //UTILITIES_FILETYPES_I 
