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

  model_interface = Plugin.model_manager.model_interface
  skp_model = Sketchup.active_model

  # make space type list
  space_type_objects = model_interface.openstudio_model.getSpaceTypes
  space_type_raw = []
  space_type_list = ["<no change>"]
  space_type_objects.each do |object|
    space_type_raw.push object.name.to_s
  end
  space_type_raw.sort.each do |index|
    space_type_list.push index
  end
  space_type_list.push "<clear field>"
  space_type_list.push "<new space type>"

  # make thermal zone list
  thermal_zone_objects = model_interface.openstudio_model.getThermalZones
  thermal_zone_raw = []
  thermal_zone_list = ["<no change>"]
  thermal_zone_objects.each do |object|
    thermal_zone_raw.push object.name.to_s
  end
  thermal_zone_raw.sort.each do |index|
    thermal_zone_list.push index
  end
  thermal_zone_list.push "<clear field>"
  thermal_zone_list.push "<new thermal zone>"

  # make building story list
  building_story_objects = model_interface.openstudio_model.getBuildingStorys
  building_story_raw = []
  building_story_list = ["<no change>"]
  building_story_objects.each do |object|
    building_story_raw.push object.name.to_s
  end
  building_story_raw.sort.each do |index|
    building_story_list.push index
  end
  building_story_list.push "<clear field>"
  building_story_list.push "<new story>"

  # make default construction list
  const_set_objects = model_interface.openstudio_model.getDefaultConstructionSets
  const_set_raw = []
  const_set_list = ["<no change>"]
  const_set_objects.each do |object|
    const_set_raw.push object.name.to_s
  end
  const_set_raw.sort.each do |index|
    const_set_list.push index
  end
  const_set_list.push "<clear field>"
  const_set_list.push "<new construction set>"

  # make ideal air loads state list
  ideal_loads_list = ["<no change>", "Yes", "No"]

  # make thermostat list
  thermostat_objects = model_interface.openstudio_model.getThermostatSetpointDualSetpoints # singular or plural
  thermostat_raw = []
  thermostat_list = ["<no change>"]
  thermostat_objects.each do |object|
    thermostat_raw.push object.name.to_s
  end
  thermostat_raw.sort.each do |index|
    thermostat_list.push index
  end
  thermostat_list.push "<clear field>"

  # build UI element
  prompts = ["Space Type", "Building Story", "Construction Set", "Thermal Zone", "Set Parent Thermal Zone's - Ideal Air Loads Status", "Set Parent Thermal Zone's - Thermostat"]
  defaults = ["<no change>", "<no change>", "<no change>", "<no change>", "<no change>", "<no change>"]
  # added the spaces in the join statement below to pad the inputbox so the name isn't cut off May need to trim later before apply values
  list = [space_type_list.join("|"), building_story_list.join("|"), const_set_list.join("|"), thermal_zone_list.join("|"), ideal_loads_list.join("|"), thermostat_list.join("|")]

  input = UI.inputbox(prompts, defaults, list, "Set Attributes for Selected Spaces")

  if input
    
    # pause event processing
    event_processing_stopped = Plugin.stop_event_processing
    
    # store starting render mode
    starting_rendermode = model_interface.materials_interface.rendering_mode

    # switch render mode to speed things up
    model_interface.materials_interface.rendering_mode = RenderWaiting
      
    begin
      progress_dialog = ProgressDialog.new("Applying Attributes to Selected Spaces")

      # user selected attributes
      selected_space_type = input[0]
      selected_building_story = input[1]
      selected_const_set = input[2]
      selected_thermal_zone = input[3]
      ideal_loads = input[4]
      thermostat = input[5]
      # if statements for special cases (don't need if ro "<no change>" unless that happens to be name of object in model. It is invalid and won't be applied
      if selected_space_type == "<clear field>" then selected_space_type = "" end
      if selected_thermal_zone == "<clear field>" then selected_thermal_zone = "" end
      if selected_building_story == "<clear field>" then selected_building_story = "" end
      if selected_const_set == "<clear field>" then selected_const_set = "" end
      if thermostat == "<clear field>" then thermostat = "" end

      # create new objects
      if selected_space_type == "<new space type>"
        # create and assign new space type
        newspacetype = OpenStudio::Model::SpaceType.new(model_interface.openstudio_model)
      end
      if selected_thermal_zone == "<new thermal zone>"
        # create and assign new thermal zone
        newthermalzone = OpenStudio::Model::ThermalZone.new(model_interface.openstudio_model)
      end
      if selected_building_story == "<new story>"
        # create and assign new building story
        newstory = OpenStudio::Model::BuildingStory.new(model_interface.openstudio_model)
      end
      if selected_const_set == "<new construction set>"
        # create and assign new construction set
        newconstset = OpenStudio::Model::DefaultConstructionSet.new(model_interface.openstudio_model)
      end

      # get selection
      selection = skp_model.selection

      num_total = selection.length
      num_complete = 0

      # loop through selection
      selection.each do |entity|
        drawing_interface = entity.drawing_interface

        if drawing_interface.class.to_s == "OpenStudio::Space"
          if selected_space_type == "<new space type>"
            drawing_interface.model_object.setSpaceType(newspacetype)
          else
            assigned_space_type = drawing_interface.model_object.setString(2,selected_space_type.to_s)
          end
          if selected_thermal_zone == "<new thermal zone>"
            drawing_interface.model_object.setThermalZone(newthermalzone)
          else
            assigned_thermal_zone = drawing_interface.model_object.setString(10,selected_thermal_zone.to_s)
          end
          if selected_building_story == "<new story>"
            drawing_interface.model_object.setBuildingStory(newstory)
          else
            assigned_building_story = drawing_interface.model_object.setString(9,selected_building_story.to_s)
          end
          if selected_const_set == "<new construction set>"
            drawing_interface.model_object.setDefaultConstructionSet(newconstset)
          else
            assigned_const_set = drawing_interface.model_object.setString(3,selected_const_set.to_s)
          end

          # set space's parent zone values
          # (I could probalby make array of unique values and loop outside of main loop so I don't reset the string over and over for spaces that share zones)

          parent_zone = drawing_interface.model_object.thermalZone
          if not parent_zone.empty?

            # assign thermostat to parent zone object
            assigned_thermostat_status = parent_zone.get.setString(19,thermostat.to_s)
            #puts "thermostat, #{thermostat.to_s}, #{assigned_thermostat_status}"

            # assign parent zone's ideal air loads status
            if ideal_loads.to_s == "Yes"
              parent_zone.get.setUseIdealAirLoads(true)
            elsif ideal_loads.to_s == "No"
              parent_zone.get.setUseIdealAirLoads(false)
            end
          end
        end

        num_complete += 1

        progress_dialog.setValue((100*num_complete)/num_total)
        
      end

    ensure

      progress_dialog.destroy
    end

    # switch render mode back to original
    proc = Proc.new { model_interface.materials_interface.rendering_mode = starting_rendermode }
    Plugin.add_event( proc )
    
    # resume event processing
    Plugin.start_event_processing if event_processing_stopped
    
  else
    #puts "User clicked Cancel - do nothing"
  end

end
