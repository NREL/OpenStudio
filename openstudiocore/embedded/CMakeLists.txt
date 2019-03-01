set(CMAKE_CXX_STANDARD 11)


add_executable(CreateEmbeddedSource
  CreateEmbeddedSource.cpp
)

target_link_libraries(CreateEmbeddedSource zlib::zlib)

