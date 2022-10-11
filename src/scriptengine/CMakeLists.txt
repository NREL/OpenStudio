# Script Engine is a base for scripting in OpenStudio
# See implementations in ruby/engine and python/engine

add_library(scriptengine INTERFACE)
target_include_directories(scriptengine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
