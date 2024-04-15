add_executable(CreateEmbeddedSource
  CreateEmbeddedSource.cpp
)

target_link_libraries(CreateEmbeddedSource ZLIB::ZLIB)

# Add a specific manifest for CreateEmbeddedSource that will include the LongPathAware attribute, which,
# in conjunction with the regkey LongPathsEnabled=1 will make it work with paths that are longer than MAX_PATH (win10 only)
if( WIN32 )
  add_custom_command(TARGET CreateEmbeddedSource
    POST_BUILD
    COMMAND  mt -nologo -manifest "${CMAKE_CURRENT_SOURCE_DIR}/CreateEmbeddedSource.manifest.xml" -outputresource:$<TARGET_FILE:CreateEmbeddedSource>
  )
endif()
