

#only building C# bindings with Visual Studio
IF(MSVC)


  INSTALL(DIRECTORY "examples"
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
  FIND_PROGRAM(SANDCASTLE_TOOLS MrefBuilder.exe
    HINTS ENV ProgramFiles 
    PATH_SUFFIXES "Sandcastle/ProductionTools"
  )

  IF ( ${SANDCASTLE_TOOLS} STREQUAL "SANDCASTLE_TOOLS-NOTFOUND")
    MESSAGE(SEND_ERROR "Sandcastle tools not found, unable to build C# SDK on Windows")
  ENDIF()


  # CSharp unit tests
#  SET( csharp_test_src
#    test/UnitTests/Time_Test.cs
#  )

  # need to know if using the Express edition of Visual Studio, devenv is split into multiple programs
  OPTION( MSVC_IS_EXPRESS "using the Express editions of Visual Studio, must install both C++ and C# versions" FALSE )

  IF(${MSVC_IS_EXPRESS})
    SET(MSVC_BUILD_TOOL "vcsexpress")
  ELSE()
    SET(MSVC_BUILD_TOOL "devenv")
  ENDIF()

  FILE(TO_NATIVE_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp" CSHARP_LIBRARY_OUTPUT_DIRECTORY )
  FILE(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/csharp" CSHARP_SOURCE_DIR )
  FILE(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/csharp_wrapper" CSHARP_BINARY_DIR )

  SET(CSHARP_VERSION_MAJOR ${CMAKE_VERSION_MAJOR})
  SET(CSHARP_VERSION_MINOR ${CMAKE_VERSION_MINOR})
  SET(CSHARP_VERSION_BUILD ${CMAKE_VERSION_PATCH}) # svn revision seems like a good match for build number, being trackable
  SET(CSHARP_VERSION_REVISION 0) # I don't think we have a good match for revision 

  # Test to see if CSHARP_VERSION_BUILD has something meaningful set
  IF (${CSHARP_VERSION_BUILD} MATCHES [^0-9])
    MESSAGE(WARNING "Version patch does not have a valid value (${CSHARP_VERSION_BUILD}), defaulting to 0")
    SET(CSHARP_VERSION_BUILD 0)
  ENDIF()

  SET(CSHARP_ASSEMBLY_VERSION "${CSHARP_VERSION_MAJOR}.${CSHARP_VERSION_MINOR}.${CSHARP_VERSION_BUILD}.${CSHARP_VERSION_REVISION}")

  IF(MSVC10)
    SET(CSHARP_TOOLS_VERSION "4.0")
  ELSE()
    SET(CSHARP_TOOLS_VERSION "3.5")
  ENDIF()

  FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/csharp_wrapper/generated_sources")
  CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj.in" "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj" )
  # CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/csharp/OpenStudioSDK.iss.in" "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudioSDK.iss" )
  CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/csharp/developer/OpenStudio/Properties/AssemblyVersion.cs.in" "${CMAKE_BINARY_DIR}/csharp_wrapper/generated_sources/AssemblyVersion.cs" )

  #This is generating odd errors about needing to convert the included project file, but it seems like it would be a good
  #way to move forward, if it worked. Leaving here as a note.
  #INCLUDE_EXTERNAL_MSPROJECT(OpenStudio "${CMAKE_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj" ${CSHARP_LIBRARIES} )
  #in leiu of the above, we are using the following:
  
  # the main OpenStudio dll
  SET( OPENSTUDIO_CSHARP_DLL "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/$(ConfigurationName)/OpenStudio.dll" )
  
  
  # custom command to make OPENSTUDIO_CSHARP_DLL
  ADD_CUSTOM_COMMAND(
    OUTPUT ${OPENSTUDIO_CSHARP_DLL}
    COMMAND "${MSVC_BUILD_TOOL}" "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj" /Build "\"$(ConfigurationName)|x86\""  
    DEPENDS ${ALL_CSHARP_BINDING_TARGETS}
  )
  
  # convienience target to just build the SDK
  ADD_CUSTOM_TARGET( csharp_sdk ALL
    DEPENDS ${OPENSTUDIO_CSHARP_DLL}
    COMMENT "Compiling .NET SDK"
  )
  
  INSTALL( FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Debug/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG  )
  INSTALL( FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Release/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE  )
  
  # run all the unit tests
  # IF( BUILD_TESTING OR BUILD_SIMPLE_TESTING )

    # # need to find NUnit
    # FIND_PROGRAM(NUNIT_EXECUTABLE nunit-console.exe 
      # HINTS ENV ProgramFiles 
      # PATH_SUFFIXES "NUnit 2.5.3/bin/net-2.0"
    # )
    
    # IF ( ${NUNIT_EXECUTABLE} STREQUAL "NUNIT_EXECUTABLE-NOTFOUND")
      # MESSAGE(SEND_ERROR "NUnit not found, unable to build CSharp tests on windows")
    # ENDIF()
  
    # # the unit test dll in current configuration mode
    # SET( OPENSTUDIO_CSHARP_TEST_DLL "${CMAKE_SOURCE_DIR}/csharp/test/UnitTests/bin/$(ConfigurationName)/UnitTests.dll" )
    # SET( OPENSTUDIO_CSHARP_TEST_INSTALL_DLL "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/$(ConfigurationName)/UnitTests.dll" )
  
    # # build the tests
    # ADD_CUSTOM_COMMAND(
      # OUTPUT ${OPENSTUDIO_CSHARP_TEST_DLL}

      # COMMAND ${CMAKE_COMMAND} -E copy "${OPENSTUDIO_CSHARP_DLL}" "${CMAKE_SOURCE_DIR}/csharp/test/UnitTests/bin/."

      # COMMAND "${MSVC_BUILD_TOOL}" "${CMAKE_SOURCE_DIR}/csharp/test/UnitTests/UnitTests.csproj" /Build $(ConfigurationName)

      # COMMAND ${CMAKE_COMMAND} -E copy "${OPENSTUDIO_CSHARP_TEST_DLL}" "${OPENSTUDIO_CSHARP_TEST_DLL}"
      # COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/csharp/test/UnitTests/bin/$(ConfigurationName)/nunit.framework.dll" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/$(ConfigurationName)/nunit.framework.dll"
      
      # DEPENDS ${OPENSTUDIO_CSHARP_DLL} ${ALL_CSHARP_BINDING_TARGETS}  ${csharp_test_src}
    # )
    
    # # build the unit tests, primarily exists so ctest will work after building ALL
    # ADD_CUSTOM_TARGET( csharp_unit_tests ALL
      # DEPENDS ${OPENSTUDIO_CSHARP_TEST_DLL} 
      # SOURCES ${csharp_test_src}
    # )

    # # run the tests
    # ADD_CUSTOM_TARGET( csharp_run_tests
      # COMMAND "${NUNIT_EXECUTABLE}" "${OPENSTUDIO_CSHARP_TEST_INSTALL_DLL}" "/nologo" 
      # DEPENDS ${OPENSTUDIO_CSHARP_TEST_DLL} 
    # )

    # # add test to dashboard, could get fancier with this but not yet
    # # hard to parse C# looking for tests, can call NUnit with '/labels' to get list of tests but don't know how to do that
    # # at the right time.  Other option is to just state naming convetion (e.g. Test_Func) and parse as in Ruby tests.
    # ADD_TEST(NAME "CSharpTest" 
      # COMMAND "${NUNIT_EXECUTABLE}" "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/$<CONFIGURATION>/UnitTests.dll" "/nologo" 
    # )    

  # ENDIF( BUILD_TESTING OR BUILD_SIMPLE_TESTING )

  # build the package
  #FIND_PROGRAM(INNO_SETUP_EXECUTABLE ISCC.exe
  #  HINTS ENV ProgramFiles 
  #  PATH_SUFFIXES "Inno Setup 5"
  #)

  #IF ( ${INNO_SETUP_EXECUTABLE} STREQUAL "INNO_SETUP_EXECUTABLE-NOTFOUND")
  #  MESSAGE(SEND_ERROR "Inno Setup compiler not found, unable to build CSharp package on windows")
  #ENDIF()

  #SET(CSHARP_INSTALLER_FILE "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/OpenStudio_DotNET_SDK-${CSHARP_ASSEMBLY_VERSION}.exe")

  #FILE(TO_NATIVE_PATH "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}" CSHARP_SDK_OUTPUT_DIR)

  #FILE(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudioSDK.iss" CSHARP_SDK_SOURCE)

  #ADD_CUSTOM_TARGET(csharp_sdk_installer
  #  COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/csharp/runSandcastle.bat" "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/Release/."
  #  COMMAND ${CMAKE_COMMAND} -E chdir "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/Release/" "runSandcastle.bat"
  #  COMMAND "${INNO_SETUP_EXECUTABLE}" "${CSHARP_SDK_SOURCE}" "/O\"${CSHARP_SDK_OUTPUT_DIR}\""
  #  DEPENDS ${ALL_CSHARP_BINDING_TARGETS} ${OPENSTUDIO_CSHARP_DLL} ${CSHARP_SDK_SOURCE} ${ALL_CPP_DOC_TARGETS} 
  #  COMMENT "Making .NET SDK Installer"
  #)

ENDIF(MSVC)

