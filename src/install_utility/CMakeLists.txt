add_executable(install_utility main.cpp)

target_link_libraries(install_utility
  boost::boost
  # boost::boost_filesystem
)

if( WIN32 )
  add_custom_command(TARGET install_utility
    POST_BUILD
    COMMAND  mt -nologo -manifest "${CMAKE_CURRENT_SOURCE_DIR}/manifest.xml" -outputresource:$<TARGET_FILE:install_utility>
  )
endif()

install(TARGETS install_utility DESTINATION bin)
