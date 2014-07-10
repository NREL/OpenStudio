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

add_library(${target_name} SHARED ${${target_name}_src})
target_link_libraries(${target_name} ${${target_name}_depends})
AddPCH(${target_name})

if(NOT APPLE)
  install(TARGETS ${target_name}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
  )
endif()

CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_depends}")

MAKE_SWIG_TARGET(OpenStudioRadiance radiance "${CMAKE_CURRENT_SOURCE_DIR}/Radiance.i" "${${target_name}_swig_src}" ${target_name} OpenStudioModel)
