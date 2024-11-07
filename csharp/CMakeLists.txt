
install(DIRECTORY "examples"
  DESTINATION "CSharp/"
  COMPONENT "CSharpAPI"
  FILES_MATCHING PATTERN "*.cs"
  PATTERN "*.csproj"
  PATTERN "*.sln"
  PATTERN "*.resx"
  PATTERN "*.settings"
  PATTERN "*.docx"
)

set(CSHARP_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp")

# For csproj.in
file(TO_NATIVE_PATH "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}" CSHARP_LIBRARY_OUTPUT_DIRECTORY_NATIVE)
file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/csharp" CSHARP_SOURCE_DIR_NATIVE)

set(CSHARP_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CSHARP_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CSHARP_VERSION_PATCH ${PROJECT_VERSION_PATCH_DIGIT})
set(CSHARP_VERSION_BUILD 0)

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(CSHARP_PLATFORM "x64")
else()
  set(CSHARP_PLATFORM "x86")
endif()

set(CSHARP_ASSEMBLY_VERSION "${CSHARP_VERSION_MAJOR}.${CSHARP_VERSION_MINOR}.${CSHARP_VERSION_PATCH}.${CSHARP_VERSION_BUILD}")

set(CSHARP_TOOLS_VERSION "4.5")

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/csharp_wrapper/generated_sources")
configure_file("${PROJECT_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj.in" "${PROJECT_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj")


#This is generating odd errors about needing to convert the included project file, but it seems like it would be a good
#way to move forward, if it worked. Leaving here as a note.
#include_external_msproject(OpenStudio "${PROJECT_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj" ${CSHARP_LIBRARIES})
#in lieu of the above, we are using the following:

# the main OpenStudio.dll generated

# Rely on dotnet Core (open source)
find_program(DOTNET dotnet)
if(NOT DOTNET)
  message(FATAL_ERROR "C# generation outside of MSVC requires .NET Core (dotnet)")
endif()

# The Release/Debug can be stripped probably
# Also probably don't need to use multiple `<TargetFrameworks>` which is the reason there's an extra netstandard2.0/ level
# Note: can't use CMAKE_BUILD_TYPE as it's empty for multi generators (MSVC for eg). So we use a genex
# We use the non-native, cmake_path, version of library output directory otherwise install fails with mistmatched `/` and `\` on windows
set(OPENSTUDIO_CSHARP_DLL "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/$<CONFIG>/netstandard2.0/OpenStudio.dll")

# ignore deprecated warnings since we will export deprecated methods
add_library(csharp_warnings INTERFACE)
target_compile_options(csharp_warnings INTERFACE "$<${is_msvc_genex}:/wd4996>")
target_compile_options(csharp_warnings INTERFACE "$<${is_msvc_genex}:/bigobj>")
target_compile_options(csharp_warnings INTERFACE "$<${is_gnu_or_clang_genex}:-Wno-deprecated-declarations>")
target_compile_options(csharp_warnings INTERFACE "$<${is_gnu_or_clang_genex}:-Wno-unused-function>")

# custom command to make OPENSTUDIO_CSHARP_DLL
add_custom_command(
  OUTPUT ${OPENSTUDIO_CSHARP_DLL}
  COMMAND "${DOTNET}" "build" "-c" "$<CONFIG>" "/p:Platform=${CSHARP_PLATFORM}" "${PROJECT_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj"
  DEPENDS ${ALL_CSHARP_WRAPPER_TARGETS} ${ALL_CSHARP_WRAPPER_FILES}
)

install(FILES "${OPENSTUDIO_CSHARP_DLL}" DESTINATION CSharp/openstudio/ COMPONENT "CSharpAPI")

# keep the following lists aligned with translator_names in ProjectMacros.cmake
set(translator_wrappers
  csharp_OpenStudioAirflow_wrap.cxx
  csharp_OpenStudioEnergyPlus_wrap.cxx
  csharp_OpenStudioGBXML_wrap.cxx
  csharp_OpenStudioGltf_wrap.cxx
  csharp_OpenStudioISOModel_wrap.cxx
  csharp_OpenStudioRadiance_wrap.cxx
  csharp_OpenStudioSDD_wrap.cxx
)

set(model_wrappers
  csharp_OpenStudioMeasure_wrap.cxx
  csharp_OpenStudioAlfalfa_wrap.cxx
  csharp_OpenStudioModel_wrap.cxx
  csharp_OpenStudioModelAirflow_wrap.cxx
  csharp_OpenStudioModelAvailabilityManager_wrap.cxx
  csharp_OpenStudioModelCore_wrap.cxx
  csharp_OpenStudioModelGenerators_wrap.cxx
  csharp_OpenStudioModelGeometry_wrap.cxx
  csharp_OpenStudioModelHVAC_wrap.cxx
  csharp_OpenStudioModelPlantEquipmentOperationScheme_wrap.cxx
  csharp_OpenStudioModelRefrigeration_wrap.cxx
  csharp_OpenStudioModelResources_wrap.cxx
  csharp_OpenStudioModelSetpointManager
  csharp_OpenStudioModelSimulation_wrap.cxx
  csharp_OpenStudioModelStraightComponent_wrap.cxx
  csharp_OpenStudioModelZoneHVAC_wrap.cxx
  csharp_OpenStudioOSVersion_wrap.cxx
)

set(utilities_wrappers
  csharp_OpenStudioEnergyPlus_wrap.cxx
  csharp_OpenStudioRadiance_wrap.cxx
  csharp_OpenStudioGBXML_wrap.cxx
  csharp_OpenStudioGltf_wrap.cxx
  csharp_OpenStudioAirflow_wrap.cxx
  csharp_OpenStudioISOModel_wrap.cxx
  csharp_OpenStudioSDD_wrap.cxx
)

foreach(wrapper_file IN LISTS ALL_CSHARP_WRAPPER_FILES)
  get_filename_component(wrapper_name ${wrapper_file} NAME)

  set(notfound TRUE)

  foreach(model_wrapper IN LISTS model_wrappers)
    if( wrapper_name STREQUAL model_wrapper )
      list(APPEND model_wrapper_files ${wrapper_file})
      set(notfound FALSE)
      break()
    endif()
  endforeach()

  foreach(translator_wrapper IN LISTS translator_wrappers)
    if( wrapper_name STREQUAL translator_wrapper )
      list(APPEND translator_wrapper_files ${wrapper_file})
      set(notfound FALSE)
      break()
    endif()
  endforeach()

  if( notfound )
    list(APPEND core_wrapper_files ${wrapper_file})
  endif()
endforeach()

#message("model_wrapper_files = ${model_wrapper_files}")
#message("translator_wrapper_files = ${translator_wrapper_files}")
#message("core_wrapper_files = ${core_wrapper_files}")

set_source_files_properties(${ALL_CSHARP_WRAPPER_FILES} PROPERTIES GENERATED TRUE)

add_library(
  openstudio_csharp
  SHARED # Was "MODULE" on Windows before
  ${core_wrapper_files}
)
add_dependencies(openstudio_csharp ${ALL_CSHARP_WRAPPER_TARGETS})


# TODO: Remove
# DLM: currently there are some multiply defined symbols, force them for now, investigate later

# Note JM 2019-10-07:
# eg: multiple definition of `CSharp_OpenStudio_LeftShift__SWIG_1'
# This is due to the ostream << operators



target_link_libraries(openstudio_csharp
  PRIVATE
    openstudiolib
    csharp_warnings
)
install(TARGETS openstudio_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
install(TARGETS openstudio_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

add_library(
  openstudio_model_csharp
  SHARED # Was "MODULE" on Windows before
  ${model_wrapper_files}
)
add_dependencies(openstudio_model_csharp ${ALL_CSHARP_WRAPPER_TARGETS})
target_link_libraries(openstudio_model_csharp
  PRIVATE
    openstudiolib
    csharp_warnings
)
install(TARGETS openstudio_model_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
install(TARGETS openstudio_model_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

add_library(
  openstudio_translators_csharp
  SHARED # Was "MODULE" on Windows before
  ${translator_wrapper_files}
)
add_dependencies(openstudio_translators_csharp ${ALL_CSHARP_WRAPPER_TARGETS})
target_link_libraries(openstudio_translators_csharp
  PRIVATE
    openstudiolib
    csharp_warnings
)
install(TARGETS openstudio_translators_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
install(TARGETS openstudio_translators_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

# convenience target to build the SDK
add_custom_target(csharp_sdk ALL
  DEPENDS ${OPENSTUDIO_CSHARP_DLL} openstudio_csharp openstudio_model_csharp openstudio_translators_csharp
)
