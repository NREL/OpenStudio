set(target_name openstudio_alfalfa)

include_directories(${CMAKE_CURRENT_BINARY_DIR}/)


set(${target_name}_src
  AlfalfaAPI.hpp
  AlfalfaComponentBase.cpp
  AlfalfaComponentBase.hpp
  AlfalfaComponent.hpp
  AlfalfaComponent.cpp
  AlfalfaConstant.hpp
  AlfalfaConstant.cpp
  AlfalfaActuator.hpp
  AlfalfaActuator.cpp
  AlfalfaGlobalVariable.hpp
  AlfalfaGlobalVariable.cpp
  AlfalfaMeter.hpp
  AlfalfaMeter.cpp
  AlfalfaOutputVariable.hpp
  AlfalfaOutputVariable.cpp
  AlfalfaPoint.hpp
  AlfalfaPoint_Impl.hpp
  AlfalfaPoint.cpp
  AlfalfaJSON.hpp
  AlfalfaJSON_Impl.hpp
  AlfalfaJSON.cpp
)

set(${target_name}_swig_src
  Alfalfa.i
)

set(${target_name}_test_src
  test/AlfalfaJSON_GTest.cpp
)

CREATE_SRC_GROUPS("${${target_name}_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

set(${target_name}_depends
  ${openstudio_model_depends}
)

set(${target_name}_depends ${${target_name}_depends} PARENT_SCOPE)

add_library(${target_name}
  OBJECT
  ${${target_name}_src}
)

target_link_libraries(${target_name} PUBLIC ${${target_name}_depends})
target_compile_definitions(${target_name} PRIVATE openstudio_alfalfa_EXPORTS SHARED_OS_LIBS)
add_dependencies(${target_name} GenerateIddFactoryRun)

if(BUILD_TESTING)
  CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" openstudiolib)
endif()


MAKE_SWIG_TARGET(OpenStudioAlfalfa alfalfa "${CMAKE_CURRENT_SOURCE_DIR}/Alfalfa.i" "${${target_name}_swig_src}" ${target_name} OpenStudioOSVersion)
