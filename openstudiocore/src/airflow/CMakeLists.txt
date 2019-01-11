include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(target_name openstudio_airflow)

set(${target_name}_src
  AirflowAPI.hpp
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
include("${CMAKE_SOURCE_DIR}/embedded/EmbedFiles.cmake")
# EMBEDDED_OUTPUT stores the list of cxx files that we need to link later
file(GLOB_RECURSE IDD_FILES  FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/templates/*.prj")
foreach( _FILE ${IDD_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_SOURCE_DIR}" ${_FILE})
  list(APPEND E_FILES ${_FILE})
  list(APPEND E_PATHS ${LOCATION})
endforeach()

include("${CMAKE_SOURCE_DIR}/embedded/EmbedFiles.cmake")
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

# Someone should make sure these are all really needed
#set(${target_name}_depends
#  ${Boost_LIBRARIES}
#  ${CMAKE_THREAD_LIBS}
#  openstudio_utilities
#  openstudio_model
#  openstudio_osversion
#)

#add_library(${target_name}
#  ${${target_name}_src}
#  ${EMBEDDED_OUTPUT}
#)

#target_link_libraries(${target_name} ${${target_name}_depends})

#AddPCH(${target_name})

# No need to install static library
#if(NOT APPLE)
#install(TARGETS ${target_name}
#  RUNTIME DESTINATION bin
#  LIBRARY DESTINATION lib
#)
#endif()

#CREATE_SRC_GROUPS("${${target_name}_src}")
#CREATE_SRC_GROUPS("${${target_name}_test_src}")
#CREATE_SRC_GROUPS("${${target_name}_swig_src}")

set(${target_name}_static_depends
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  openstudio_utilities_static
  openstudio_model_static
  openstudio_osversion_static
)

add_library(${target_name}_static
  ${${target_name}_src}
  ${EMBEDDED_OUTPUT}
)
target_link_libraries(${target_name}_static ${${target_name}_static_depends})

if(BUILD_TESTING)
  CREATE_TEST_TARGETS(${target_name}_static "${${target_name}_test_src}" "${${target_name}_static_depends}")
  add_dependencies(${target_name}_static_tests openstudio_airflow_resources)
endif()

MAKE_SWIG_TARGET(OpenStudioAirflow Airflow "${CMAKE_CURRENT_SOURCE_DIR}/Airflow.i" "${${target_name}_swig_src}" ${target_name}_static OpenStudioEnergyPlus)
