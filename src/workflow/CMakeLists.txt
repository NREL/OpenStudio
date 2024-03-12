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

if(BUILD_TESTING)
  set(openstudio_workflow_test_depends
    openstudio_workflow
    CONAN_PKG::boost # Maybe at some point replace with openstudiolib more simply
    CONAN_PKG::fmt
  )

  set(openstudio_workflow_test_src
    test/Util_GTest.cpp
    test/RunPreProcessMonthlyReports_GTest.cpp
  )

  CREATE_TEST_TARGETS(openstudio_workflow "${openstudio_workflow_test_src}" "${openstudio_workflow_test_depends}")
endif()
