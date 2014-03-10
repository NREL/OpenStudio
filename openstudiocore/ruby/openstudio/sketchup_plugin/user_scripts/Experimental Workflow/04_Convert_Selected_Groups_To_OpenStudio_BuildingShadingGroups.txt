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

# Each user script is implemented within a class that derives from OpenStudio::Ruleset::UserScript
class ConvertGroupsToOpenStudioBuildingShadingGroups < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Convert Selected SketchUp Groups To OpenStudio Building Shading Groups"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    return result
  end

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end

    # get sketchup model
    suModel = Sketchup.active_model
    entities = suModel.active_entities #not using this, running script on selection instead of everything
    selection = suModel.selection

    #make array of groups, and make unique
    groupsAndComponents = []
    puts "Status: Making array of groups."
    selection.each do |entity|
      if entity.class.to_s == "Sketchup::Group" or entity.class.to_s == "Sketchup::ComponentInstance"
        if entity.drawing_interface == nil
          entity.make_unique #this is only needed if a group was copied.
          groupsAndComponents << entity
        else
          puts "#{entity.drawing_interface.model_object.name} is already an OpenStudio Model Object."
        end
      end
    end

    #loop through groups
    puts "Status: Creating shading group geometry using observers."
    groupsAndComponents.each do |groupAndComponent|

      #store name of group
      groupAndComponent_name = groupAndComponent.name #not sure if this works for components

      # make new shading group
      shading_group = OpenStudio::ShadingSurfaceGroup.new
      shading_group.create_model_object
      shading_group.draw_entity
      shading_group.add_observers
      shading_group.add_watcher
      shading_group.model_object.setName(groupAndComponent_name)

      #move group inside of shading surface group and explode
      entity = shading_group.entity
      if groupAndComponent.class.to_s == "Sketchup::Group"
        tempGroup = entity.entities.add_instance(groupAndComponent.entities.parent, groupAndComponent.transformation*entity.transformation)
      elsif groupAndComponent.class.to_s == "Sketchup::ComponentInstance"
        tempGroup = entity.entities.add_instance(groupAndComponent.definition.entities.parent, groupAndComponent.transformation*entity.transformation)
      end

      #explode temp copy so observers can classify, then erase original geometry
      tempGroup.explode
      groupAndComponent.erase!

    end #end of groupsAndComponents.each do

  end  #end of run

end #end of user script

# this call registers your script with the OpenStudio SketchUp plug-in
ConvertGroupsToOpenStudioBuildingShadingGroups.new.registerWithApplication