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
set(${target_name}_qrc
  contam.qrc
)

# generate rules for building source files from the resources
qt5_add_resources(${target_name}_qrcs ${${target_name}_qrc})

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
set(${target_name}_depends
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${QT_LIBS}
  openstudio_utilities
  openstudio_model
  openstudio_osversion
  #zkexpat
)

add_library(${target_name} STATIC
  ${${target_name}_src}
  ${${target_name}_qrcs}
)

target_link_libraries(${target_name} ${${target_name}_depends})

AddPCH(${target_name})

# No need to install static library
#if(NOT APPLE)
#install(TARGETS ${target_name}
#  RUNTIME DESTINATION bin
#  LIBRARY DESTINATION lib
#)
#endif()

CREATE_SRC_GROUPS("${${target_name}_src}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_depends}")
if(BUILD_TESTING)
  add_dependencies(${target_name}_tests openstudio_airflow_resources)
endif()

MAKE_SWIG_TARGET(OpenStudioAirflow Airflow "${CMAKE_CURRENT_SOURCE_DIR}/Airflow.i" "${${target_name}_swig_src}" ${target_name} OpenStudioEnergyPlus)
