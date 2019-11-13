set(target_name openstudio_isomodel)

set(${target_name}_src
  ISOModelAPI.hpp
  mainpage.hpp
  ForwardTranslator.hpp
  ForwardTranslator.cpp
  SimModel.hpp
  SimModel.cpp
  UserModel.hpp
  UserModel.cpp
  Building.cpp
  Cooling.cpp
  Heating.cpp
  Lighting.cpp
  Location.cpp
  Population.cpp
  Structure.cpp
  Ventilation.cpp
  WeatherData.cpp
  Building.hpp
  Cooling.hpp
  Heating.hpp
  Lighting.hpp
  Location.hpp
  Population.hpp
  Structure.hpp
  Ventilation.hpp
  WeatherData.hpp
  EpwData.hpp
  EpwData.cpp
  SolarRadiation.hpp
  SolarRadiation.cpp
  TimeFrame.hpp
  TimeFrame.cpp
)

set(${target_name}_test_src
  Test/ISOModelFixture.hpp
  Test/ISOModelFixture.cpp
  Test/ForwardTranslator_GTest.cpp
  Test/SimModel_GTest.cpp
  Test/UserModel_GTest.cpp
)

set(${target_name}_swig_src
  ISOModel.i
)

set(${target_name}_depends
  ${openstudio_model_depends}
)
set(${target_name}_depends ${${target_name}_depends} PARENT_SCOPE)

add_library(${target_name} OBJECT ${${target_name}_src})
target_link_libraries(${target_name} PUBLIC ${${target_name}_depends})
target_compile_definitions(${target_name} PRIVATE openstudio_isomodel_EXPORTS SHARED_OS_LIBS)
add_dependencies(${target_name} GenerateIddFactoryRun)


set(${target_name}_test_depends
  openstudiolib
)

CREATE_SRC_GROUPS("${${target_name}_src}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

if(BUILD_TESTING)
  CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}")
  add_dependencies(${target_name}_tests openstudio_isomodel_resources)
endif()

MAKE_SWIG_TARGET(OpenStudioISOModel ISOModel "${CMAKE_CURRENT_SOURCE_DIR}/ISOModel.i" "${${target_name}_swig_src}" ${target_name} OpenStudioModel)

