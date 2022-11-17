add_library(openstudio_workflow
  OBJECT
  OSWorkflow.hpp
  OSWorkflow.cpp
  WorkflowRunOptions.hpp
  WorkflowRunOptions.cpp

  # Jobs
  RunInitialization.cpp
  RunOpenStudioMeasures.cpp
  RunTranslator.cpp
  RunEnergyPlusMeasures.cpp
  RunPreProcess.cpp
  RunEnergyPlus.cpp
  RunReportingMeasures.cpp
  RunPostProcess.cpp
  RunCleanup.cpp

  ApplyMeasure.cpp

  # Util
  Util.hpp
  Util.cpp
  Timer.hpp
  Timer.cpp
)

target_link_libraries(openstudio_workflow PRIVATE openstudiolib)
