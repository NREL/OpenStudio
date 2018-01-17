########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

require("openstudio/sketchup_plugin/lib/tools/Tool")
require("openstudio/sketchup_plugin/lib/interfaces/IlluminanceMap")

module OpenStudio

  class NewIlluminanceMapTool < Tool
  
    def initialize
      @cursor = UI.create_cursor(Plugin.dir + "/lib/resources/icons/OriginToolCursor-14x20.tiff", 3, 3)
    end
    
    def onMouseMove(flags, x, y, view)
      super
      # Should apply user's precision setting here   --automatically done, I think
      # Also:  show relative coordinates?
      Sketchup.set_status_text("Select a point to insert the Output:Illuminance Map = " + @ip.position.to_s)
      view.tooltip = "New Output:IlluminanceMap"
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
        UI.messagebox "You need to be in a Space to add an Illuminance Map"
        Sketchup.send_action("selectSelectionTool:")
        return false
      end

      had_observers = this_space.remove_observers
      
      begin
      
        model_interface.start_operation("New Illuminance Map", true)

        initial_position = @ip.position
        if @ip.face
          # bump up or in by 30" if placed on a face
          distance = @ip.face.normal
          distance.length = 30.0
          initial_position = initial_position - distance
        end

        illuminance_map = IlluminanceMap.new
        illuminance_map.create_model_object
        illuminance_map.model_object.setSpace(this_space.model_object)
        illuminance_map.model_object_transformation = this_space.coordinate_transformation.inverse * Geom::Transformation::translation(initial_position)

        thermal_zone = this_space.model_object.thermalZone
        if not thermal_zone.empty?
          if thermal_zone.get.illuminanceMap.empty?
            thermal_zone.get.setIlluminanceMap(illuminance_map.model_object)
          end
        end

        illuminance_map.draw_entity
        illuminance_map.add_observers
        illuminance_map.add_watcher
      
      ensure
        
        model_interface.commit_operation
      
      end
      
      this_space.add_observers if had_observers
      
      # selection observers will ignore signals because selection tool is not yet active
      model_interface.skp_model.selection.clear
      model_interface.skp_model.selection.add(illuminance_map.entity)
      Plugin.dialog_manager.selection_changed
      
      # pick selection tool after changing selection
      Sketchup.send_action("selectSelectionTool:")

    end

  end

end
