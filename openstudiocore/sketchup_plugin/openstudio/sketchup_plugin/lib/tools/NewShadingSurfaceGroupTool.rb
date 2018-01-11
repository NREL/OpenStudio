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

require("openstudio/sketchup_plugin/lib/interfaces/ShadingSurfaceGroup")
require("openstudio/sketchup_plugin/lib/tools/NewGroupTool")


module OpenStudio

  class NewShadingSurfaceGroupTool < NewGroupTool

    def onMouseMove(flags, x, y, view)
      super
      Sketchup.set_status_text("Select a point to become the New Shading Group")
      view.tooltip = "New Shading Group"
    end


    def onLButtonUp(flags, x, y, view)
      super
      
      model_interface = Plugin.model_manager.model_interface
      
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
      
      if not model_interface.skp_model.active_path.nil? and not this_space
        UI.messagebox("Shading Surface Group should be added at the top level of a SketchUp model or directly under a Space")
        Sketchup.send_action("selectSelectionTool:")
        return false
      end
      
      begin
      
        model_interface.start_operation("New Shading Surface Group", true)

        # input point is in absolute coordinates
        initial_position = nil

        had_observers = nil

        shading_group = ShadingSurfaceGroup.new
        shading_group.create_model_object
        if this_space
          had_observers = this_space.remove_observers

          shading_group.model_object.setSpace(this_space.model_object)
          initial_position = this_space.coordinate_transformation.inverse * @ip.position
          #initial_position = @ip.position
        else
          had_observers = model_interface.building.remove_observers

          initial_position = @ip.position
        end

        shading_group.model_object.setXOrigin(initial_position.x.to_m)
        shading_group.model_object.setYOrigin(initial_position.y.to_m)
        shading_group.model_object.setZOrigin(initial_position.z.to_m)
        shading_group.draw_entity
        shading_group.create_initial_box("#{$OPENSTUDIO_SKETCHUPPLUGIN_DIR}/resources/components/OpenStudio_NewShadingSurfaceGroup.skp")
        shading_group.add_observers
        shading_group.add_watcher
      
      ensure
      
        model_interface.commit_operation
      
      end
      
      if had_observers
        if this_space
          this_space.add_observers
        else
          model_interface.building.add_observers
        end
      end
      
      # selection observers will ignore signals because selection tool is not yet active
      model_interface.skp_model.selection.clear
      model_interface.skp_model.selection.add(shading_group.entity)
      Plugin.dialog_manager.selection_changed
      
      # pick selection tool after changing selection
      Sketchup.send_action("selectSelectionTool:")

    end
    
  end

end
