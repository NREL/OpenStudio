set(target_name openstudio_osversion)

set(${target_name}_src
  mainpage.hpp
  OSVersionAPI.hpp
  VersionTranslator.hpp
  VersionTranslator.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/../OpenStudio.hxx
)

set(${target_name}_test_src
  test/OSVersionFixture.hpp
  test/OSVersionFixture.cpp
  test/VersionTranslator_GTest.cpp
)

set(${target_name}_swig_src
  OSVersion.i
)

set(${target_name}_depends
  openstudio_model
)

add_library(${target_name} SHARED
  ${${target_name}_src}
)

AddPCH(${target_name})

target_link_libraries(${target_name} ${${target_name}_depends})

if(NOT APPLE)
  install(TARGETS ${target_name}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
  )
endif()

set(${target_name}_test_depends
  ${${target_name}_depends}
)

CREATE_SRC_GROUPS("${${target_name}_src}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}")
if(BUILD_TESTING)

  add_dependencies("${target_name}_tests"
    openstudio_osversion_resources
  )

endif()

MAKE_SWIG_TARGET(OpenStudioOSVersion OSVersion "${CMAKE_CURRENT_SOURCE_DIR}/OSVersion.i" "${${target_name}_swig_src}" ${target_name} OpenStudioModel)
