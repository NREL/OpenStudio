# Copy this file into the SketchUp plugins directory for development testing

require '${OPENSTUDIO_LIB}'
$: << '${RUBY_SOURCE_DIR}'
$: << File.dirname('${OPENSTUDIO_LIB}')

OpenStudio::Application::instance.application(true)
OpenStudio::Application::instance.application.setOrganizationName("NREL")
OpenStudio::Application::instance.application.setOrganizationDomain("nrel.gov")
OpenStudio::Application::instance.application.setApplicationName("OpenStudio")

$OPENSTUDIO_SKETCHUPPLUGIN_NAME = "OpenStudio Development"
ext = SketchupExtension.new($OPENSTUDIO_SKETCHUPPLUGIN_NAME, "${RUBY_SOURCE_DIR}/openstudio/sketchup_plugin/lib/PluginManager.rb")
ext.name = $OPENSTUDIO_SKETCHUPPLUGIN_NAME

# 'true' automatically loads the extension the first time it is registered, e.g., after install
Sketchup.register_extension(ext, true)

