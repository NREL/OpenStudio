#only building C# bindings with Visual Studio

#set(ALL_CSHARP_WRAPPER_FILES
#	C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesCore_wrap.cxx
#	C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesIdd_wrap.cxx
#	C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesIdf_wrap.cxx
#	C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesBCL_wrap.cxx
#	C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesData_wrap.cxx
#	C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesGeometry_wrap.cxx
#	C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesSql_wrap.cxx
#	C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesTime_wrap.cxx
#	C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesUnits_wrap.cxx
#	C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilitiesFileTypes_wrap.cxx
#	C:/OpenStudio/build-32/src/utilities/csharp_OpenStudioUtilities_wrap.cxx
#	C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelCore_wrap.cxx
#	C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelSimulation_wrap.cxx
#	C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelResources_wrap.cxx
#	C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelGeometry_wrap.cxx
#	C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelHVAC_wrap.cxx
#	C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelRefrigeration_wrap.cxx
#	C:/OpenStudio/build-32/src/model/csharp_OpenStudioModelGenerators_wrap.cxx
#	C:/OpenStudio/build-32/src/model/csharp_OpenStudioModel_wrap.cxx
#	C:/OpenStudio/build-32/src/energyplus/csharp_OpenStudioEnergyPlus_wrap.cxx
#	C:/OpenStudio/build-32/src/gbxml/csharp_OpenStudioGBXML_wrap.cxx
#	C:/OpenStudio/build-32/src/airflow/csharp_OpenStudioAirflow_wrap.cxx
#	C:/OpenStudio/build-32/src/osversion/csharp_OpenStudioOSVersion_wrap.cxx
#	C:/OpenStudio/build-32/src/sdd/csharp_OpenStudioSDD_wrap.cxx
#)


if(MSVC)

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
  file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/csharp" CSHARP_SOURCE_DIR)
  file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/csharp_wrapper" CSHARP_BINARY_DIR)

  set(CSHARP_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CSHARP_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CSHARP_VERSION_PATCH ${PROJECT_VERSION_PATCH})
  set(CSHARP_VERSION_BUILD 0)
  
  if (CMAKE_CL_64)
    set(CSHARP_PLATFORM "x64")
  else()
    set(CSHARP_PLATFORM "x86")
  endif()

  set(CSHARP_ASSEMBLY_VERSION "${CSHARP_VERSION_MAJOR}.${CSHARP_VERSION_MINOR}.${CSHARP_VERSION_PATCH}.${CSHARP_VERSION_BUILD}")

  set(CSHARP_TOOLS_VERSION "4.5")

  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/csharp_wrapper/generated_sources")
  configure_file("${CMAKE_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj.in" "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj")
  configure_file("${CMAKE_SOURCE_DIR}/csharp/developer/OpenStudio/Properties/AssemblyVersion.cs.in" "${CMAKE_BINARY_DIR}/csharp_wrapper/generated_sources/AssemblyVersion.cs")

  #This is generating odd errors about needing to convert the included project file, but it seems like it would be a good
  #way to move forward, if it worked. Leaving here as a note.
  #include_external_msproject(OpenStudio "${CMAKE_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj" ${CSHARP_LIBRARIES})
  #in lieu of the above, we are using the following:

  # the main OpenStudio dll
  set(OPENSTUDIO_CSHARP_DLL "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/$(ConfigurationName)/OpenStudio.dll")

  # custom command to make OPENSTUDIO_CSHARP_DLL
  add_custom_command(
    OUTPUT ${OPENSTUDIO_CSHARP_DLL}
    COMMAND "MSBuild" "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj" "/p:Configuration=$(ConfigurationName)"
    DEPENDS ${ALL_CSHARP_WRAPPER_TARGETS} ${ALL_CSHARP_WRAPPER_FILES}
  )

  install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Debug/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
  install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Release/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")
  
  # DLM: currently there are some multiply defined symbols, force them for now, investigate later
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /FORCE:MULTIPLE")
  
  # ignore deprecated warnings since we will export deprecated methods
  add_definitions(/wd4996 /bigobj)

  set(translator_wrappers
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

  set_source_files_properties(${ALL_CSHARP_WRAPPER_FILES} PROPERTIES GENERATED TRUE)

  # build a dll to link all the static C# bindings into
  add_library(
    openstudio_csharp
    MODULE
    ${core_wrapper_files}
  )
  add_dependencies(openstudio_csharp ${ALL_CSHARP_WRAPPER_TARGETS})
  target_link_libraries(openstudio_csharp
   ${ALL_CSHARP_BINDING_DEPENDS}
   ${Boost_LIBRARIES}
  )
  install(TARGETS openstudio_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
  install(TARGETS openstudio_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

  add_library(
    openstudio_translators_csharp
    MODULE
    ${translator_wrapper_files}
  )
  add_dependencies(openstudio_translators_csharp ${ALL_CSHARP_WRAPPER_TARGETS})
  target_link_libraries(openstudio_translators_csharp
   ${ALL_CSHARP_BINDING_DEPENDS}
   ${Boost_LIBRARIES}
  )
  install(TARGETS openstudio_translators_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
  install(TARGETS openstudio_translators_csharp DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

  # convenience target to build the SDK
  add_custom_target(csharp_sdk ALL
    DEPENDS ${OPENSTUDIO_CSHARP_DLL} openstudio_csharp openstudio_translators_csharp
  )
  
endif()


