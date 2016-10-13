
#install(DIRECTORY "examples"
#  DESTINATION "CSharp/"
#  COMPONENT "CSharpAPI"
#  FILES_MATCHING PATTERN "*.cs"
#  PATTERN "*.csproj"
#  PATTERN "*.sln"
#  PATTERN "*.resx"
#  PATTERN "*.settings"
#  PATTERN "*.docx"
#)

#file(TO_NATIVE_PATH "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp" CSHARP_LIBRARY_OUTPUT_DIRECTORY)
#file(TO_NATIVE_PATH "${CMAKE_SOURCE_DIR}/csharp" CSHARP_SOURCE_DIR)
#file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/csharp_wrapper" CSHARP_BINARY_DIR)

#set(CSHARP_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
#set(CSHARP_VERSION_MINOR ${PROJECT_VERSION_MINOR})
#set(CSHARP_VERSION_PATCH ${PROJECT_VERSION_PATCH})
#set(CSHARP_VERSION_BUILD 0)

#if (CMAKE_CL_64)
#  set(CSHARP_PLATFORM "x64")
#else()
#  set(CSHARP_PLATFORM "x86")
#endif()

#set(CSHARP_ASSEMBLY_VERSION "${CSHARP_VERSION_MAJOR}.${CSHARP_VERSION_MINOR}.${CSHARP_VERSION_PATCH}.${CSHARP_VERSION_BUILD}")

#set(CSHARP_TOOLS_VERSION "4.5")

#file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/csharp_wrapper/generated_sources")
#configure_file("${CMAKE_SOURCE_DIR}/csharp/developer/OpenStudio/OpenStudio.csproj.in" "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj")
#configure_file("${CMAKE_SOURCE_DIR}/csharp/developer/OpenStudio/Properties/AssemblyVersion.cs.in" "${CMAKE_BINARY_DIR}/csharp_wrapper/generated_sources/AssemblyVersion.cs")

# the main OpenStudio dll
#set(OPENSTUDIO_CSHARP_DLL "${CSHARP_LIBRARY_OUTPUT_DIRECTORY}/$(ConfigurationName)/OpenStudio.dll")

# custom command to make OPENSTUDIO_CSHARP_DLL
#add_custom_command(
#  OUTPUT ${OPENSTUDIO_CSHARP_DLL}
#  COMMAND "MSBuild" "${CMAKE_BINARY_DIR}/csharp_wrapper/OpenStudio.csproj" "/p:Configuration=$(ConfigurationName)"
#  DEPENDS ${ALL_CSHARP_WRAPPER_TARGETS} ${ALL_CSHARP_WRAPPER_FILES}
#)

#install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Debug/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS DEBUG COMPONENT "CSharpAPI")
#install(FILES "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/csharp/Release/OpenStudio.dll" DESTINATION CSharp/openstudio/ CONFIGURATIONS RELEASE COMPONENT "CSharpAPI")

# DLM: currently there are some multiply defined symbols, force them for now, investigate later
#set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /FORCE:MULTIPLE")

# ignore deprecated warnings since we will export deprecated methods
add_definitions(/wd4996)

# ignore macro redefinition
add_definitions(/wd4005)

# build a dll to link all the static C# bindings into
add_library(
  openstudio_python
  MODULE
  ${ALL_PYTHON_WRAPPER_FILES}
)
add_dependencies(openstudio_python ${ALL_PYTHON_WRAPPER_TARGETS})

set_source_files_properties(${ALL_PYTHON_WRAPPER_FILES} PROPERTIES GENERATED TRUE)

target_link_libraries(openstudio_python
 ${ALL_PYTHON_BINDING_DEPENDS}
 ${Boost_LIBRARIES}
 ${PYTHON_LIBRARY}
)

install(TARGETS openstudio_python DESTINATION Python CONFIGURATIONS COMPONENT "Python")



