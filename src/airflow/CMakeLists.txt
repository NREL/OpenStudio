set(target_name openstudio_airflow)

set(${target_name}_src
  AirflowAPI.hpp
  contam/ContamEnums.hpp
  contam/ForwardTranslator.hpp
  contam/ForwardTranslator.cpp
  contam/PrjReader.hpp
  contam/PrjReader.cpp
  contam/SimFile.hpp
  contam/SimFile.cpp
  WindPressure.hpp
  WindPressure.cpp
  contam/PrjDefines.hpp
  contam/PrjModel.hpp
  contam/PrjModel.cpp
  contam/PrjModelImpl.hpp
  contam/PrjModelImpl.cpp
  contam/PrjObjects.hpp
  contam/PrjObjects.cpp
  contam/PrjObjectsImpl.hpp
  contam/PrjObjectsImpl.cpp
  contam/PrjSubobjects.hpp
  contam/PrjSubobjects.cpp
  contam/PrjSubobjectsImpl.hpp
  contam/PrjSubobjectsImpl.cpp
  contam/PrjAirflowElements.hpp
  contam/PrjAirflowElements.cpp
  contam/PrjAirflowElementsImpl.hpp
  contam/PrjAirflowElementsImpl.cpp
  SurfaceNetworkBuilder.hpp
  SurfaceNetworkBuilder.cpp
)

# resource files
include("${PROJECT_SOURCE_DIR}/embedded/EmbedFiles.cmake")
# EMBEDDED_OUTPUT stores the list of cxx files that we need to link later
file(GLOB_RECURSE IDD_FILES  FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/templates/*.prj")
foreach( _FILE ${IDD_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_SOURCE_DIR}" ${_FILE})
  list(APPEND E_FILES ${_FILE})
  list(APPEND E_PATHS ${LOCATION})
endforeach()

include("${PROJECT_SOURCE_DIR}/embedded/EmbedFiles.cmake")
embed_files("${E_FILES}" "${E_PATHS}" EMBEDDED_OUTPUT openstudiocontam)
# message("EMBEDDED_OUTPUT: ${EMBEDDED_OUTPUT}")

# set up groups of source files for Visual Studio
source_group(embedded FILES ${EMBEDDED_OUTPUT})


set(${target_name}_test_src
  Test/AirflowFixture.hpp
  Test/AirflowFixture.cpp
  Test/ContamModel_GTest.cpp
  Test/ForwardTranslator_GTest.cpp
  Test/SurfaceNetworkBuilder_GTest.cpp
  Test/DemoModel.hpp
  Test/DemoModel.cpp
)

set(${target_name}_swig_src
  Airflow.i
)

set(${target_name}_depends
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${openstudio_utilities_depends}
  ${openstudio_model_depends}
  ${openstudio_osversion_depends}
)
set(${target_name}_depends ${${target_name}_depends} PARENT_SCOPE)

add_library(${target_name}
  OBJECT
  ${${target_name}_src}
  ${EMBEDDED_OUTPUT}
)

target_link_libraries(${target_name} PUBLIC ${${target_name}_depends})
target_compile_definitions(${target_name} PRIVATE openstudio_airflow_EXPORTS SHARED_OS_LIBS)
target_include_directories(${target_name} PUBLIC ${PROJECT_BINARY_DIR})
add_dependencies(${target_name} GenerateIddFactoryRun)


set(${target_name}_test_depends
  openstudiolib
)


if(BUILD_TESTING)
  CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}")
  add_dependencies(${target_name}_tests openstudio_airflow_resources)
endif()

MAKE_SWIG_TARGET(OpenStudioAirflow Airflow "${CMAKE_CURRENT_SOURCE_DIR}/Airflow.i" "${${target_name}_swig_src}" ${target_name} OpenStudioEnergyPlus)
