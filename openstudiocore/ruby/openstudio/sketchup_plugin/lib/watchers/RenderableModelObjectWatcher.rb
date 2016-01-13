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

require("openstudio/sketchup_plugin/lib/watchers/PluginModelObjectWatcher")

module OpenStudio

  class RenderableModelObjectWatcher < PluginModelObjectWatcher
  
    def initialize(drawing_interface, interface_to_paint, rendering_indices, rendering_modes)
      super(drawing_interface)
      
      @interface_to_paint = interface_to_paint
      @rendering_indices = rendering_indices
      @rendering_modes = rendering_modes
    end

    #def onChangeIdfObject
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super 
    #end
    
    #def onDataFieldChange
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super 
    #end
    
    #def onNameChange
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super 
    #end

    #def onBecomeDirty
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super
    #end

    #def onBecomeClean
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super 
    #end

    #def clearState
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super   
    #end
  
    def onRelationshipChange(index, newHandle, oldHandle)
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      super

      if i = @rendering_indices.index(index)
        if @drawing_interface.model_interface.materials_interface.rendering_mode == @rendering_modes[i]
          @interface_to_paint.request_paint
        end
      end
    end
    
    #def onRemoveFromWorkspace(handle)
    #  Plugin.log(OpenStudio::Trace, "#{current_method_name}")
    #  super
    #end
    
  end
end

