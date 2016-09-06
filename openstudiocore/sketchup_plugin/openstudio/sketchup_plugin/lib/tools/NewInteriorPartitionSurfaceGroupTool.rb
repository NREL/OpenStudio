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

require("lib/interfaces/InteriorPartitionSurfaceGroup")
require("lib/tools/NewGroupTool")


module OpenStudio

  class NewInteriorPartitionSurfaceGroupTool < NewGroupTool

    def onMouseMove(flags, x, y, view)
      super
      Sketchup.set_status_text("Select a point to become the New Interior Partition Surface Group")
      view.tooltip = "New Interior Partition Surface Group"
    end


    def onLButtonUp(flags, x, y, view)
      super
      
      model_interface = Plugin.model_manager.model_interface
      
      # look for this group in the spaces
      this_space = nil
      if model_interface.skp_model.active_path
        model_interface.spaces.each do |space|
          if space.entity == model_interface.skp_model.active_path[-1]
            # good
            this_space = space
            break
          end
        end
      end

      if not this_space
        UI.messagebox "You need to be in a Space to add an Interior Partition Surface Group"
        Sketchup.send_action("selectSelectionTool:")
        return false
      end      
      
      had_observers = this_space.remove_observers
      
      begin
      
        model_interface.start_operation("New Interior Partition Surface Group", true)

        # input point is in absolute coordinates
        initial_position = this_space.coordinate_transformation.inverse * @ip.position
        #initial_position = @ip.position

        partition_group = InteriorPartitionSurfaceGroup.new
        partition_group.create_model_object
        partition_group.model_object.setSpace(this_space.model_object)
        partition_group.model_object.setXOrigin(initial_position.x.to_m)
        partition_group.model_object.setYOrigin(initial_position.y.to_m)
        partition_group.model_object.setZOrigin(initial_position.z.to_m)      
        partition_group.draw_entity
        partition_group.create_initial_box("#{$OpenStudio_LibPath}openstudio/sketchup_plugin/lib/resources/components/OpenStudio_NewInteriorPartitionSurfaceGroup.skp")
        partition_group.add_observers
        partition_group.add_watcher
      
      ensure
      
        model_interface.commit_operation
      
      end
      
      this_space.add_observers if had_observers
      
      # selection observers will ignore signals because selection tool is not yet active
      model_interface.skp_model.selection.clear
      model_interface.skp_model.selection.add(partition_group.entity)
      Plugin.dialog_manager.selection_changed
      
      # pick selection tool after changing selection
      Sketchup.send_action("selectSelectionTool:")
      
    end
    
  end

end
