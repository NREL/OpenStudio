set(CMAKE_CXX_STANDARD 11)

include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/src/zlib)
include_directories(${CMAKE_BINARY_DIR}/src/zlib)

add_executable(CreateEmbeddedSource
  CreateEmbeddedSource.cpp
)

target_link_libraries(CreateEmbeddedSource miniziplib)

