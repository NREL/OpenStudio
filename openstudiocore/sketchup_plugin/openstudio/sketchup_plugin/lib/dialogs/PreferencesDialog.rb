########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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
