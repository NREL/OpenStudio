# create a library out of the runmanager tools

set(target_name openstudio_runmanager)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

MAKE_LITE_SQL_TARGET(RunManager.xml runmanagerdatabase)

set(runmanager_src
  RunManagerAPI.hpp
  FileInfo.hpp
  FileInfo.cpp
  JobErrors.hpp
  JobErrors.cpp
  JobParam.hpp
  Job.cpp
  Job.hpp
  Job_Impl.cpp
  Job_Impl.hpp
  JobState.hpp
  RunManager.cpp
  RunManager.hpp
  RunManager_Impl.cpp
  RunManager_Impl.hpp
  ToolBasedJob.cpp
  ToolBasedJob.hpp
  EnergyPlusJob.cpp
  EnergyPlusJob.hpp
  ModelToIdfJob.cpp
  ModelToIdfJob.hpp
  IdfToModelJob.cpp
  IdfToModelJob.hpp
  "${CMAKE_CURRENT_BINARY_DIR}/runmanagerdatabase.hxx"
  "${CMAKE_CURRENT_BINARY_DIR}/runmanagerdatabase.cxx"
  JobFactory.hpp
  JobFactory.cpp
  Configuration.hpp
  Configuration.cpp
  JobOutputCleanup.hpp
  JobOutputCleanup.cpp
  JobErrorsDisplay.hpp
  JobErrorsDisplay.cpp
  RunManagerStatus.hpp
  RunManagerStatus.cpp
  ConfigOptions.hpp
  ConfigOptions.cpp
  CalculateEconomicsJob.hpp
  CalculateEconomicsJob.cpp
  Process.hpp
  Process.cpp
  ProcessCreator.hpp
  LocalProcess.hpp
  LocalProcess.cpp
  LocalProcessCreator.hpp
  LocalProcessCreator.cpp
  RunManager_Util.hpp
  RunManager_Util.cpp
  ExpandObjectsJob.cpp
  ExpandObjectsJob.hpp
  XMLPreprocessorJob.cpp
  XMLPreprocessorJob.hpp
  ReadVarsJob.cpp
  ReadVarsJob.hpp
  AdvancedStatus.cpp
  AdvancedStatus.hpp
  ToolInfo.cpp
  ToolInfo.hpp
  Workflow.cpp
  Workflow.hpp
  RunManagerStatusWidget.cpp
  RunManagerStatusWidget.hpp
  JobStatusWidget.cpp
  JobStatusWidget.hpp
  AddTool.cpp
  AddTool.hpp
  WeatherFileFinder.hpp
  WeatherFileFinder.cpp
  PreviewIESJob.hpp
  PreviewIESJob.cpp
  RubyJob.hpp
  RubyJob.cpp
  UserScriptJob.hpp
  UserScriptJob.cpp
  NullJob.hpp
  NullJob.cpp
  RubyJobUtils.hpp
  RubyJobUtils.cpp
  NormalizeURLs.hpp
  NormalizeURLs.cpp
  TreeStatus.hpp
  ToolFinder.hpp
  ToolFinder.cpp
  ModelInModelOutJob.hpp
  ModelInModelOutJob.cpp
  EnergyPlusPostProcessJob.hpp
  EnergyPlusPostProcessJob.cpp
  EnergyPlusPreProcessJob.hpp
  EnergyPlusPreProcessJob.cpp
  ParallelEnergyPlusSplitJob.hpp
  ParallelEnergyPlusSplitJob.cpp
  ParallelEnergyPlusJoinJob.hpp
  ParallelEnergyPlusJoinJob.cpp
  BasementJob.hpp
  BasementJob.cpp
  SlabJob.hpp
  SlabJob.cpp
  PostProcessReporting.hpp
  PostProcessReporting.cpp
  OpenStudioPostProcessJob.hpp
  OpenStudioPostProcessJob.cpp
  ModelToRadJob.hpp
  ModelToRadJob.cpp
  ModelToRadPreProcessJob.hpp
  ModelToRadPreProcessJob.cpp
  DakotaJob.hpp
  DakotaJob.cpp
  WorkItem.hpp
  WorkItem.cpp
  ParallelEnergyPlus/ParallelEnergyPlus.hpp
  ParallelEnergyPlus/ParallelEnergyPlus.cpp
  ParallelEnergyPlus/SqliteObject.hpp
  ParallelEnergyPlus/SqliteObject.cpp
  ParallelEnergyPlus/SqliteMerge.hpp
  ParallelEnergyPlus/SqliteMerge.cpp

  ErrorEstimation.hpp
  ErrorEstimation.cpp

  LinearApproximation.hpp
  LinearApproximation.cpp

  SimulationEngine.hpp
  SimulationEngine.cpp
  JSON.cpp
  JSON.hpp

  MergedJobResults.hpp

  RunManagerWatcher.hpp
  RunManagerWatcher.cpp

  JSONWorkflowOptions.hpp
)

if(BUILD_TESTING OR BUILD_PACKAGE)
  configure_file("Test/ToolBin.hxx.in" "${CMAKE_BINARY_DIR}/src/runmanager/Test/ToolBin.hxx")
endif()

set(runmanager_depends
  openstudio_radiance
  openstudio_model
  openstudio_energyplus
  openstudio_ruleset
)

set(runmanager_documentation_src
  mainpage.hpp
)

set(runmanager_UI
  Configuration.ui
  JobOutputCleanup.ui
  JobErrorsDisplay.ui
  RunManagerStatus.ui
  RunManagerStatusWidget.ui
  JobStatusWidget.ui
  AddTool.ui
)

