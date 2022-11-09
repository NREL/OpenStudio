add_library(openstudio_workflow
  OBJECT
  OSWorkflow.hpp
  OSWorkflow.cpp
  WorkflowRunOptions.hpp
  WorkflowRunOptions.cpp

  # Jobs
  RunInitialization.cpp
  RunTranslator.cpp

  # Util
  Util.hpp
  Util.cpp
)

target_link_libraries(openstudio_workflow PRIVATE openstudiolib)
