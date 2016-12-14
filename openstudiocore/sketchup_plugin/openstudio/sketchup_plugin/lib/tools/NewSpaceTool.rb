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
