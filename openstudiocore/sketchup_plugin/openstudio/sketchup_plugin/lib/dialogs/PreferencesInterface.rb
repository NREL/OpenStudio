########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

require("openstudio/sketchup_plugin/lib/dialogs/DialogInterface")
require("openstudio/sketchup_plugin/lib/dialogs/PreferencesDialog")


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
