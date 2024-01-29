set(target_name openstudio_gltf)

set(${target_name}_src
        mainpage.hpp
        GltfForwardTranslator.hpp
        GltfForwardTranslator.cpp
        GltfBoundingBox.hpp
        GltfBoundingBox.cpp
        GltfModelObjectMetaData.hpp
        GltfModelObjectMetaData.cpp
        GltfUserData.hpp
        GltfUserData.cpp
        GltfMetaData.hpp
        GltfMetaData.cpp
        GltfMaterialData.hpp
        GltfMaterialData.cpp
        GltfUtils.hpp
        GltfUtils.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/../OpenStudio.hxx
        )

set(${target_name}_test_src
        test/GltfFixture.hpp
        test/GltfFixture.cpp
        test/GltfForwardTranslator_GTest.cpp
        )

set(${target_name}_swig_src
        Gltf.i
        )

add_library(${target_name}
        OBJECT
        ${${target_name}_src}
        )

set(${target_name}_depends
  ${openstudio_model_depends}
)
set(${target_name}_depends ${${target_name}_depends} PARENT_SCOPE)

target_compile_definitions(${target_name} PRIVATE openstudio_gltf_EXPORTS SHARED_OS_LIBS)
target_link_libraries(${target_name} PUBLIC ${${target_name}_depends})
target_link_libraries(${target_name} PRIVATE TinyGLTF::TinyGLTF)

add_dependencies(${target_name} GenerateIddFactoryRun)

set(${target_name}_test_depends
        openstudiolib
        )

CREATE_SRC_GROUPS("${${target_name}_src}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_SRC_GROUPS("${${target_name}_swig_src}")

if (BUILD_TESTING)

  CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}")
  # TODO
  #add_dependencies(${target_name}_tests
  #  openstudio_gltf_resources
  #)
endif ()

MAKE_SWIG_TARGET(OpenStudioGltf Gltf "${CMAKE_CURRENT_SOURCE_DIR}/Gltf.i" "${${target_name}_swig_src}" ${target_name} OpenStudioModel)
