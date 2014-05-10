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
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${QT_LIBS}
  openstudio_utilities
  openstudio_model
  zkexpat
)

add_library(${target_name} SHARED ${${target_name}_src})
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
  add_dependencies(${target_name}_tests openstudio_isomodel_resources)
endif()

MAKE_SWIG_TARGET(OpenStudioISOModel ISOModel "${CMAKE_CURRENT_SOURCE_DIR}/ISOModel.i" "${${target_name}_swig_src}" ${target_name} OpenStudioModel)
