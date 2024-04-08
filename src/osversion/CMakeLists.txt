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

add_library(${target_name}
  OBJECT
  ${${target_name}_src}
)

set(${target_name}_depends
  ${openstudio_model_depends}
)
set(${target_name}_depends ${${target_name}_depends} PARENT_SCOPE)

target_compile_definitions(${target_name} PRIVATE openstudio_osversion_EXPORTS SHARED_OS_LIBS)
target_link_libraries(${target_name} PUBLIC ${${target_name}_depends})
add_dependencies(${target_name} GenerateIddFactoryRun)

set(${target_name}_test_depends
  openstudiolib
)

CREATE_SRC_GROUPS("${${target_name}_src}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

if(BUILD_TESTING)

  # TODO: this isn't working perfectly yet...

  CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}")

  # Glob all OSM/OSC files in test/ folder
  file(GLOB_RECURSE osversion_source_models "${CMAKE_CURRENT_SOURCE_DIR}/test/**/*.osm")
  file(GLOB_RECURSE osversion_source_oscs   "${CMAKE_CURRENT_SOURCE_DIR}/test/**/*.osc")
  list (APPEND osversion_source_models ${osversion_source_oscs})

  # Copy them to <build_dir>resources/osversion/
  foreach( _FILE ${osversion_source_models} )
    file(RELATIVE_PATH REL_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/test/" ${_FILE})
    set(TARGET_PATH "${PROJECT_BINARY_DIR}/resources/osversion/${REL_LOCATION}")
    add_custom_command(
      OUTPUT "${TARGET_PATH}"
      COMMAND ${CMAKE_COMMAND} -E copy_if_different "${_FILE}" "${TARGET_PATH}"
      DEPENDS "${_FILE}"
    )
   list(APPEND osversion_test_models ${TARGET_PATH})
  endforeach()

  # add target
  add_custom_target(openstudio_osversion_test_resources
    DEPENDS ${osversion_test_models}
    SOURCES ${osversion_test_models}
    )

  add_dependencies(${target_name}_tests
    openstudio_osversion_test_resources
    # openstudio_osversion_resources
  )

endif()

if(BUILD_BENCHMARK)

  set(${target_name}_benchmark_src
    benchmark/VersionTranslation_Benchmark.cpp
  )

  foreach( bench_file ${${target_name}_benchmark_src} )
    get_filename_component(bench_name ${bench_file} NAME_WE)
    message("bench_name=${bench_name}")
    add_executable( ${bench_name} ${bench_file} )
    target_link_libraries(${bench_name}
      benchmark::benchmark_main
      openstudiolib
    )
    set_target_properties(${bench_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/benchmark")
    add_dependencies(run_benchmarks ${bench_name})
  endforeach()

endif()

MAKE_SWIG_TARGET(OpenStudioOSVersion OSVersion "${CMAKE_CURRENT_SOURCE_DIR}/OSVersion.i" "${${target_name}_swig_src}" ${target_name} OpenStudioModel)
