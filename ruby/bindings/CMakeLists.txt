add_library( rubybindings OBJECT
  InitRubyBindings.hpp
  InitRubyBindings.cpp
)

target_include_directories(rubybindings PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)
target_link_libraries(rubybindings PRIVATE ${ALL_RUBY_BINDING_TARGETS} rubyinterpreter-declarations)

# wd4996=no deprecated warnings ; wd5033=register
target_compile_options(rubybindings PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/bigobj /wd4996 /wd5033>
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wno-error=register -Wno-register>
  $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>:-Wno-deprecated-volatile>
  $<$<CXX_COMPILER_ID:GNU>:-Wno-volatile>
)
