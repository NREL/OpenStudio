add_library(openstudio_workflow
  OBJECT
  OSWorkflow.hpp
  OSWorkflow.cpp
  WorkflowRunOptions.hpp
  WorkflowRunOptions.cpp
)

target_link_libraries(openstudio_workflow PRIVATE openstudiolib)
