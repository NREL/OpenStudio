
  file(TO_NATIVE_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/java" JAVA_LIBRARY_OUTPUT_DIRECTORY )

  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/java_wrapper/generated_sources")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources")

  # the main OpenStudio jar file
  if(MSVC)
    set(OPENSTUDIO_JAVA_JAR "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/$(ConfigurationName)/OpenStudio.jar")
  else()
    set(OPENSTUDIO_JAVA_JAR "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/OpenStudio.jar")
  endif()


  # custom command to make OPENSTUDIO_JAVA_JAR
  add_custom_command(
    OUTPUT ${OPENSTUDIO_JAVA_JAR}
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources"
    DEPENDS ${ALL_JAVA_BINDING_TARGETS}
  )

  foreach(D ${ALL_JAVA_SRC_DIRECTORIES})
    add_custom_command(
      OUTPUT ${OPENSTUDIO_JAVA_JAR}
      COMMAND "${CMAKE_COMMAND}" -E copy_directory "${D}" "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources"
      APPEND
    )
  endforeach()

  if(MSVC)
    add_custom_command(
      OUTPUT  ${OPENSTUDIO_JAVA_JAR}
      COMMAND cmd /C dir /B "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources/*.java" > "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources/filelist"
      APPEND
    )
  else()
    add_custom_command(
      OUTPUT  ${OPENSTUDIO_JAVA_JAR}
      COMMAND find "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources/" -name "\"*.java\"" > "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources/filelist"
      APPEND
    )
  endif()

  add_custom_command(
    OUTPUT ${OPENSTUDIO_JAVA_JAR}
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_BINARY_DIR}/java_wrapper/build/"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/java_wrapper/build/"
    COMMAND "${Java_JAVAC_EXECUTABLE}" -target 1.6 -source 1.6 -d "${CMAKE_BINARY_DIR}/java_wrapper/build/" "@${CMAKE_BINARY_DIR}/java_wrapper/merged_sources/filelist"
    COMMAND "${Java_JAR_EXECUTABLE}"  -cvf ${OPENSTUDIO_JAVA_JAR} -C "${CMAKE_BINARY_DIR}/java_wrapper/build/" gov
    APPEND
  )

  # convenience target to just build the SDK
  add_custom_target(java_sdk ALL
    DEPENDS ${OPENSTUDIO_JAVA_JAR}
    COMMENT "Compiling Java JAR"
  )




  if(MSVC)
    install(FILES "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/Debug/OpenStudio.jar" DESTINATION Java/openstudio/ CONFIGURATIONS DEBUG)
    install(FILES "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/Release/OpenStudio.jar" DESTINATION Java/openstudio/)
  elseif(APPLE)
    install(FILES "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/OpenStudio.jar" DESTINATION Java/openstudio/)
  else()
    install(FILES "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/OpenStudio.jar" DESTINATION lib/openstudio/java)
  endif()






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

  # build the package
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
