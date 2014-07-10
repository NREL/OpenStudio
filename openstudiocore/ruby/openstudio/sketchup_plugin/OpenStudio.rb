######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

require 'extensions.rb'   # defines the SketchupExtension class

# look for the new plugin
new_plugin = Sketchup.find_support_file("Startup.rb", "Plugins/OpenStudio")
new_version = nil
if new_plugin

  # peek at Startup.rb to figure out version
  new_version = "Unknown"
  File.open(new_plugin, 'r') do |file|
    while line = file.gets
      if matchdata = /\$OPENSTUDIO_SKETCHUPPLUGIN_VERSION = \"([^\.]+\.[^\.]+\.[^\.]+).*?\"/.match(line)
        new_version = matchdata[1]
        break
      end
    end
  end
  
  $OPENSTUDIO_SKETCHUPPLUGIN_NAME = "OpenStudio"
  $OPENSTUDIO_SKETCHUPPLUGIN_VERSION_BRIEF = new_version
  $OPENSTUDIO_SKETCHUPPLUGIN_LAUNCH_GETTING_STARTED_ON_START = false
end

# look for the old plugin
old_plugin = Sketchup.find_support_file("Energy+.idd", "Plugins/OpenStudio")
old_version = nil
if old_plugin
  
  # peek at Idd to figure out version
  old_version = "Unknown"
  File.open(old_plugin, 'r') do |file|
    line = file.gets
    if /IDD_Version 4\.0\.0/.match(line)
      old_version = "1.0.4"
    elsif /IDD_Version 5\.0\.0/.match(line)
      old_version = "1.0.5"
    elsif /IDD_Version 6\.0\.0/.match(line)
      old_version = "1.0.6"
    elsif /IDD_Version 7\.0\.0/.match(line)
      old_version = "1.0.7"
     elsif /IDD_Version 7\.1\.0/.match(line)
      old_version = "1.0.8"     
     elsif /IDD_Version 7\.2\.0/.match(line)
      old_version = "1.0.9"     
     elsif /IDD_Version 8\.0\.0/.match(line)
      old_version = "1.0.10"     
     elsif /IDD_Version 8\.1\.0/.match(line)
      old_version = "1.0.11"     
    end
  end
  
  OPENSTUDIO_PLUGIN_NAME = "Legacy OpenStudio"
  OPENSTUDIO_PLUGIN_VERSION = old_version
end

# class for selecting version of OpenStudio
module OpenStudio
  class VersionSelector
  
    def VersionSelector.select_version

      current_load_old_plugin = Sketchup.read_default("OpenStudio", "Load Old Plugin")

      old_plugin_text = "Legacy OpenStudio #{OPENSTUDIO_PLUGIN_VERSION}"
      new_plugin_text = "OpenStudio #{$OPENSTUDIO_SKETCHUPPLUGIN_VERSION_BRIEF}"

      prompts = ["OpenStudio Version"]
      if current_load_old_plugin
        defaults = [old_plugin_text]
      else
        defaults = [new_plugin_text]
      end
      list = ["#{old_plugin_text}|#{new_plugin_text}"]

      input = UI.inputbox(prompts, defaults, list, "Select OpenStudio Version.")
      if input
        if input[0] == old_plugin_text
          load_old_plugin = Sketchup.write_default("OpenStudio", "Load Old Plugin", true)
          if not current_load_old_plugin
            UI.messagebox("Restart SketchUp for changes to take effect", MB_OK)
          end
        else
          load_old_plugin = Sketchup.write_default("OpenStudio", "Load Old Plugin", false)
          if current_load_old_plugin
            UI.messagebox("Restart SketchUp for changes to take effect", MB_OK)
          end            
        end
      end
    end
  end
end

# check current settings
load_old_plugin = Sketchup.read_default("OpenStudio", "Load Old Plugin")
first_launch = load_old_plugin.nil?
if first_launch
  load_old_plugin = false
  
  result = UI.messagebox("Welcome to OpenStudio SketchUp Plug-in #{$OPENSTUDIO_SKETCHUPPLUGIN_VERSION}\nSwitch between the new and legacy versions of the plug-in using 'Plugins->Select OpenStudio Version'\nYes to see the getting started guide now.\nNo to skip this message next time.\nCancel to do nothing.\n", MB_YESNOCANCEL )
  if result == 2 # CANCEL
    # nothing
  elsif result == 6 # YES
    $OPENSTUDIO_SKETCHUPPLUGIN_LAUNCH_GETTING_STARTED_ON_START = true
  elsif result == 7 # NO
    Sketchup.write_default("OpenStudio", "Load Old Plugin", false)
  end
  
end

# if have both plugins, add menu item for switching
if new_plugin and old_plugin
    
  # add menu item to switch versions
  switch_version_cmd = UI::Command.new("Select OpenStudio Version") { OpenStudio::VersionSelector::select_version }
  switch_version_cmd.set_validation_proc { MF_ENABLED }
  UI.menu("Plugins").add_item(switch_version_cmd)

elsif new_plugin
  load_old_plugin = false
elsif old_plugin
  load_old_plugin = true
end


# load one plugin or the other
if load_old_plugin and old_plugin

  ext = SketchupExtension.new(OPENSTUDIO_PLUGIN_NAME, "OpenStudio/lib/Startup.rb")
  ext.name = OPENSTUDIO_PLUGIN_NAME
  ext.description = "Adds building energy modeling capabilities by coupling SketchUp to the EnergyPlus simulation engine.  \r\n\r\nVisit www.energyplus.gov for more information."
  ext.version = old_version
  ext.creator = "National Renewable Energy Laboratory"
  ext.copyright = "2008-2014, Alliance for Sustainable Energy"

  # 'true' automatically loads the extension the first time it is registered, e.g., after install
  Sketchup.register_extension(ext, true)
  
elsif new_plugin
  
  ext = SketchupExtension.new($OPENSTUDIO_SKETCHUPPLUGIN_NAME, "OpenStudio/Startup.rb")
  ext.name = $OPENSTUDIO_SKETCHUPPLUGIN_NAME
  ext.description = "Adds building energy modeling capabilities by coupling SketchUp to the OpenStudio suite of tools.  \r\n\r\nVisit openstudio.nrel.gov for more information."
  ext.version = new_version
  ext.creator = "National Renewable Energy Laboratory"
  ext.copyright = "2008-2014, Alliance for Sustainable Energy"

  # 'true' automatically loads the extension the first time it is registered, e.g., after install
  Sketchup.register_extension(ext, true)

end
