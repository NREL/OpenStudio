######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

$OPENSTUDIO_SKETCHUPPLUGIN_VERSION = "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}.${PROJECT_VERSION_BUILD}"
$OPENSTUDIO_SKETCHUPPLUGIN_DEVELOPER_MENU = false

begin
  filedir = File.dirname(__FILE__) 
  require "#{filedir}/../../../../bin/openstudio"
  $: << "#{filedir}/../../../"

  minimum_version = ''
  minimum_version_key = ''
  if (RUBY_PLATFORM =~ /mswin/ || RUBY_PLATFORM =~ /mingw/)  # Windows
    minimum_version = '14.0.0000'
    minimum_version_key = '001400000000'
  elsif (RUBY_PLATFORM =~ /darwin/)  # Mac OS X
    minimum_version = '14.0.0000'
    minimum_version_key = '001400000000'
  end

  installed_version = Sketchup.version
  installed_version_key = ''; installed_version.split('.').each { |e| installed_version_key += e.rjust(4, '0') }

  if (installed_version_key < minimum_version_key)
    UI.messagebox("OpenStudio is only compatible with SketchUp version " + minimum_version +
      " or higher.\nThe installed version is " + installed_version + ".  The plugin was not loaded.", MB_OK)
  else
    load("openstudio/sketchup_plugin/lib/PluginManager.rb")
  end
rescue LoadError => e 
  UI.messagebox("Error loading OpenStudio SketchUp Plug-In:\n  #{e.message}", MB_OK)
end
