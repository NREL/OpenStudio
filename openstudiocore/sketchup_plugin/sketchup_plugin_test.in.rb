# Copy this file into the SketchUp plugins directory for development testing

# path to openstudio.so
$OpenStudio_Dir = File.dirname('${OPENSTUDIO_LIB}')

# DLM: this is really the path to the sketchup plugin
$OpenStudio_LibPath = '${RUBY_SOURCE_DIR}/'

$: << $OpenStudio_Dir
$: << $OpenStudio_LibPath 

# add OpenStudio shared library locations to system path
# we need to add these so that require can load the c++ shared libraries
original_path = ENV['PATH']
platform_specific_path = nil
if /mswin/.match(RUBY_PLATFORM) or /mingw/.match(RUBY_PLATFORM)
  front = []
  back = []
  original_path.split(';').each do |p|
    if /SketchUp/.match(p)
      if /platform_specific/.match(p)
        platform_specific_path = p
      end
      front << p
    else
      back << p
    end
  end

  ENV['PATH'] = "#{front.join(';')};#{$OpenStudio_Dir};#{back.join(';')}"
  
  # Pre-load our specific Qt Dll's on Windows to make sure we control which get loaded
  require 'Win32API'
  library = Win32API.new('kernel32','LoadLibrary', 'P', 'L')  
  library.call("#{$OpenStudio_Dir}/icudt51.dll")
  library.call("#{$OpenStudio_Dir}/icuin51.dll")
  library.call("#{$OpenStudio_Dir}/icuuc51.dll")
  library.call("#{$OpenStudio_Dir}/libEGL.dll")
  library.call("#{$OpenStudio_Dir}/libGLESv2.dll")
  library.call("#{$OpenStudio_Dir}/Qt5Core.dll")
  library.call("#{$OpenStudio_Dir}/Qt5Gui.dll")
  library.call("#{$OpenStudio_Dir}/Qt5Network.dll")
  library.call("#{$OpenStudio_Dir}/Qt5Sql.dll")
  library.call("#{$OpenStudio_Dir}/Qt5Widgets.dll")
  library.call("#{$OpenStudio_Dir}/Qt5Xml.dll")
  library.call("#{$OpenStudio_Dir}/qtwinmigrate.dll")
else
  # Do something here for Mac OSX environments
end

# require openstudio.so
require '${OPENSTUDIO_LIB}'

OpenStudio::Application::instance.application(true)
OpenStudio::Application::instance.application.setOrganizationName("NREL")
OpenStudio::Application::instance.application.setOrganizationDomain("nrel.gov")
OpenStudio::Application::instance.application.setApplicationName("OpenStudio")

$OPENSTUDIO_SKETCHUPPLUGIN_NAME = "OpenStudio Development"
ext = SketchupExtension.new($OPENSTUDIO_SKETCHUPPLUGIN_NAME, "${RUBY_SOURCE_DIR}/openstudio/sketchup_plugin/lib/PluginManager.rb")
ext.name = $OPENSTUDIO_SKETCHUPPLUGIN_NAME

# 'true' automatically loads the extension the first time it is registered, e.g., after install
Sketchup.register_extension(ext, true)

