set(target_name openstudio_epjson)

set(${target_name}_src
        mainpage.hpp
        epJSONAPI.hpp
        epJSONTranslator.hpp
        epJSONTranslator.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/../OpenStudio.hxx
        )

set(${target_name}_test_src
        test/epJSONFixture.hpp
        test/epJSONFixture.cpp
        test/epJSONTranslator_GTest.cpp
        )

set(${target_name}_swig_src
        epJSON.i
        )

add_library(${target_name}
        OBJECT
        ${${target_name}_src}
        )

set(${target_name}_depends
        ${openstudio_utilities_depends}
        )
set(${target_name}_depends ${${target_name}_depends} PARENT_SCOPE)

target_compile_definitions(${target_name} PRIVATE openstudio_epjson_EXPORTS SHARED_OS_LIBS)
target_link_libraries(${target_name} PUBLIC ${${target_name}_depends})

add_dependencies(${target_name} GenerateIddFactoryRun)

set(${target_name}_test_depends
        openstudiolib
        )

CREATE_SRC_GROUPS("${${target_name}_src}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

if (BUILD_TESTING)

  CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}")
  target_link_libraries(${target_name}_tests fmt::fmt)

  # Glob all OSM files in test/ folder
#  file(GLOB_RECURSE epjson_source_models "${CMAKE_CURRENT_SOURCE_DIR}/test/**/*.osm")

  # Copy them to <build_dir>resources/epjson/
#  foreach (_FILE ${epjson_source_models})
#    file(RELATIVE_PATH REL_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/test/" ${_FILE})
#    set(TARGET_PATH "${PROJECT_BINARY_DIR}/resources/epjson/${REL_LOCATION}")
#    add_custom_command(
#            OUTPUT "${TARGET_PATH}"
#            COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_FILE}" "${TARGET_PATH}"
#            DEPENDS "${_FILE}"
#    )
#    list(APPEND epjson_test_models ${TARGET_PATH})
#  endforeach ()

  # add target
#  add_custom_target(openstudio_epjson_test_resources
#          DEPENDS ${epjson_test_models}
#          SOURCES ${epjson_test_models}
#          )

#  add_dependencies(${target_name}_tests
#          openstudio_epjson_test_resources
#          # openstudio_epjson_resources
#          )

endif ()

MAKE_SWIG_TARGET(OpenStudioEPJSON EPJSON "${CMAKE_CURRENT_SOURCE_DIR}/epJSON.i" "${${target_name}_swig_src}" ${target_name} OpenStudioModelCore)
