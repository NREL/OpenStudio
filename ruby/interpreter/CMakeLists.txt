# "Link" to this when you need to include ruby.h,
# but don't want to link to libruby.
add_library(rubyinterpreter-declarations INTERFACE)
target_include_directories(rubyinterpreter-declarations INTERFACE
  ${Ruby_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# Must call CMake itself in order to set the SWIG_DIR env var for add_custom_command
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  COMMAND ${CMAKE_COMMAND} -E env SWIG_DIR="${SWIG_DIR}"
          "${SWIG_EXECUTABLE}"
          "-ruby"
          -external-runtime "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
)

# Link to rubyinterpreter when you need to link to OpenStudio's flavor of the Ruby interpreter
if (CMAKE_VERSION VERSION_LESS 3.19.0)
  add_library(rubyinterpreter INTERFACE)
  target_sources(rubyinterpreter PUBLIC INTERFACE
    RubyInterpreter.hpp
    RubyEval.hpp
    RubyException.hpp
    "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  )
else()
  add_library(rubyinterpreter INTERFACE
    RubyInterpreter.hpp
    RubyEval.hpp
    RubyException.hpp
    "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  )
endif()

target_include_directories(rubyinterpreter INTERFACE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(rubyinterpreter INTERFACE Ruby::Ruby)
