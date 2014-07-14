######################################################################
#  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

  # get selection
  skp_model = Sketchup.active_model
  selection = skp_model.selection

  # unselect all OpenStudio Objects
  model_interface = Plugin.model_manager.model_interface
  model_interface.spaces.each { |group| selection.remove(group.entity) }
  model_interface.shading_surface_groups.each { |group| selection.remove(group.entity) }
  model_interface.interior_partition_surface_groups.each { |group| selection.remove(group.entity) }
  model_interface.illuminance_maps.each { |group| selection.remove(group.entity) }
  model_interface.daylighting_controls.each { |group| selection.remove(group.entity) }
  model_interface.luminaires.each { |group| selection.remove(group.entity) }

  # save selection
  saved_selection = []
  selection.each {|e| saved_selection << e}

  valid_diagram = true

  # canel if there is no selection (remove OpenStudio objects first)
  if selection.empty?
    UI.messagebox("No loose geometry is selected, please select objects to extrude into OpenStudio spaces")
    valid_diagram = false
  end

  # canel if all faces in selection are not horizontal, flip reversed faces
  expected_normal = Geom::Vector3d.new 0,0,-1
  inv_expected_normal = Geom::Vector3d.new 0,0,1
  status = 0
  selection.each do |index|
    if index.typename == "Face"
      if index.normal != expected_normal
        if index.normal == inv_expected_normal
          flip = index.reverse!
        else
          status = 1
        end
      end
    end
  end

  if status == 1
    UI.messagebox("Not all selected surfaces are horizontal, please limit selection to horizontal surfaces")
    valid_diagram = false
  end

  # sort SketchUp selection and pass this to OpenStudio
  # later add more complex sort that gives preference to adjacent surfaces if same "y" before looking to "x"
  # it woudl be nice to let use selection order do this but that would require a custom tool.
  sel_sort = Sketchup.active_model.selection.to_a
  sel_sort.delete_if {|e| not e.is_a?(Sketchup::Face) }
  sel_sort.sort! {|a,b|
    ([a.vertices.min{|v1,v2| v1.position.y <=> v2.position.y }.position.y,
      a.vertices.min{|v1,v2| v1.position.x <=> v2.position.x }.position.x] <=>
     [b.vertices.min{|v1,v2| v1.position.y <=> v2.position.y }.position.y,
      b.vertices.min{|v1,v2| v1.position.x <=> v2.position.x }.position.x] )
  }

  if valid_diagram # skip over if not valid selection for space diagram

    # UI
    prompts = ["Floor Height (SketchUp Units)   ","Number of Floors"]
    defaults = ["10'","1"]
    input = UI.inputbox(prompts, defaults, "Create Spaces From 2d Floor Plan")

    # this skips through the script if the user cancels without providing input
    valid_input = false

    # this will skip through the script if the user doesn't provide valid input
    if input
      valid_input = true
      height = input[0].to_l.to_m
      if height <= 0
        UI.messagebox("Height must be greater than 0 meters.")
        valid_input = false
      end

      num_floors = input[1].to_i
      if num_floors < 1
        UI.messagebox("Number of floors must be greater than or equal to 1.")
        valid_input = false
      end
    end

    if valid_input # skip over if not valid user input

      begin

        # pause event processing
        event_processing_stopped = Plugin.stop_event_processing

        # store starting render mode
        starting_rendermode = model_interface.materials_interface.rendering_mode

        # switch render mode to speed things up
        model_interface.materials_interface.rendering_mode = RenderWaiting

        # create a progress bar
        progress_dialog = ProgressDialog.new("Creating Spaces from Floorprint")

        faces = []
        sel_sort.each { |entity| faces << entity if entity.class == Sketchup::Face }

        num_total = faces.size * num_floors
        num_complete = 0

        # add loop to create multiple floors. Will need to create new stories and adjust z values
        for floor in (1..num_floors)

          # create story if user selected new
          story = OpenStudio::Model::BuildingStory.new(model_interface.openstudio_model)
          story.setNominalZCoordinate(height*(floor-1.0))
          story.setNominalFloortoFloorHeight(height)

          # rest room counter
          rm = 0

          # loop through faces in the selection
          faces.each do |face|

            verticies_pre_t = face.outer_loop.vertices

            # create new copy of points
            pts = []
            t = Geom::Transformation.new verticies_pre_t[0].position
            verticies_pre_t.each do |point_trans|
              test =  t.inverse * point_trans.position
              pts.push Geom::Point3d.new(test)
            end

            vertices = OpenStudio::Point3dVector.new
            pts.each do |point|
              vertices << OpenStudio::Point3d.new(point.x.to_m, point.y.to_m, point.z.to_m)
            end

            # make space model_object
            space = OpenStudio::Model::Space::fromFloorPrint(vertices, height, model_interface.openstudio_model)

            if not space.empty?

              space = space.get
              space.setXOrigin(verticies_pre_t[0].position.x.to_m)
              space.setYOrigin(verticies_pre_t[0].position.y.to_m)
              space.setZOrigin(verticies_pre_t[0].position.z.to_m + height*(floor-1))
              space.setBuildingStory(story)

              rm = rm + 1
              # set space name
              if faces.length < 100
                padded_room_number = "Space " + floor.to_s + "%02d" % rm
              else
                padded_room_number = "Space " + floor.to_s + "%03d" % rm
              end
              space.setName(padded_room_number)


              if Plugin.read_pref("New Zone for Space")
                thermal_zone = OpenStudio::Model::ThermalZone.new(model_interface.openstudio_model)
                space.setThermalZone(thermal_zone)
              end

            end

            num_complete += 1
            progress_dialog.setValue((100*num_complete)/num_total)

          end # end of selection loop

        end # end of floor loop

      ensure

        progress_dialog.destroy

      end

      # add surface matching here, maybe with its own progress bar

      # create or confirm layer called "OpenStudio - Space Diagrams" exists
      layers = skp_model.layers
      new_layer = layers.add("OpenStudio - Space Diagrams")

      # turn off layer visibility
      new_layer.visible  = false

      # make group out of selection and put onto OS Loose Geometry Layer
      thermal_diagram = Sketchup.active_model.entities.add_group(saved_selection)
      thermal_diagram.layer = new_layer

      # switch render mode back to original
      proc = Proc.new { model_interface.materials_interface.rendering_mode = starting_rendermode }
      Plugin.add_event( proc )

      # resume event processing
      Plugin.start_event_processing if event_processing_stopped

    else

       puts "User clicked Cancel - do nothing"

    end # valid_input

  end # valid_diagram

end
