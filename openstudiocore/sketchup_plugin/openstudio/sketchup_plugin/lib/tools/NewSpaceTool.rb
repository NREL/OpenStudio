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

require("openstudio/sketchup_plugin/lib/interfaces/Space")
require("openstudio/sketchup_plugin/lib/tools/NewGroupTool")


module OpenStudio

  class NewSpaceTool < NewGroupTool

    def onMouseMove(flags, x, y, view)
      super
      # Should apply user's precision setting here   --automatically done, I think
      # Also:  show relative coordinates?
      Sketchup.set_status_text("Select a point to become the New Space Origin = " + @ip.position.to_s)
      view.tooltip = "New Space"
    end


    def onLButtonUp(flags, x, y, view)
      super
 
      model_interface = Plugin.model_manager.model_interface
      
      if not model_interface.skp_model.active_path.nil?
        UI.messagebox("Space should be added at the top level of a SketchUp model")
        Sketchup.send_action("selectSelectionTool:")
        return false
      end
      
      had_observers = model_interface.building.remove_observers
      
      begin
      
        model_interface.start_operation("New Space", true)

        # input point is in absolute coordinates
        initial_position = @ip.position

        space = Space.new
        space.create_model_object
        space.model_object.setXOrigin(initial_position.x.to_m)
        space.model_object.setYOrigin(initial_position.y.to_m)
        space.model_object.setZOrigin(initial_position.z.to_m)
        space.draw_entity
        space.create_initial_box("#{$OPENSTUDIO_SKETCHUPPLUGIN_DIR}/resources/components/OpenStudio_NewSpace.skp")
        space.add_observers
        space.add_watcher     
      
      ensure
        
        model_interface.commit_operation
      
      end
      
      model_interface.building.add_observers if had_observers
      
      # selection observers will ignore signals because selection tool is not yet active
      model_interface.skp_model.selection.clear
      model_interface.skp_model.selection.add(space.entity)
      Plugin.dialog_manager.selection_changed
      
      # pick selection tool after changing selection
      Sketchup.send_action("selectSelectionTool:")
     
    end

  end

end
