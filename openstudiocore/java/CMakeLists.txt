  set(JAVA_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/java")

  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/java_wrapper/generated_sources")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources")

  # the main OpenStudio jar file
  if(MSVC)
    set(OPENSTUDIO_JAVA_JAR "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/$(ConfigurationName)/OpenStudio.jar")
  else()
    set(OPENSTUDIO_JAVA_JAR "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/OpenStudio.jar")
  endif()

  if(MSVC)
    set(OPENSTUDIO_JAVA_BIN_DIR "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/$<CONFIGURATION>")
  else()
    set(OPENSTUDIO_JAVA_BIN_DIR "${JAVA_LIBRARY_OUTPUT_DIRECTORY}")
  endif()


  file(TO_NATIVE_PATH "${OPENSTUDIO_JAVA_JAR}" OPENSTUDIO_NATIVE_JAVA_JAR)


  # custom command to make OPENSTUDIO_JAVA_JAR
  add_custom_command(
    OUTPUT "${OPENSTUDIO_JAVA_JAR}"
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources"
    DEPENDS ${ALL_JAVA_BINDING_TARGETS}
  )

  foreach(D ${ALL_JAVA_SRC_DIRECTORIES})
    add_custom_command(
      OUTPUT "${OPENSTUDIO_JAVA_JAR}"
      COMMAND "${CMAKE_COMMAND}" -E copy_directory "${D}" "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources"
      APPEND
    )
  endforeach()

  if(MSVC)
    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources/*.java" NATIVE_JAVA_SOURCE_LIST)
    file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources/filelist" NATIVE_JAVA_SOURCE_FILELIST)
    add_custom_command(
      OUTPUT "${OPENSTUDIO_JAVA_JAR}"
      COMMAND cmd /C dir /s /B "${NATIVE_JAVA_SOURCE_LIST}" > "${NATIVE_JAVA_SOURCE_FILELIST}"
      APPEND
    )
  else()
    add_custom_command(
      OUTPUT "${OPENSTUDIO_JAVA_JAR}"
      COMMAND find "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources/" -name "\"*.java\"" > "${CMAKE_BINARY_DIR}/java_wrapper/merged_sources/filelist"
      APPEND
    )
  endif()

  add_custom_command(
    OUTPUT "${OPENSTUDIO_JAVA_JAR}"
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "${CMAKE_BINARY_DIR}/java_wrapper/build/"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/java_wrapper/build/"
    COMMAND "${Java_JAVAC_EXECUTABLE}" -target 1.6 -source 1.6 -d "${CMAKE_BINARY_DIR}/java_wrapper/build/" "@${CMAKE_BINARY_DIR}/java_wrapper/merged_sources/filelist"
    COMMAND "${Java_JAR_EXECUTABLE}"  -cvf "${OPENSTUDIO_JAVA_JAR}" -C "${CMAKE_BINARY_DIR}/java_wrapper/build/" gov
    APPEND
  )

  # convenience target to just build the SDK
  add_custom_target(java_sdk ALL
    DEPENDS "${OPENSTUDIO_JAVA_JAR}"
    COMMENT "Compiling Java JAR"
  )

  if(MSVC)
    install(FILES "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/Debug/OpenStudio.jar" DESTINATION Java/openstudio/ CONFIGURATIONS DEBUG)
    install(FILES "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/Release/OpenStudio.jar" DESTINATION Java/openstudio/)
  elseif(APPLE)
    install(FILES "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/OpenStudio.jar" DESTINATION Java/openstudio/)
  else()
    install(FILES "${JAVA_LIBRARY_OUTPUT_DIRECTORY}/OpenStudio.jar" DESTINATION lib/openstudio-${OPENSTUDIO_VERSION}/java)
  endif()

  # Hand run commands we are replicating here
  
  #  jason@ubuntu12:~/OpenStudio-main/openstudiocore/java$ /usr/lib/jvm/java-1.7.0-openjdk-amd64/bin/javac TestOpenStudio.java -classpath ~/openstudio-main-build/OSCore-prefix/src/OSCore-build/Products/java/OpenStudio.jar
  #  jason@ubuntu12:~/OpenStudio-main/openstudiocore/java$ LD_LIBRARY_PATH=~/openstudio-main-build/OSCore-prefix/src/OSCore-build/Products/java /usr/lib/jvm/java-1.7.0-openjdk-amd64/bin/java -classpath ~/openstudio-main-build/OSCore-prefix/src/OSCore-build/Products/java/OpenStudio.jar:.  TestOpenStudio

  add_custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/TestOpenStudio.class"
    COMMAND "${Java_JAVAC_EXECUTABLE}" -d "${CMAKE_BINARY_DIR}" "${CMAKE_SOURCE_DIR}/java/TestOpenStudio.java" -classpath "${OPENSTUDIO_JAVA_JAR}"
    DEPENDS "${OPENSTUDIO_JAVA_JAR}"
    )

  add_custom_target(java_tests ALL
    DEPENDS "${CMAKE_BINARY_DIR}/TestOpenStudio.class"
   )

  if (MSVC)
    add_test(NAME "JavaSanityCheck" 
      COMMAND "${Java_JAVA_EXECUTABLE}"  -classpath "${CMAKE_BINARY_DIR}/Products/java/$<CONFIGURATION>/OpenStudio.jar;${CMAKE_BINARY_DIR}" TestOpenStudio
	  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Products/java/$<CONFIGURATION>" 
      )
  else()
    add_test(NAME "JavaSanityCheck" 
      COMMAND "${Java_JAVA_EXECUTABLE}"  "-Djava.library.path=${OPENSTUDIO_JAVA_BIN_DIR}" -classpath "${OPENSTUDIO_JAVA_JAR}:${CMAKE_BINARY_DIR}" TestOpenStudio
      )
  endif()
