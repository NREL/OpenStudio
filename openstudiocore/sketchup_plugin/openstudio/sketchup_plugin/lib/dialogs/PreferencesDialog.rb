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

require("openstudio/sketchup_plugin/lib/dialogs/Dialogs")
require("openstudio/sketchup_plugin/lib/dialogs/DialogContainers")


module OpenStudio

  class PreferencesDialog < PropertiesDialog

    def initialize(container, interface, hash)
      super
      w = Plugin.platform_select(550, 600)
      h = Plugin.platform_select(550, 600)
      @container = WindowContainer.new("Preferences", w, h, 150, 220)
      @container.set_file(Plugin.dir + "/lib/dialogs/html/Preferences.html")

      add_callbacks
    end


    def add_callbacks
      super
      @container.web_dialog.add_action_callback("on_browse_text_editor") { browse_text_editor }
      @container.web_dialog.add_action_callback("on_browse_exe") { browse_exe }
    end


    def browse_text_editor
      path = @hash['TEXT_EDITOR_PATH']

      if (path.nil? or path.empty?)
        path = Plugin.default_preferences['Text Editor Path']
      end

      dir = File.dirname(path)
      file_name = File.basename(path)

      if (not File.exist?(dir))
        dir = ""
      end

      if (path = UI.open_panel("Locate Text Editor Program", dir, file_name))
        path = path.split("\\").join("/")  # Have to convert the file separator for other stuff to work later
        # Above is a kludge...should allow any separators to be cut and paste into the text box
        @hash['TEXT_EDITOR_PATH'] = path
        update
      end
    end

    def browse_exe
      path = @hash['ENERGYPLUS_PATH']

      if (path.nil? or path.empty?)
        path = Plugin.default_preferences['EnergyPlus Path']
      end

      dir = File.dirname(path)
      file_name = File.basename(path)

      if (not File.exist?(dir))
        dir = ""
      end

      if (path = UI.open_panel("Locate EnergyPlus Program", dir, file_name))
        path = path.split("\\").join("/")  # Have to convert the file separator for other stuff to work later
        # Above is a kludge...should allow any separators to be cut and paste into the text box

        @hash['ENERGYPLUS_PATH'] = path
        update
      end
    end

  end
  
end
