#only building C# bindings with Visual Studio

#set(ALL_CSHARP_WRAPPER_FILES
#  C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesCore_wrap.cxx
#  C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesIdd_wrap.cxx
#  C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesIdf_wrap.cxx
#  C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesBCL_wrap.cxx
#  C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesData_wrap.cxx
#  C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesGeometry_wrap.cxx
#  C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesSql_wrap.cxx
#  C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesTime_wrap.cxx
#  C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesUnits_wrap.cxx
#  C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesFileTypes_wrap.cxx
#  C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilities_wrap.cxx
#  C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelCore_wrap.cxx
#  C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelSimulation_wrap.cxx
#  C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelResources_wrap.cxx
#  C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelGeometry_wrap.cxx
#  C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelHVAC_wrap.cxx
#  C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelRefrigeration_wrap.cxx
#  C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelGenerators_wrap.cxx
#  C:/OpenStudio/build-32/src/model/csharp_OpenStudioModel_wrap.cxx
#  C:/OpenStudio/build-32/src/energyplus/csharp_OpenStudioEnergyPlus_wrap.cxx
#  C:/OpenStudio/build-32/src/gbxml/csharp_OpenStudioGBXML_wrap.cxx
#  C:/OpenStudio/build-32/src/airflow/csharp_OpenStudioAirflow_wrap.cxx
#  C:/OpenStudio/build-32/src/osversion/csharp_OpenStudioOSVersion_wrap.cxx
#  C:/OpenStudio/build-32/src/sdd/csharp_OpenStudioSDD_wrap.cxx
#)



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

