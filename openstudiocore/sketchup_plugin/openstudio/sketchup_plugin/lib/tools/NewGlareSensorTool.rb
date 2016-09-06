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

require("lib/tools/Tool")
require("lib/interfaces/GlareSensor")

module OpenStudio

  class NewGlareSensorTool < Tool
  
    def initialize
      @cursor = UI.create_cursor(Plugin.dir + "/lib/resources/icons/OriginToolCursor-14x20.tiff", 3, 3)
    end
    
    def onMouseMove(flags, x, y, view)
      super
      # Should apply user's precision setting here   --automatically done, I think
      # Also:  show relative coordinates?
      Sketchup.set_status_text("Select a point to insert the Glare Sensor = " + @ip.position.to_s)
      view.tooltip = "New Glare Sensor"
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
        UI.messagebox "You need to be in a Space to add a Glare Sensor."
        Sketchup.send_action("selectSelectionTool:")
        return false
      end
      
      had_observers = this_space.remove_observers

      begin
      
        model_interface.start_operation("New Glare Sensor", true)

        initial_position = @ip.position
        if @ip.face
          # bump up or in by 30" if placed on a face
          distance = @ip.face.normal
          distance.length = 30.0
          initial_position = initial_position - distance
        end

        glare_sensor = GlareSensor.new
        glare_sensor.create_model_object
        glare_sensor.model_object.setSpace(this_space.model_object)
        glare_sensor.model_object_transformation = this_space.coordinate_transformation.inverse * Geom::Transformation::translation(initial_position)

        # no primary/secondary sensors
        #thermal_zone = this_space.model_object.thermalZone
        #if not thermal_zone.empty?
        #  if thermal_zone.get.primaryDaylightingControl.empty?
        #    thermal_zone.get.setPrimaryDaylightingControl(daylighting_control.model_object)
        #  elsif thermal_zone.get.secondaryDaylightingControl.empty?
        #    thermal_zone.get.setSecondaryDaylightingControl(daylighting_control.model_object)
        #  end
        #end

        glare_sensor.draw_entity
        glare_sensor.add_observers
        glare_sensor.add_watcher

      ensure
        
        model_interface.commit_operation
      
      end
      
      this_space.add_observers if had_observers
      
      # selection observers will ignore signals because selection tool is not yet active
      model_interface.skp_model.selection.clear
      model_interface.skp_model.selection.add(glare_sensor.entity)
      Plugin.dialog_manager.selection_changed
      
      # pick selection tool after changing selection
      Sketchup.send_action("selectSelectionTool:")
    end

  end

end
