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

      # get model
      model = model_interface.openstudio_model

      # get thermostat object from input name
      thermostat_objects = model.getThermostats
      found_thermostat = false
      thermostat_objects.each do |object|
        if object.name.to_s == thermostat
          thermostat = object
          found_thermostat = true
          break
        end
      end
      if not found_thermostat
        puts "Can't find thermostat in model matching selected name. Won't set thermostat for selected thermal zones."
      end

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

          parent_zone = drawing_interface.model_object.thermalZone
          if not parent_zone.empty?

            if found_thermostat
              # clone and rename thermostat
              new_thermostat = thermostat.clone(model).to_Thermostat.get
              new_thermostat.setName("#{parent_zone.get.name} Thermostat")

              # setThermostat will delete thermostat previouslly associated with the zone
              parent_zone.get.setThermostat(new_thermostat)
            end

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
