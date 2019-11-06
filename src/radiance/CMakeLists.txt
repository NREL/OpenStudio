set(target_name openstudio_radiance)

set(${target_name}_src
  RadianceAPI.hpp
  mainpage.hpp
  AnnualIlluminanceMap.hpp
  AnnualIlluminanceMap.cpp
  HeaderInfo.hpp
  HeaderInfo.cpp
  ForwardTranslator.hpp
  ForwardTranslator.cpp
  LightFixture.hpp
  LightFixture.cpp
  MaterialProperties.hpp
  MaterialProperties.cpp
  Photosensor.hpp
  Photosensor.cpp
  Renderer.hpp
  Renderer.cpp
  Utils.hpp
  Utils.cpp
  WindowGroup.hpp
  WindowGroup.cpp
)


set(${target_name}_test_src
  Test/AnnualIlluminanceMap_GTest.cpp
  Test/ForwardTranslator_GTest.cpp
)

set(${target_name}_depends
  openstudio_model
)

set(${target_name}_swig_src
  Radiance.i
  HeaderInfo.i
  AnnualIlluminanceMap.i
)

# resource files
include("${PROJECT_SOURCE_DIR}/embedded/EmbedFiles.cmake")
# EMBEDDED_OUTPUT stores the list of cxx files that we need to link later
file(GLOB_RECURSE IDD_FILES  FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.xml")
foreach( _FILE ${IDD_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_SOURCE_DIR}" ${_FILE})
  list(APPEND E_FILES ${_FILE})
  list(APPEND E_PATHS ${LOCATION})
endforeach()

include("${PROJECT_SOURCE_DIR}/embedded/EmbedFiles.cmake")
embed_files("${E_FILES}" "${E_PATHS}" EMBEDDED_OUTPUT openstudioradiance)
# message("EMBEDDED_OUTPUT: ${EMBEDDED_OUTPUT}")

# set up groups of source files for Visual Studio
source_group(embedded FILES ${EMBEDDED_OUTPUT})


set(${target_name}_depends
  openstudio_model
)

add_library(${target_name} OBJECT ${${target_name}_src} ${EMBEDDED_OUTPUT})
target_compile_definitions(${target_name} PRIVATE openstudio_radiance_EXPORTS)
target_link_libraries(${target_name} PUBLIC ${${target_name}_depends})
target_include_directories(${target_name} PUBLIC ${PROJECT_BINARY_DIR})


# No need to install static library
#if(NOT APPLE)
#  install(TARGETS ${target_name}
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib
#  )
#endif()

CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_depends}")

MAKE_SWIG_TARGET(OpenStudioRadiance radiance "${CMAKE_CURRENT_SOURCE_DIR}/Radiance.i" "${${target_name}_swig_src}" ${target_name} OpenStudioModel)

