# sketchup plugin

install(DIRECTORY openstudio/sketchup_plugin DESTINATION ./SketchUpPlugin/openstudio/ COMPONENT "SketchUpPlugin")
install(FILES plugin/Startup.rb DESTINATION ./SketchUpPlugin/plugin/ COMPONENT "SketchUpPlugin")
install(FILES plugin/OpenStudio.hash DESTINATION ./SketchUpPlugin/plugin/ COMPONENT "SketchUpPlugin")
install(FILES plugin/OpenStudio.rb DESTINATION ./SketchUpPlugin/plugin/ COMPONENT "SketchUpPlugin")

# Elaborate shenanigans because we want to make a plugin_test.rb file,
# that can be copied into the SketchUp plugins directory for testing the 
# plugin during development.
# Complicated because important parts of OpenStudio are in <build>/Products/<config>/
# where config is determined by a cmake generator expression
add_custom_target( SketchUp_Plugin 
  ALL
  COMMAND ${CMAKE_COMMAND} 
  -DRUBY_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" 
  -DCMAKE_PRODUCTS_DIR="${CMAKE_BINARY_DIR}/Products"
  -DOPENSTUDIO_LIB=$<TARGET_FILE:openstudio_rb> 
  -P "${CMAKE_CURRENT_SOURCE_DIR}/configure_sketchup_plugin_test.cmake"
)

