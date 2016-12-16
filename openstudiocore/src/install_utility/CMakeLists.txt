add_executable(install_utility main.cpp)

if( WIN32 )
  add_custom_command(TARGET install_utility
    POST_BUILD
    COMMAND  mt -nologo -manifest "${CMAKE_CURRENT_SOURCE_DIR}/manifest.xml" -outputresource:$<TARGET_FILE:install_utility>
  )
endif()
