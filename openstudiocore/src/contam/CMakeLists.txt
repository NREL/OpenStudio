set(target_name openstudio_contam)

set(${target_name}_src
  ContamAPI.hpp
  ForwardTranslator.hpp
  ForwardTranslator.cpp
  PrjReader.hpp
  PrjReader.cpp
  SimFile.hpp
  SimFile.cpp
  WindPressure.hpp
  WindPressure.cpp
  PrjDefines.hpp
  PrjModel.hpp
  PrjModel.cpp
  PrjModelImpl.hpp
  PrjModelImpl.cpp
  PrjObjects.hpp
  PrjObjects.cpp
  PrjObjectsImpl.hpp
  PrjObjectsImpl.cpp
  PrjSubobjects.hpp
  PrjSubobjects.cpp
  PrjSubobjectsImpl.hpp
  PrjSubobjectsImpl.cpp
  PrjAirflowElements.hpp
  PrjAirflowElements.cpp
  PrjAirflowElementsImpl.hpp
  PrjAirflowElementsImpl.cpp
)

# resource files
set(${target_name}_qrc
  contam.qrc
)

# generate rules for building source files from the resources
qt5_add_resources(${target_name}_qrcs ${${target_name}_qrc})

set(${target_name}_test_src
  Test/ContamFixture.hpp
  Test/ContamFixture.cpp
  Test/ForwardTranslator_GTest.cpp
)

set(${target_name}_swig_src
  Contam.i
)

# Someone should make sure these are all really needed
set(${target_name}_depends
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${QT_LIBS}
  openstudio_utilities
  openstudio_model
  openstudio_osversion
  zkexpat
)

add_library(${target_name} SHARED
  ${${target_name}_src}
  ${${target_name}_qrcs}
)

target_link_libraries(${target_name} ${${target_name}_depends})

AddPCH(${target_name})

#if(NOT APPLE)
install(TARGETS ${target_name}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)
#endif()

CREATE_SRC_GROUPS("${${target_name}_src}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_depends}")
if(BUILD_TESTING)
  add_dependencies(${target_name}_tests openstudio_contam_resources)
endif()

MAKE_SWIG_TARGET(OpenStudioContam Contam "${CMAKE_CURRENT_SOURCE_DIR}/Contam.i" "${${target_name}_swig_src}" ${target_name} OpenStudioEnergyPlus)
