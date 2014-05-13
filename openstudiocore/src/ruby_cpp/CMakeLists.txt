# Adding math.h definitions to avoid "inconsistent dll linkage" from Ruby 1.8 and VS2013
if(${MSVC_VERSION} EQUAL 1800 AND RUBY_VERSION_MAJOR EQUAL 1 AND RUBY_VERSION_MINOR EQUAL 8)
  add_definitions(-DHAVE_ACOSH -DHAVE_ERF)
endif()

#include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${RUBY_INCLUDE_DIRS})
include_directories(${RUBY_CONFIG_INCLUDE_DIR})

add_executable(ruby_cpp
  ruby.cpp
)

if(MSVC)
  set(RUBY_CPP_EXE "${CMAKE_BINARY_DIR}/Products/$(ConfigurationName)/ruby_cpp" PARENT_SCOPE)
else()
  set(RUBY_CPP_EXE "${CMAKE_BINARY_DIR}/Products/ruby_cpp" PARENT_SCOPE)
endif()

if(UNIX)
  target_link_libraries(ruby_cpp ${RUBY_LIBRARY} dl)
else()
  target_link_libraries(ruby_cpp ${RUBY_LIBRARY})
endif()
