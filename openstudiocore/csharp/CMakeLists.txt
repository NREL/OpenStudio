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

  # Take stab at finding Sandcastle to make sure the runSandcastle.bat will execute
  # TODO: consider rolling runSandcastle.bat into this file, using CMake to find the
  # required tools.
  #find_program(SANDCASTLE_TOOLS MrefBuilder.exe
  #  HINTS ENV ProgramFiles 
  #  PATH_SUFFIXES "Sandcastle/ProductionTools"
  #)

  #if(${SANDCASTLE_TOOLS} STREQUAL "SANDCASTLE_TOOLS-NOTFOUND")
  #  message(SEND_ERROR "Sandcastle tools not found, unable to build C# SDK on Windows")
  #endif()


  # CSharp unit tests
  #set(csharp_test_src
  #  test/UnitTests/Time_Test.cs
  #)

  # need to know if using the Express edition of Visual Studio, devenv is split into multiple programs
  #option(MSVC_IS_EXPRESS "using the Express editions of Visual Studio, must install both C++ and C# versions" FALSE)

  #if(${MSVC_IS_EXPRESS})
    set(MSVC_BUILD_TOOL "MSBuild")
  #else()
  #  set(MSVC_BUILD_TOOL "MSBuild")
  #endif()

  file(TO_NATIVE_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp" CSHARP_LIBRARY_OUTPUT_DIRECTORY)
  file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/csharp" CSHARP_SOURCE_DIR)
  file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/csharp_wrapper" CSHARP_BINARY_DIR)

  set(CSHARP_VERSION_MAJOR ${CMAKE_VERSION_MAJOR})
  set(CSHARP_VERSION_MINOR ${CMAKE_VERSION_MINOR})
  set(CSHARP_VERSION_PATCH ${CMAKE_VERSION_PATCH}) 
  set(CSHARP_VERSION_BUILD ${CMAKE_VERSION_BUILD}) 

  # Test to see if CSHARP_VERSION_BUILD has something meaningful set
  if(${CSHARP_VERSION_BUILD} MATCHES [^0-9])
    message(WARNING "CSHARP_VERSION_BUILD does not have a valid value (${CSHARP_VERSION_BUILD}), defaulting to 0")
    set(CSHARP_VERSION_BUILD 0)
  endif()

  set(CSHARP_ASSEMBLY_VERSION "${CSHARP_VERSION_MAJOR}.${CSHARP_VERSION_MINOR}.${CSHARP_VERSION_PATCH}.${CSHARP_VERSION_BUILD}")

  #if(MSVC12)
    set(CSHARP_TOOLS_VERSION "4.5")
  #elseif(MSVC10)
  #  set(CSHARP_TOOLS_VERSION "4.0")
  #else()
  #  set(CSHARP_TOOLS_VERSION "3.5")
  #endif()

  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/csharp_wrapper/generated_sources")
  configure_file("${CMAKE_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj.in" "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj")
  #configure_file("${CMAKE_SOURCE_DIR}/csharp/OpenStudioSDK.iss.in" "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudioSDK.iss")
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
    COMMAND "${MSVC_BUILD_TOOL}" "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj" "/p:Configuration=$(ConfigurationName)"  
    DEPENDS ${ALL_CSHARP_BINDING_TARGETS}
  )
  
  # convenience target to just build the SDK
  add_custom_target( csharp_sdk ALL
    DEPENDS ${OPENSTUDIO_CSHARP_DLL}
    COMMENT "Compiling .NET SDK"
  )
  
  install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Debug/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG)
  install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Release/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE)
  
  # run all the unit tests
  #if(BUILD_TESTING OR BUILD_SIMPLE_TESTING)

    ## need to find NUnit
    #find_program(NUNIT_EXECUTABLE nunit-console.exe 
    #  HINTS ENV ProgramFiles 
    #  PATH_SUFFIXES "NUnit 2.5.3/bin/net-2.0"
    #)
    
    #if(${NUNIT_EXECUTABLE} STREQUAL "NUNIT_EXECUTABLE-NOTFOUND")
    #  message(SEND_ERROR "NUnit not found, unable to build CSharp tests on windows")
    #endif()
  
    ## the unit test dll in current configuration mode
    #set(OPENSTUDIO_CSHARP_TEST_DLL "${CMAKE_SOURCE_DIR}/csharp/test/UnitTests/bin/$(ConfigurationName)/UnitTests.dll")
    #set(OPENSTUDIO_CSHARP_TEST_INSTALL_DLL "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/$(ConfigurationName)/UnitTests.dll")
  
    ## build the tests
    #add_custom_command(
    #  OUTPUT ${OPENSTUDIO_CSHARP_TEST_DLL}

    #  COMMAND ${CMAKE_COMMAND} -E copy "${OPENSTUDIO_CSHARP_DLL}" "${CMAKE_SOURCE_DIR}/csharp/test/UnitTests/bin/."

    #  COMMAND "${MSVC_BUILD_TOOL}" "${CMAKE_SOURCE_DIR}/csharp/test/UnitTests/UnitTests.csproj" /Build $(ConfigurationName)

    #  COMMAND ${CMAKE_COMMAND} -E copy "${OPENSTUDIO_CSHARP_TEST_DLL}" "${OPENSTUDIO_CSHARP_TEST_DLL}"
    #  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/csharp/test/UnitTests/bin/$(ConfigurationName)/nunit.framework.dll" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/$(ConfigurationName)/nunit.framework.dll"
      
    #  DEPENDS ${OPENSTUDIO_CSHARP_DLL} ${ALL_CSHARP_BINDING_TARGETS} ${csharp_test_src}
    #)
    
    ## build the unit tests, primarily exists so ctest will work after building ALL
    #add_custom_target(csharp_unit_tests ALL
    #  DEPENDS ${OPENSTUDIO_CSHARP_TEST_DLL} 
    #  SOURCES ${csharp_test_src}
    #)

    ## run the tests
    #add_custom_target(csharp_run_tests
    #  COMMAND "${NUNIT_EXECUTABLE}" "${OPENSTUDIO_CSHARP_TEST_INSTALL_DLL}" "/nologo" 
    #  DEPENDS ${OPENSTUDIO_CSHARP_TEST_DLL} 
    #)

    ## add test to dashboard, could get fancier with this but not yet
    ## hard to parse C# looking for tests, can call NUnit with '/labels' to get list of tests but don't know how to do that
    ## at the right time.  Other option is to just state naming convetion (e.g. Test_Func) and parse as in Ruby tests.
    #add_test(NAME "CSharpTest" 
    #  COMMAND "${NUNIT_EXECUTABLE}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/$<CONFIGURATION>/UnitTests.dll" "/nologo" 
    #)    

  #endif()

  ## build the package
  #find_program(INNO_SETUP_EXECUTABLE ISCC.exe
  #  HINTS ENV ProgramFiles 
  #  PATH_SUFFIXES "Inno Setup 5"
  #)

  #if(${INNO_SETUP_EXECUTABLE} STREQUAL "INNO_SETUP_EXECUTABLE-NOTFOUND")
  #  message(SEND_ERROR "Inno Setup compiler not found, unable to build CSharp package on windows")
  #endif()

  #set(CSHARP_INSTALLER_FILE "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/OpenStudio_DotNET_SDK-${CSHARP_ASSEMBLY_VERSION}.exe")

  #file(TO_NATIVE_PATH "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}" CSHARP_SDK_OUTPUT_DIR)

  #file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudioSDK.iss" CSHARP_SDK_SOURCE)

  #add_custom_target(csharp_sdk_installer
  #  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/csharp/runSandcastle.bat" "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/Release/."
  #  COMMAND ${CMAKE_COMMAND} -E chdir "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/Release/" "runSandcastle.bat"
  #  COMMAND "${INNO_SETUP_EXECUTABLE}" "${CSHARP_SDK_SOURCE}" "/O\"${CSHARP_SDK_OUTPUT_DIR}\""
  #  DEPENDS ${ALL_CSHARP_BINDING_TARGETS} ${OPENSTUDIO_CSHARP_DLL} ${CSHARP_SDK_SOURCE} ${ALL_CPP_DOC_TARGETS} 
  #  COMMENT "Making .NET SDK Installer"
  #)

endif()
