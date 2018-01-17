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
require("openstudio/sketchup_plugin/lib/interfaces/Surface")
require("openstudio/sketchup_plugin/lib/interfaces/SubSurface")
require("openstudio/sketchup_plugin/lib/dialogs/LastReportInterface")

module OpenStudio

  class LooseGeometryDialog < PropertiesDialog

    def initialize(container, interface, hash)
      super
      @container = WindowContainer.new("Project Loose Geometry", 300, 200, 150, 150)
      @container.set_file(Plugin.dir + "/lib/dialogs/html/LooseGeometry.html")

      @last_report = ""

      add_callbacks
    end

    def add_callbacks
      super
      @container.web_dialog.add_action_callback("on_project_all") { on_project_all }
      @container.web_dialog.add_action_callback("on_project_selected") { on_project_selected }
      @container.web_dialog.add_action_callback("on_cancel") { on_cancel }
    end

    def on_load
      super
    end

    def on_project_selected
      model = Plugin.model_manager.model_interface.skp_model
      # store original selection (not using this right now)
      orig_selection = []
      model.selection.each {|e| orig_selection.push(e)}

      project(model.selection)
    end

    def on_project_all
      # select all
      model = Plugin.model_manager.model_interface.skp_model
      model.entities.each {|e| model.selection.add(e)}

      project(model.selection)
    end

    def project(selection)
      # cancel if there is no selection
      if selection.empty?
        UI.messagebox("Selection is empty, please select objects for projection routine or choose 'Project All Loose Geometry'.")
        return
      end

      skp_model = Sketchup.active_model

      # only top level loose geometry can be intersected
      skp_model.entities.each do |ent|
        case ent.typename
          when "ComponentInstance"
            selection.remove(ent)
          when "Group"
            selection.remove(ent)
        end
      end
      if selection.empty?
        UI.messagebox("No top level loose geometry in selection.")
        return
      end

      # offer user chance to cancel
      result = UI.messagebox(
"Warning this will create new geometry in your spaces.\n
This operation cannot be undone. Do you want to continue?", MB_OKCANCEL)

      if result == 2 # cancel
        return false
      end

      model_interface = Plugin.model_manager.model_interface

      # pause event processing
      event_processing_stopped = Plugin.stop_event_processing

      # store starting render mode
      starting_rendermode = model_interface.materials_interface.rendering_mode

      # switch render mode to speed things up
      model_interface.materials_interface.rendering_mode = RenderWaiting

      # get all spaces
      spaces = model_interface.spaces.to_a

      model_interface.remove_observers(true)
      begin
        # start an operation
        model_interface.start_operation("Hide OpenStudio Geometry", true)

        # temporarily hide all OpenStudio Objects will unhide one at a time in loop
        model_interface.spaces.each { |group| group.entity.visible = false }
        model_interface.shading_surface_groups.each { |group| group.entity.visible = false }
        model_interface.interior_partition_surface_groups.each { |group| group.entity.visible = false }
        model_interface.illuminance_maps.each { |group| group.entity.visible = false }
        model_interface.daylighting_controls.each { |group| group.entity.visible = false }
        model_interface.luminaires.each { |group| group.entity.visible = false }
      ensure
        model_interface.commit_operation
      end
      model_interface.add_observers(true)

      # DLM: cannot include this section in the operation
      # creating a lot of subsurfaces in an operation appears to create problems when multiple surfaces
      # are swapped simultaneously, need more testing to understand this

      # iterate through spaces to create intersecting geometry
      spaces.each_index do |index|
        space_entity = spaces[index].entity
        space_entity.visible = true
        space_entity.entities.intersect_with(true, space_entity.transformation, space_entity.entities.parent, space_entity.transformation, false, selection.to_a)
        space_entity.visible = false
      end

      model_interface.remove_observers(true)
      begin
        # start an operation
        model_interface.start_operation("Unhide OpenStudio Geometry", true)

        # unhide everything
        model_interface.spaces.each { |group| group.entity.visible = true }
        model_interface.shading_surface_groups.each { |group| group.entity.visible = true }
        model_interface.interior_partition_surface_groups.each { |group| group.entity.visible = true }
        model_interface.illuminance_maps.each { |group| group.entity.visible = true }
        model_interface.daylighting_controls.each { |group| group.entity.visible = true }
        model_interface.luminaires.each { |group| group.entity.visible = true }

        # create or confirm layer called "OS Projected Geometry" exists
        layers = skp_model.layers
        new_layer = layers.add("OpenStudio - Loose Geometry")

        # turn off layer visibility
        new_layer.visible = false

        # make group out of selection and put onto OS Loose Geometry Layer
        loose_geometry = skp_model.entities.add_group(selection)
        loose_geometry.layer = new_layer
      ensure
        model_interface.commit_operation
      end
      model_interface.add_observers(true)

      # switch render mode back to original
      proc = Proc.new { model_interface.materials_interface.rendering_mode = starting_rendermode }
      Plugin.add_event( proc )

      # resume event processing
      Plugin.start_event_processing if event_processing_stopped

    end

    def on_cancel
      close
    end

  end

end
