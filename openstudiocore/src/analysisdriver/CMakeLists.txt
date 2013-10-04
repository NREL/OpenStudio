SET( target_name openstudio_analysisdriver )

SET( ${target_name}_src
  AnalysisDriverAPI.hpp
  mainpage.hpp
  AnalysisDriver.hpp
  AnalysisDriver_Impl.hpp
  AnalysisDriver.cpp
  CloudAnalysisDriver.hpp
  CloudAnalysisDriver_Impl.hpp
  CloudAnalysisDriver.cpp
  AnalysisRunOptions.hpp
  AnalysisRunOptions.cpp
  CurrentAnalysis.hpp
  CurrentAnalysis_Impl.hpp
  CurrentAnalysis.cpp
  AnalysisDriverWatcher.hpp
  AnalysisDriverWatcher.cpp
  SimpleProject.hpp
  SimpleProject_Impl.hpp
  SimpleProject.cpp
)

SET( ${target_name}_moc
  AnalysisDriver_Impl.hpp
  CloudAnalysisDriver_Impl.hpp
  CurrentAnalysis_Impl.hpp
  AnalysisDriverWatcher.hpp
  SimpleProject_Impl.hpp
)

SET( ${target_name}_test_src
  test/AnalysisDriverFixture.hpp
  test/AnalysisDriverFixture.cpp
  test/StopWatcher.hpp
  test/StopWatcher.cpp
  test/AnalysisDriverWatcher_GTest.cpp
  test/AnalysisRunOptions_GTest.cpp
  test/RuntimeBehavior_GTest.cpp
  test/DataPersistence_GTest.cpp
  test/DesignOfExperiments_GTest.cpp
  test/PostProcessJobs_GTest.cpp
  test/SimpleProject_GTest.cpp
)

IF(DAKOTA_FOUND)
  LIST(APPEND ${target_name}_test_src
    test/DDACEAlgorithm_GTest.cpp
    test/FSUDaceAlgorithm_GTest.cpp
    test/ParameterStudyAlgorithm_GTest.cpp  
    test/PSUADEDaceAlgorithm_GTest.cpp
    test/SamplingAlgorithm_GTest.cpp
    test/SimpleProjectDakota_GTest.cpp
  )
ENDIF(DAKOTA_FOUND)

SET( ${target_name}_swig_src
  AnalysisDriver.i
)

qt4_wrap_cpp( ${target_name}_moc_src ${${target_name}_moc} )

SET( ${target_name}_depends
  openstudio_project
)

ADD_LIBRARY( ${target_name} SHARED
  ${${target_name}_src}
  ${${target_name}_moc_src}
)
TARGET_LINK_LIBRARIES(${target_name} ${${target_name}_depends} )

AddPCH(${target_name})

IF( NOT APPLE )
INSTALL(TARGETS ${target_name}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib )
ENDIF()

CREATE_SRC_GROUPS( "${${target_name}_src}" )
CREATE_SRC_GROUPS( "${${target_name}_test_src}" )
CREATE_SRC_GROUPS( "${${target_name}_swig_src}" )

CREATE_TEST_TARGETS( ${target_name} "${${target_name}_test_src}" "${${target_name}_depends};${RUBY_LIBRARY}")
MAKE_SWIG_TARGET( OpenStudioAnalysisDriver AnalysisDriver "${CMAKE_CURRENT_SOURCE_DIR}/AnalysisDriver.i" "${${target_name}_swig_src}" "${target_name}" "OpenStudioProject")

IF (BUILD_TESTING)
  ADD_DEPENDENCIES( "${target_name}_tests"
    openstudio_analysisdriver_resources
    openstudio_utilities_resources
  )
ENDIF ()

