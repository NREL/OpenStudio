if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "^(Apple)?Clang$")
  # using Clang
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -undefined dynamic_lookup")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # using GCC
  #set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --unresolved-symbols=ignore-all")
  #set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --unresolved-symbols=ignore-all")
endif()


include_directories(${CMAKE_CURRENT_BINARY_DIR} ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

# openstudio ruby bindings to load into normal ruby
add_library(
  openstudio_rb
  MODULE
  RubyAPI.hpp
  openstudio_rb.cpp
)
set_target_properties(openstudio_rb PROPERTIES PREFIX "")
set_target_properties(openstudio_rb PROPERTIES OUTPUT_NAME openstudio)
target_include_directories(openstudio_rb SYSTEM PRIVATE ${Ruby_INCLUDE_DIRS})

# change output directory so the openstudio.pdb file does not conflict between this target and the CLI
set_target_properties(openstudio_rb PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/ruby/")
set_target_properties(openstudio_rb PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/ruby/")
set_target_properties(openstudio_rb PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ruby/")

if(APPLE)
  set_target_properties(openstudio_rb PROPERTIES SUFFIX ".bundle" )
else()
  set_target_properties(openstudio_rb PROPERTIES SUFFIX ".so" )
endif()

if(MSVC)
  set_target_properties(openstudio_rb PROPERTIES COMPILE_FLAGS "/bigobj /wd4996 /wd5033")
endif()

add_custom_command(TARGET openstudio_rb
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/openstudio.rb $<TARGET_FILE_DIR:openstudio_rb>/openstudio.rb
)

file(GLOB_RECURSE OPENSTUDIO_FILES FOLLOW_SYMLINKS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/" "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/**/*.*")
foreach(OPENSTUDIO_FILE ${OPENSTUDIO_FILES})
  add_custom_command(TARGET openstudio_rb
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/${OPENSTUDIO_FILE}" "$<TARGET_FILE_DIR:openstudio_rb>/openstudio/${OPENSTUDIO_FILE}"
  )
endforeach()

if( WIN32 )
  # Dynamically link to openstudiolib.dll on Windows only
  target_link_libraries(
    openstudio_rb
    PUBLIC
    openstudiolib
    PRIVATE
    rubybindings
    ${ALL_RUBY_BINDING_TARGETS}
  )
  target_link_libraries(openstudio_rb PRIVATE ${RUBY_MINGW_STUB_LIB})
else()
  # statically link everywhere else
  target_link_libraries(
    openstudio_rb
    PRIVATE
    openstudio_utilities
    openstudio_airflow
    openstudio_model
    openstudio_energyplus
    openstudio_epjson
    openstudio_alfalfa
    openstudio_measure
    openstudio_osversion
    openstudio_sdd
    openstudio_isomodel
    openstudio_gbxml
    openstudio_gltf
    openstudio_radiance
    rubybindings
    ${ALL_RUBY_BINDING_TARGETS}
  )
endif()

if (APPLE)
  target_link_libraries(openstudio_rb
    PRIVATE
    ${OPENSSL_CRYPTO_LIBRARY}
    ${OPENSSL_SSL_LIBRARY}
  )
endif()

install(TARGETS openstudio_rb
  EXPORT openstudio
  DESTINATION Ruby
  COMPONENT "RubyAPI"
)
install(FILES openstudio.rb DESTINATION Ruby COMPONENT "RubyAPI")
install(DIRECTORY openstudio DESTINATION Ruby COMPONENT "RubyAPI")