file(TO_NATIVE_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp" CSHARP_LIBRARY_OUTPUT_DIRECTORY)
file(TO_NATIVE_PATH "${PROJECT_SOURCE_DIR}/csharp" CSHARP_SOURCE_DIR)
file(TO_NATIVE_PATH "${PROJECT_BINARY_DIR}/csharp_wrapper" CSHARP_BINARY_DIR)

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
configure_file("${PROJECT_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj_dotnet.in" "${PROJECT_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj")

# Trying to get rid of this by placing more attributes in csproj instead
if (MSVC)
  configure_file("${PROJECT_SOURCE_DIR}/csharp/developer/OpenStudio/Properties/AssemblyVersion.cs.in" "${PROJECT_BINARY_DIR}/csharp_wrapper/generated_sources/AssemblyVersion.cs")
endif()

#This is generating odd errors about needing to convert the included project file, but it seems like it would be a good
#way to move forward, if it worked. Leaving here as a note.
#include_external_msproject(OpenStudio "${PROJECT_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj" ${CSHARP_LIBRARIES})
#in lieu of the above, we are using the following:

# the main OpenStudio dll
set(OPENSTUDIO_CSHARP_DLL "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/$(ConfigurationName)/OpenStudio.dll")
set(CONFIG_ARGS "/p:Configuration=$(ConfigurationName)")

if(NINJA)
  set(CONFIG_ARGS "/p:Configuration=${CMAKE_BUILD_TYPE}")
  set(OPENSTUDIO_CSHARP_DLL "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/OpenStudio.dll")
endif()


if (MSVC)
  # TODO: Remove
  # set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /FORCE:MULTIPLE")

  # ignore deprecated warnings since we will export deprecated methods
  add_definitions(/wd4996 /bigobj)

  # custom command to make OPENSTUDIO_CSHARP_DLL
  add_custom_command(
    OUTPUT ${OPENSTUDIO_CSHARP_DLL}
    COMMAND "MSBuild" "${PROJECT_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj" "${CONFIG_ARGS}"
    DEPENDS ${ALL_CSHARP_WRAPPER_TARGETS} ${ALL_CSHARP_WRAPPER_FILES}
  )
else()
  # Rely on dotnet Core (open source)
  find_program(DOTNET dotnet)
  if(NOT DOTNET)
    message(FATAL_ERROR "C# generation outside of MSVC requires .NET Core (dotnet)")
  endif()

  # ignore deprecated warnings since we will export deprecated methods
  add_definitions(-Wno-deprecated-declarations)

  # custom command to make OPENSTUDIO_CSHARP_DLL
  add_custom_command(
    OUTPUT ${OPENSTUDIO_CSHARP_DLL}
    COMMAND "${DOTNET}" "build" "-c" "$<CONFIGURATION>" "-f" "netcoreapp3.0" "${PROJECT_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj"
    DEPENDS ${ALL_CSHARP_WRAPPER_TARGETS} ${ALL_CSHARP_WRAPPER_FILES}
  )
endif()

install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Debug/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Release/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

# keep the following lists aligned with translator_names in \openstudiocore\ProjectMacros.cmake
set(translator_wrappers
  csharp_OpenStudioAirflow_wrap.cxx
  csharp_OpenStudioEnergyPlus_wrap.cxx
  csharp_OpenStudioGBXML_wrap.cxx
  csharp_OpenStudioISOModel_wrap.cxx
  csharp_OpenStudioRadiance_wrap.cxx
  csharp_OpenStudioSDD_wrap.cxx
)

set(model_wrappers
  csharp_OpenStudioMeasure_wrap.cxx
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
  MODULE
  ${core_wrapper_files}
)
add_dependencies(openstudio_csharp ${ALL_CSHARP_WRAPPER_TARGETS})


# TODO: Remove
# DLM: currently there are some multiply defined symbols, force them for now, investigate later

# Note JM 2019-10-07:
# eg: multiple definition of `CSharp_OpenStudio_LeftShift__SWIG_1'
# This is due to the ostream << operators

# Couldn't make it work with target_link_options, nor with generator expressions in target_link_libraries.
# Only thing that worked was explicit `target_link_libraries(openstudio_XXX PUBLIC "-Wl,--allow-multiple-definition" ...)` for each of the three libraries

#target_link_options(openstudio_csharp
#   PUBLIC
#   $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:"LINKER:--allow-multiple-definition">
#   $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:"-Wl,--allow-multiple-definition">
#   $<$<CXX_COMPILER_ID:MSVC>:"/FORCE:MULTIPLE">
#)

target_link_libraries(openstudio_csharp
 openstudiolib
)
install(TARGETS openstudio_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
install(TARGETS openstudio_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

add_library(
  openstudio_model_csharp
  MODULE
  ${model_wrapper_files}
)
add_dependencies(openstudio_model_csharp ${ALL_CSHARP_WRAPPER_TARGETS})

target_link_libraries(openstudio_model_csharp
  openstudiolib
)
install(TARGETS openstudio_model_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
install(TARGETS openstudio_model_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

add_library(
  openstudio_translators_csharp
  MODULE
  ${translator_wrapper_files}
)
add_dependencies(openstudio_translators_csharp ${ALL_CSHARP_WRAPPER_TARGETS})
target_link_libraries(openstudio_translators_csharp
  openstudiolib
)
install(TARGETS openstudio_translators_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
install(TARGETS openstudio_translators_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

# convenience target to build the SDK
add_custom_target(csharp_sdk ALL
  DEPENDS ${OPENSTUDIO_CSHARP_DLL} openstudio_csharp openstudio_model_csharp openstudio_translators_csharp
)

if( BUILD_NUGET_PACKAGE )

  # TODO: move in MSVC block, remove altogether? For dotnet I'm trying to use the nuget properties metadata directly in csproj
  configure_file( ${PROJECT_SOURCE_DIR}/OpenStudio.nuspec.in ${PROJECT_BINARY_DIR}/OpenStudio.nuspec )

  if (MSVC)

    add_custom_target(nuget_package
      COMMAND NuGet pack OpenStudio.nuspec -BasePath "${PROJECT_BINARY_DIR}/" -properties Configuration=$<CONFIGURATION>
      DEPENDS csharp_sdk
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}"
    )
  else()
    #dotnet pack OpenStudio.csproj --configuration $<CONFIG> -p:NuspecFile="${PROJECT_BINARY_DIR}/OpenStudio.nuspec" -p:NuspectProperties="Configuration=$<CONFIG>"
    # dotnet pack <path to .csproj file> -p:NuspecFile=<path to nuspec file> -p:NuspecProperties=<> -p:NuspecBasePath=<Base path>
    add_custom_target(nuget_package
      COMMAND ${DOTNET} pack --configuration $<CONFIG> -p:TargetFrameworks=netcoreapp3.0 ${PROJECT_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj
      DEPENDS csharp_sdk
      WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/csharp_wrapper/"
    )
  endif()

endif()
