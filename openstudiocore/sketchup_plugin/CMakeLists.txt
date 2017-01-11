# sketchup plugin

install(DIRECTORY openstudio/sketchup_plugin DESTINATION ./SketchUpPlugin/openstudio/ COMPONENT "SketchUpPlugin")
install(FILES plugin/OpenStudio/Startup.rb DESTINATION ./SketchUpPlugin/plugin/OpenStudio/ COMPONENT "SketchUpPlugin")
install(FILES plugin/OpenStudio/OpenStudio.hash DESTINATION ./SketchUpPlugin/plugin/OpenStudio/ COMPONENT "SketchUpPlugin")
install(FILES plugin/OpenStudio.rb DESTINATION ./SketchUpPlugin/plugin/ COMPONENT "SketchUpPlugin")

