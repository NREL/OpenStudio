add_library( rubybindings OBJECT
  InitRubyBindings.hpp
  InitRubyBindings.cpp
)

target_include_directories(rubybindings PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rubybindings PUBLIC ${ALL_RUBY_BINDING_TARGETS} rubyinterpreter-delcarations)

if(MSVC)
  # wd4996=no deprecated warnings ; wd5033=register
  set_target_properties(rubybindings PROPERTIES COMPILE_FLAGS "/bigobj /wd4996 /wd5033")
elseif (UNIX)
  # Disable register warnings
  set_target_properties(rubybindings PROPERTIES COMPILE_FLAGS " -Wno-error=register -Wno-register")
endif()
