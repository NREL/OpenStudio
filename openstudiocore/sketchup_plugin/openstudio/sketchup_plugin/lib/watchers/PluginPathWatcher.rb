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

module OpenStudio

  class PluginPathWatcher < PathWatcher
  
    def initialize(model_interface, path)
      super(path)
      
      @model_interface = model_interface
    end

    def onPathChanged
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")

      # ignore any further signals until we finish processing
      disable

      skp_model = @model_interface.skp_model

      if (not skp_model.valid?)

        Plugin.log(OpenStudio::Debug, "skp_model #{skp_model} is not valid, active model is #{Sketchup.active_model}")

        # skp_model is no longer valid (e.g. closed on Mac)
        proc = Proc.new { Plugin.model_manager.purge_invalid_model_interfaces }
        Plugin.add_event( proc )
        
        # continue ignoring

      elsif skp_model != Sketchup.active_model

        Plugin.log(OpenStudio::Debug, "skp_model #{skp_model} is not active_model, active model is #{Sketchup.active_model}")

        # no-op to avoid a crash when you try to update a model that is not the active model

        # re-enable the watcher
        enable

      else

        success = false
        result = UI.messagebox("Another application has updated #{self.path.to_s}, do you want to reload it?", MB_YESNO)
        if result == 6 # Yes
          success = Plugin.model_manager.open_openstudio(self.path.to_s, Sketchup.active_model)
        end

        if success
          # continue ignoring, new watcher will take over
        else
          # re-enable the watcher
          enable
        end
      end

    end
    
  end
end
