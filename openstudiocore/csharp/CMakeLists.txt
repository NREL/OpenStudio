#only building C# bindings with Visual Studio
if(MSVC)

  install(DIRECTORY "examples"
    DESTINATION "CSharp/"
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

  set(CSHARP_VERSION_MAJOR ${CMAKE_VERSION_MAJOR})
  set(CSHARP_VERSION_MINOR ${CMAKE_VERSION_MINOR})
  set(CSHARP_VERSION_PATCH ${CMAKE_VERSION_PATCH})
  set(CSHARP_VERSION_BUILD 0)

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
    DEPENDS ${ALL_CSHARP_BINDING_TARGETS}
  )

  # convenience target to just build the SDK
  add_custom_target(csharp_sdk ALL
    DEPENDS ${OPENSTUDIO_CSHARP_DLL}
    COMMENT "Compiling .NET SDK"
  )

  install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Debug/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG)
  install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Release/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE)
endif()
