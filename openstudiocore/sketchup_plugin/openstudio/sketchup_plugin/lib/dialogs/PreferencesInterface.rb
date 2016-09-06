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

require("lib/dialogs/DialogInterface")
require("lib/dialogs/PreferencesDialog")


module OpenStudio

  class PreferencesInterface < DialogInterface

    def initialize
      super
      @dialog = PreferencesDialog.new(nil, self, @hash)
    end


    def populate_hash 
      @hash['CHECK_FOR_UPDATE'] = Plugin.read_pref("Check For Update #{Plugin.version}")
      @hash['NEW_ZONE_FOR_SPACE'] = Plugin.read_pref("New Zone for Space")
      @hash['DISABLE_USER_SCRIPTS'] = Plugin.read_pref("Disable OpenStudio User Scripts")
      @hash['UNIT_SYSTEM'] = Plugin.read_pref("Unit System")
      @hash['TEXT_EDITOR_PATH'] = Plugin.read_pref("Text Editor Path")
      
      @hash['SHOW_ERRORS_ON_IDF_TRANSLATION'] = Plugin.read_pref("Show Errors on Idf Translation")
      @hash['SHOW_WARNINGS_ON_IDF_TRANSLATION'] = Plugin.read_pref("Show Warnings on Idf Translation")
      @hash['ENERGYPLUS_PATH'] = Plugin.read_pref("EnergyPlus Path")
    end


    def report
      
      path = @hash['TEXT_EDITOR_PATH']
      # Should filter out any arguments that get passed for line number, etc.
      # For example:  textpad.exe -l%1 -p
      if (path.nil? or path.empty?)
        # do nothing
      elsif (not File.exists?(path))  
        UI.messagebox("WARNING:  Bad file path for the text editor.")
      end

      # DLM: this is no longer needed as simulations are not run from the plugin
      #path = @hash['ENERGYPLUS_PATH']
      #if (path.nil? or path.empty?)
      #  # do nothing
      #elsif (not File.exists?(path))  
      #  UI.messagebox("WARNING:  Bad file path for the EnergyPlus engine.")
      #else
      #  idd_path = File.dirname(path) + "/Energy+.idd"
      #  if (not File.exists?(idd_path))
      #    UI.messagebox("WARNING:  Cannot locate the input data dictionary (IDD) in the EnergyPlus directory.")
      #    #@hash['ENERGYPLUS_PATH'] = Plugin.read_pref("EnergyPlus Path")
      #    #@dialog.update
      #    #return(false)
      #  else
      #  
      #    # check idd version
      #    user_version = "Unknown"
      #    File.open(idd_path, 'r') do |file|
      #      line = file.gets
      #      if md = /IDD_Version (\d+\.\d+\.\d+)/.match(line)
      #        user_version = md[1]
      #      end
      #    end
      #
      #    if (user_version != Plugin.energyplus_version)
      #      UI.messagebox("WARNING:  The EnergyPlus engine you have specified is version " + user_version + ".  The plugin is designed for version " +
      #        Plugin.energyplus_version + ".\nThere might be problems with compatibility. Try updating your EnergyPlus engine if there are a lot of simulation errors.")
      #    end
      #  end
      #end
      
      need_update = false
      if @hash['SHOW_WARNINGS_ON_IDF_EXPORT'] and not @hash['SHOW_ERRORS_ON_IDF_EXPORT']
        @hash['SHOW_ERRORS_ON_IDF_EXPORT'] = true
        need_update = true
      end

      Plugin.write_pref("Check For Update #{Plugin.version}", @hash['CHECK_FOR_UPDATE'])
      Plugin.write_pref("New Zone for Space", @hash['NEW_ZONE_FOR_SPACE'])
      Plugin.write_pref("Disable OpenStudio User Scripts", @hash['DISABLE_USER_SCRIPTS'])
      Plugin.write_pref("Unit System", @hash['UNIT_SYSTEM'])
      Plugin.write_pref("Text Editor Path", @hash['TEXT_EDITOR_PATH'])

      Plugin.write_pref("Show Errors on Idf Translation", @hash['SHOW_ERRORS_ON_IDF_TRANSLATION'])
      Plugin.write_pref("Show Warnings on Idf Translation", @hash['SHOW_WARNINGS_ON_IDF_TRANSLATION'])
      Plugin.write_pref("EnergyPlus Path", @hash['ENERGYPLUS_PATH'])
      
      if (@hash['UNIT_SYSTEM'] != Plugin.dialog_manager.units_system)
        Plugin.dialog_manager.update_units
      end
      
      update if need_update

      return(true)
    end

  end

end
