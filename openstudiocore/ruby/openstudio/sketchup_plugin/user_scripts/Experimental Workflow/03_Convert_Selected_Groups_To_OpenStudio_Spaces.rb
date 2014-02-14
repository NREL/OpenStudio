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
class ConvertGroupsToOpenStudioSpaces < OpenStudio::Ruleset::ModelUserScript

  # override name to return the name of your script
  def name
    return "Convert Selected SketchUp Groups To OpenStudio Spaces"
  end
  
  # returns a vector of arguments, the runner will present these arguments to the user
  # then pass in the results on run
  def arguments(model)
    result = OpenStudio::Ruleset::OSArgumentVector.new

    manifold_only = OpenStudio::Ruleset::OSArgument::makeBoolArgument("manifold_only",false)
    manifold_only.setDisplayName("Convert Only Manifold Solid Groups? ")
    manifold_only.setDefaultValue(true)
    result << manifold_only

    intersect = OpenStudio::Ruleset::OSArgument::makeBoolArgument("intersect",false)
    intersect.setDisplayName("Intersect Geometry Before Conversion? ")
    intersect.setDefaultValue(true)
    result << intersect

    #todo - add bool for surface matching

    #todo - add bool for assign building stories

    return result
  end

  # override run to implement the functionality of your script
  # model is an OpenStudio::Model::Model, runner is a OpenStudio::Ruleset::UserScriptRunner
  def run(model, runner, user_arguments)
    super(model, runner, user_arguments) # initializes runner for new script

    if not runner.validateUserArguments(arguments(model),user_arguments)
      return false
    end

    manifold_only = runner.getBoolArgumentValue("manifold_only",user_arguments)
    intersect = runner.getBoolArgumentValue("intersect",user_arguments)

    # get sketchup model
    suModel = Sketchup.active_model
    entities = suModel.active_entities #not using this, running script on selection instead of everything
    selection = suModel.selection

    #make array of groups, and make unique
    groupsAndComponents = []
    puts "Status: Making array of groups, checking manifold solid."
    selection.each do |entity|
      if entity.class.to_s == "Sketchup::Group" or entity.class.to_s == "Sketchup::ComponentInstance"
        if entity.drawing_interface == nil
          if entity.manifold?
            entity.make_unique #this is only needed if a group was copied.
            groupsAndComponents << entity
          else
            if manifold_only
              puts "*(warning) Space not created, geometry of group is not a manifold solid."
            else
              entity.make_unique #this is only needed if a group was copied.
              groupsAndComponents << entity
            end
          end
        else
          puts "#{entity.drawing_interface.model_object.name} is already an OpenStudio Model Object."
        end
      end
    end

    #creating intersections
    if intersect
      puts "Status: Intersecting group geometry before making spaces."
      groupsAndComponents.each do |groupAndComponent|
        if groupAndComponent.class.to_s == "Sketchup::Group"
          groupAndComponent.entities.intersect_with(true, groupAndComponent.transformation, groupAndComponent, groupAndComponent.transformation, false, groupsAndComponents)
        elsif groupAndComponent.class.to_s == "Sketchup::ComponentInstance"
          groupAndComponent.definition.entities.intersect_with(true, groupAndComponent.transformation, groupAndComponent.definition, groupAndComponent.transformation, false, groupsAndComponents)
        end
      end
    end

    #loop through groups
    puts "Status: Creating space geometry using observers."
    groupsAndComponents.each do |groupAndComponent|

      #store name of group
      groupAndComponent_name = groupAndComponent.name #not sure if this works for components

      # make new space
      space = OpenStudio::Space.new
      space.create_model_object
      space.draw_entity
      space.add_observers
      space.add_watcher
      space.model_object.setName(groupAndComponent_name)

      #move group inside of space and explode
      entity = space.entity
      if groupAndComponent.class.to_s == "Sketchup::Group"
        tempGroup = entity.entities.add_instance(groupAndComponent.entities.parent, groupAndComponent.transformation*entity.transformation)
      elsif groupAndComponent.class.to_s == "Sketchup::ComponentInstance"
        tempGroup = entity.entities.add_instance(groupAndComponent.definition.entities.parent, groupAndComponent.transformation*entity.transformation)
      end

      #explode temp copy so observers can classify, then erase original geometry
      tempGroup.explode
      groupAndComponent.erase!

    end #end of groupsAndComponents.each do

    #doesn't support nested groups to be used for shading an interior partition surfaces - todo

    #surface matching had sync issue - todo

    #clean up origin may had problems as well - todo

  end  #end of run

end #end of user script

# this call registers your script with the OpenStudio SketchUp plug-in
ConvertGroupsToOpenStudioSpaces.new.registerWithApplication