# moc files
set(runmanager_moc
  Job_Impl.hpp
  RunManager_Impl.hpp
  EnergyPlusJob.hpp
  ModelToIdfJob.hpp
  IdfToModelJob.hpp
  JobOutputCleanup.hpp
  JobErrorsDisplay.hpp
  RunManagerStatus.hpp
  Configuration.hpp
  Process.hpp
  LocalProcess.hpp
  CalculateEconomicsJob.hpp
  ProcessCreator.hpp
  LocalProcessCreator.hpp
  ToolBasedJob.hpp
  ExpandObjectsJob.hpp
  XMLPreprocessorJob.hpp
  ReadVarsJob.hpp
  RunManagerStatusWidget.hpp
  JobStatusWidget.hpp
  AddTool.hpp
  PreviewIESJob.hpp
  RubyJob.hpp
  UserScriptJob.hpp
  NullJob.hpp
  ModelInModelOutJob.hpp
  EnergyPlusPostProcessJob.hpp
  OpenStudioPostProcessJob.hpp
  EnergyPlusPreProcessJob.hpp
  ParallelEnergyPlusSplitJob.hpp
  ParallelEnergyPlusJoinJob.hpp
  BasementJob.hpp
  SlabJob.hpp
  ModelToRadJob.hpp
  ModelToRadPreProcessJob.hpp
  DakotaJob.hpp
  SimulationEngine.hpp
  RunManagerWatcher.hpp
)

set(runmanager_test_moc
  Test/SignalListener.hpp
)

## Qt moc generation
qt5_wrap_cpp_minimally(runmanager_moc_src ${runmanager_moc})
qt5_wrap_cpp_minimally(runmanager_test_moc_src ${runmanager_test_moc})

qt5_wrap_ui(runmanager_UIS ${runmanager_UI})

# resource files
set(runmanager_qrc
  RunManager.qrc
)

# generate rules for building source files from the resources
qt5_add_resources(runmanager_qrcs ${runmanager_qrc})


source_group(runmanager FILES ${runmanager_src} ${runmanager_moc_src})
source_group(runmanager_documentation FILES ${rmkt_documentation_src})

set(runmanager_src
  ${runmanager_src}
  ${runmanager_moc_src}
  ${runmanager_UIS}
  ${runmanager_documentation}
)

set(runmanager_test_src
  Test/RunManagerTestFixture.cpp
  Test/RunManagerTestFixture.hpp
  Test/Job_GTest.cpp
  Test/Workflow_GTest.cpp
  Test/RunManager_GTest.cpp
  Test/ConfigOptions_GTest.cpp
  Test/JobRunOrder_GTest.cpp
  Test/ToolFinder_GTest.cpp
  Test/MergeJobs_GTest.cpp
  Test/EnergyPlusPostProcessJob_GTest.cpp
  Test/OpenStudioPostProcessJob_GTest.cpp
  Test/FlatOutDir_GTest.cpp
  Test/ModelToRadJob_GTest.cpp
  Test/UserScript_GTest.cpp
  Test/ClearJobsPerformance_GTest.cpp
  Test/JobCreatePerformance_GTest.cpp
  Test/JobClean_GTest.cpp
  Test/JobStatePersistence_GTest.cpp
  Test/WeatherFileFinder_GTest.cpp
  Test/OSResultLoading_GTest.cpp
  Test/ParallelEnergyPlusJob_GTest.cpp
  Test/ErrorEstimation_GTest.cpp
  Test/JSON_GTest.cpp
  Test/ExternallyManagedJobs_GTest.cpp
  Test/RunJSONWorkflow_GTest.cpp
  Test/JobErrors_GTest.cpp
  "${CMAKE_BINARY_DIR}/src/runmanager/Test/ToolBin.hxx"
)

add_library(openstudio_runmanager SHARED
  ${runmanager_src}
  ${runmanager_qrcs}
)

AddPCH(openstudio_runmanager)



#add_dependencies(openstudio_runmanager openstudio_energyplus openstudio_model)

#if(OPENSSL_FOUND AND BUILD_WITH_OPENSSL)
#  target_link_libraries(${target_name} openstudio_energyplus openstudio_model openstudio_radiance openstudio_ruleset openstudio_isomodel ${OPENSSL_LIBRARIES})
#
#  if(MSVC)
#    target_link_libraries(${target_name} delayimp ws2_32)
#
#    set_target_properties(${target_name} PROPERTIES LINK_FLAGS "/DELAYLOAD:libeay32.dll")
#  endif()
#
#  add_definitions(-DRUNMANAGER_HAVE_OPENSSL)
#else()
  target_link_libraries(${target_name} openstudio_energyplus openstudio_model openstudio_radiance openstudio_ruleset openstudio_isomodel)
#endif()

if(NOT APPLE)
  install(TARGETS ${target_name}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
  )
endif()

if(BUILD_TESTING)
  add_library(openstudio_runmanager_lib_test STATIC
    ${runmanager_test_moc_src}
  )
  set(runmanager_test_depends
    ${runmanager_depends}
    openstudio_runmanager_lib_test
  )
else()
  set(runmanager_test_depends
    ${runmanager_depends}
  )
endif()

CREATE_SRC_GROUPS("${runmanager_test_src}")
CREATE_TEST_TARGETS("openstudio_runmanager" "${runmanager_test_src}" "${runmanager_test_depends}")


set(runmanager_swig_src
  RunManager.i
)


CREATE_SRC_GROUPS("${runmanager_swig_src}")
MAKE_SWIG_TARGET(OpenStudioRunManager runmanager "${CMAKE_CURRENT_SOURCE_DIR}/RunManager.i" "${runmanager_swig_src}" openstudio_runmanager "OpenStudioRadiance;OpenStudioEnergyPlus;OpenStudioRuleset")
